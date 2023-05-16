from BonsaiSender import BonsaiSender
import time
sender = BonsaiSender()

time.sleep(1)
sender.send_data('A2', 500)
time.sleep(1)
sender.send_data('A2', 250)