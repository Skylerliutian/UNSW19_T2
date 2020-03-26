# Python version 3.7.2

from socket import *
from datetime import datetime
import time
import sys

serverIP = sys.argv[1]
serverPort = int(sys.argv[2])

# create an UDP socket
clientSocket = socket(AF_INET, SOCK_DGRAM)
# in order to compute max, min and average rtt
all_rtt = []

for i in range(10):
    # get the current time
    now_time = time.strftime('%Y-%m-%d %H:%M:%S ',time.localtime())
    # the message to be sent to the server
    msg = 'PING ' + str(i) + ' ' + now_time + '\r\n'
    # time when the message is sent
    start_time = time.time()
    # send the message
    clientSocket.sendto(msg.encode(), (serverIP, serverPort))

    try:
        clientSocket.settimeout(1)
        response, serverAddress = clientSocket.recvfrom(1024)
        # time when the message is received by client
        end_time = time.time()
        rtt = round((end_time - start_time) * 1000)
        all_rtt.append(rtt)
        print(f"ping to {serverIP}, seq = {i + 1}, rtt = {rtt} ms")

    except timeout:
        # the client does not receive any reply from the server
        print(f'ping to {serverIP}, seq = {i + 1}, time out')

print(f'Minimum RTT = {min(all_rtt)} ms')
print(f'Maximum RTT = {max(all_rtt)} ms')
print(f'Average RTT = {round(float(sum(all_rtt) / len(all_rtt)))} ms')
clientSocket.close()






