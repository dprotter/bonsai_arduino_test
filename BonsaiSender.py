import time
import pandas as pd
import queue
import serial as ser
from concurrent.futures import ThreadPoolExecutor
import inspect 


def thread_it(func):
        '''simple decorator to pass function to our thread distributor via a queue. 
        these 4 lines took about 4 hours of googling and trial and error.
        the returned 'future' object has some useful features, such as its own task-done monitor. '''
        
        def pass_to_thread(self, *args, **kwargs):
            bound_args = inspect.signature(func).bind(self, *args, **kwargs)
            bound_args.apply_defaults()
            bound_args_dict = bound_args.arguments

            new_kwargs = {k:v for k, v in bound_args_dict.items() if k not in ('self')}
            #print(f'submitting {func}')
            future = self.thread_executor.submit(func,self, **new_kwargs)
            return future
        return pass_to_thread

class BonsaiSender:

    # SENDER is the object that sends information to Bonsai which includes timestamps and information on what event has occured.
    #        The arduino will send analog signals via Firmata on the specified pin. Those values can be between 
    # INPUTS: port (str) - path of the serial port to connect to, defaults to GPIO serial of the pi
    #         baud (int) - Baud rate that the serial port runs on (default 9600 to match arduino)
    #         commandFile (str) - path of the commands.csv file where the commands are located

    def __init__(self, port = '/dev/serial0', baud = 9600):
        # Set the initial properties
        print('initializing sender')
        self.thread_executor = ThreadPoolExecutor(max_workers = 5)
        self.finished = False
        self.sending = False
        
        self.port        = port
        self.baudRate    = baud
        self.history     = queue.Queue()
        self.command_stack = queue.Queue()
        self.timeout = 2
        # Initialize the port
        try:
            self.ser = ser.Serial(self.port, self.baudRate)
            self.command_dict = self.get_commands() # Assign the commands property

            start = time.time() 
            self.send_data('startup_test')
            
            while self.sending and time.time() - start < self.timeout:
                time.sleep(0.05)
            finished = time.time()
            if finished - start > self.timeout:
                print('serial sender failed to send test message ')
        except:
            print('serial sender failed setup. If not sending serial data for Bonsai integration, ignore this warning.')
        
        self.active = True


    def busy(self):
        return self.sending

    def shutdown(self):
        self.finished = True

    def running(self):
        return self.active
    
    @thread_it
    def run(self):
        while not self.finished:
            
            if not self.command_stack.empty():
                command = self.command_stack.get()
                
                self._send_data(command)
                
            time.sleep(0.05)

        while not self.command_stack.empty():
            
            command = self.command_stack.get()
            self._send_data(command)
            self.sleep(0.05)
        self.active = False

    def send_data(self, pin, value):
        self.command_stack.put(f'{pin}|{value}')
     


    def _send_data(self, command):
        # SEND_DATA sends the data through the associated serial port, and then logs all the commands that have been send to the self.history queue object.
        self.sending = True
        if not command in self.command_dict.keys():
            print(f'WARNING: "{command}" is not a valid command being sent, will not be read by the Arduino and Bonsai')
            print(self.command_dict.keys())
            return 
        elif not self.command_dict[command]['send to bonsai']:
            print(f'WARNING: command {command} was passed to the serial encoder, but attribute "send to bonsai" is FALSE. This will not be sent off the pi.') 
            return
        else:
            formatted = command + '\r'
            formatted = formatted.encode('ascii')
            
            self.ser.write(formatted)
        print(f'\n\nserial message sent: {command}\n\n')
        self.sending = False
    
    def get_commands(self):
        # GET_COMMANDS gets the list of possible command names from a previously defined file in csv format. 

        commDict = pd.read_csv(self.commandFile, index_col=0).to_dict('index')
        return commDict