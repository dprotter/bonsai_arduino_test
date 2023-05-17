from BonsaiSender import BonsaiSender
import time
sender = BonsaiSender()

while True:
    time.sleep(1)
    sender._send_string('lever_out_food')
    time.sleep(1)
    sender._send_string('lever_out_door_2')