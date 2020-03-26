import socket
import sys
HOST = ''
PORT = int(sys.argv[1])


socket_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Socket created')

try:
    socket_server.bind((HOST, PORT))
except socket.error as msg:
    print('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
    sys.exit()

print('Socket bind complete')
socket_server.listen(1)
print('Socket now listening')

while 1:
    try:
        conn, addr = socket_server.accept()
        print ('Connected with ' + addr[0] + ':' + str(addr[1]))
        data = conn.recv(1024).decode()


        data_file = data.split()[1]
        f = open(data_file[1:],'rb')
        fp = f.read()
        conn.send(b"HTTP/1.1 200 OK\r\n\r\n")
        conn.send(fp)
        conn.close()
    except:
        conn.send(b"HTTP/1.1 404 Not Found\r\n\r\n")
        conn.send(b"404 Not Found")
        conn.close()


socket_server.close()