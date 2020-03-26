#!/usr/bin/python3.7
import sys
import json
import time
from copy import deepcopy
import threading
from socket import *

host = '127.0.0.1'
UPDATE_INTERVAL = 1
FAIL_TIME = 3
HEARTBEAT = 0.7
ROUTE_UPDATE_INTERVAL = 30
file = sys.argv[1]


# encode graph before send to neighbours
def encode_data(graph_dic):
    str_graph_dic = json.dumps(graph_dic)
    return str_graph_dic


# decode data into graph after receive data
def decode_data(data):
    message = json.loads(bytes.decode(data))
    return message


# graph received from broadcast and then forward to neighbours
def forward_msg(graph, addr):
    new_dict = {v: k for k, v in neighbor_port.items()}
    sent_by = new_dict[int(addr[1])]
    forward_who = list(graph.keys())[0]
    self_neighbor_port = [i for i in forward_graph[routerid].keys() if i != sent_by]
    # receive message from direct neighbour
    if forward_who == sent_by:
        send_msg(self_neighbor_port, graph)
    # receive message from indirect neighbour
    else:
        if routerid not in graph[forward_who]:
            send_msg(self_neighbor_port, graph)


# function used to receive data
def recv_msg():
    while True:
        data, addr = sock_et.recvfrom(2048)
        graph = decode_data(data)
        forward_msg(graph, addr)
        update_graph(graph)


# function used to send data
def send_msg(n_port, data):
    data_encode = encode_data(data)
    for i in n_port:
        addr = ('127.0.0.1', neighbor_port[i])
        sock_et.sendto(data_encode.encode(), addr)


# function used to send own link-state only, every 1 second
def send_msg_self():
    while True:
        time.sleep(1)
        port = neighbor_port
        send_msg(port, forward_graph)


# update updating_graph while receive data
def update_graph(graph):
    fail_nodes = []
    recv_id = list(graph.keys())[0]

    # 1. receive direct neighbour
    if recv_id in neighbor_port:

        # assume that my neighbour is not fail
        if recv_id in record_times:
            for i in record_times:
                if recv_id == i:
                    if record_times[i] + FAIL_TIME * HEARTBEAT < time.time():
                        fail_nodes.append(i)
                    else:
                        updating_graph[recv_id] = graph[recv_id]
                        record_times[recv_id] = time.time()
                else:
                    if record_times[i] + FAIL_TIME * HEARTBEAT < time.time():
                        fail_nodes.append(i)
        # assume that failure neighbour restart
        else:
            record_times[recv_id] = time.time()
            updating_graph[recv_id] = graph[recv_id]
            updating_graph[routerid][recv_id] = graph[recv_id][routerid]
            forward_graph[routerid][recv_id] = graph[recv_id][routerid]

    # 2. receive data from indirect neighbour（可以判断对应的信息是否一致，从而判断是否有节点失败）
    else:
        updating_graph[recv_id] = graph[recv_id]

    # Traversing fail_nodes list and update updating_graph
    for i in fail_nodes:
        del forward_graph[routerid][i]
        del record_times[i]
        if i in updating_graph:
            del updating_graph[i]
        for j in updating_graph:
            if i in updating_graph[j]:
                del updating_graph[j][i]
    return


# get details from CONFIG.txt
def read_file(file_name):
    graph_dic = {}
    record = {}
    with open(file_name) as f:
        list_f = f.readlines()
    router = list_f[0].split()[0]
    port = int(list_f[0].split()[1])
    neighbor_dic = {}
    neighbor_port_list = {}
    for i in range(2, len(list_f)):
        if len(list_f[i].split()) == 3:
            neighbor_dic[list_f[i].split()[0]] = float(list_f[i].split()[1])
            neighbor_port_list[list_f[i].split()[0]] = int(list_f[i].split()[2])
    graph_dic[router] = neighbor_dic
    for i in graph_dic[router]:
        record[i] = time.time()
    return router, port, graph_dic, neighbor_port_list, record


# dijkstra algorithm
def dijkstra(graph, start):
    if start not in graph.keys():
        return f'ValueError: No {start} on record'

    nodes = [i for i in graph.keys()]
    distance_from_start = {i: float('inf') for i in nodes}
    distance_from_start[start] = 0
    predecessor = {}
    while len(nodes) > 0:
        set_n = {i: distance_from_start[i] for i in nodes}
        w = min(set_n, key=set_n.get)

        for i in graph[w]:
            if i in graph:
                c_w_i = graph[w][i]
                distance_to_i = c_w_i + distance_from_start[w]
                if distance_from_start[i] > distance_to_i:
                    distance_from_start[i] = round(distance_to_i, 1)
                    predecessor[i] = w
        nodes.remove(w)
    return distance_from_start, predecessor


# find least-cost path by calling dijkstra function
def find_path():
    global updating_graph
    time.sleep(ROUTE_UPDATE_INTERVAL)
    start = routerid
    print(f'I am Router {start}')
    distance_from_start, predecessor = dijkstra(updating_graph, start)
    end_list = []
    for i in distance_from_start:
        if distance_from_start[i] != float('inf') and distance_from_start[i] != 0:
            end_list.append(i)
    for i in end_list:
        path = []
        temp = i
        while temp != start:
            path.append(temp)
            temp = predecessor[temp]
        path.append(start)
        path = ''.join(path[::-1])
        dis = distance_from_start[i]
        print(f'Least cost path to router {i}:{path} and the cost is {dis}')
    updating_graph.clear()
    updating_graph[routerid] = forward_graph[routerid]
    find_thread = threading.Timer(0, find_path)
    find_thread.start()


if __name__ == "__main__":

    # global variables
    routerid, portnum, forward_graph, neighbor_port, record_times = read_file(file)

    updating_graph = deepcopy(forward_graph)

    # create socket
    sock_et = socket(AF_INET, SOCK_DGRAM)

    # bind address
    address = ('127.0.0.1', portnum)
    sock_et.bind(address)

    # create threads
    thread_send = threading.Thread(target=send_msg_self)
    thread_recv = threading.Thread(target=recv_msg)
    thread_path = threading.Thread(target=find_path)

    # start
    try:
        thread_send.start()
        thread_recv.start()
        thread_path.start()
    except KeyboardInterrupt:
        thread_send.join()
        thread_recv.join()
        thread_path.join()



