from BonsaiSender import BonsaiSender
import time
import csv
sender = BonsaiSender()

time.sleep(3)
with open('test_sender_timing.csv', "w+") as f:
    writer = csv.writer(f)
    for i in range(10):
        
        sender._send_string(f'test_1_{i}')
        writer.writerow([f'test_1_{i}', time.time()])
        time.sleep(0.5)
        sender._send_string(f'test_2_{i}')
        writer.writerow([f'test_2_{i}', time.time()])
        time.sleep(1)