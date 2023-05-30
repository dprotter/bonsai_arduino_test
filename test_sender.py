from BonsaiSender import BonsaiSender
import time
sender = BonsaiSender()

while True:
    time.sleep(1)
    sender._send_string('A1 400')
    time.sleep(1)
    sender._send_string('A2 100')