# Python version 3.7.2

from socket import *
import sys

Host = '127.0.0.1'
Port = int(sys.argv[1])

server_socket = socket(AF_INET, SOCK_STREAM)

server_socket.bind((Host, Port))

server_socket.listen(1)

print('Initialization completed...')
print(f'connect to {Host}')

while True:
    try:
        connection_socket, address = server_socket.accept()
        http_request = connection_socket.recv(1024).decode()
        print(http_request)
        file_name = (http_request.split()[1])[1:]
        content = open(file_name, 'rb').read()
        connection_socket.send(b"HTTP/1.1 200 OK\r\n\r\n")
        connection_socket.send(content)
        connection_socket.close()
    except:
        connection_socket.send(b"HTTP/1.1 404 Not Found\r\n\r\n")
        connection_socket.send(b'404 Not Found')
        connection_socket.close()


