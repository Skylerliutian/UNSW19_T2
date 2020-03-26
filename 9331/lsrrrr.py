# coding=utf8

'''
    Author: Yuji Ruan
    ID: z5046341
'''

import json
import socket
import sys
import threading
import time
import traceback

UPDATE_INTERVAL = 1  # 1 second
NODE_EXPIRE_TIME = 3
ROUTE_UPDATE_INTERVAL = 30  # 30 seconds
DEBUG_MODE = False


'''
node entity.
'''
class Node(object):
    def __init__(self, id, port, neighbours, neighbour_ports, status=False):
        self.id = id
        self.port = str(port)
        self.neighbours = neighbours            # dict neighbour_id -> cost
        self.neighbour_ports = neighbour_ports  # dict neighbour_id -> port
        self.status = status

    def set_status(self, status):
        self.status = status

    def set_neighbour_cost(self, node_id, value):
        self.neighbours[node_id] = value

    def set_neighbour_ports(self, neighbour_ports):
        self.neighbour_ports = neighbour_ports

    def get_neighbour_ports(self, node_id):
        if node_id not in self.neighbour_ports.keys():
            debug("error, get_neighbour_ports, node not found.")
            return None
        return self.neighbour_ports[node_id]

    def get_neighbour_cost(self, node_id):
        if node_id not in self.neighbours.keys():
            return -1
        return self.neighbours[node_id]

    def is_neighbour(self, node_id):
        return node_id in self.neighbours

    def __eq__(self, other):
        return self.id == other.id

    def __hash__(self):
        return hash(self.id)

    def __repr__(self):
        return 'node[ id:' + self.id + ' | port:' + self.port + ' | status:' + str(
            self.status) + ' | neighbours: ' + str(self.neighbours) + ' ]'


class Graph(object):
    def __init__(self):
        self.nodes = dict()
        self.update_times = dict()  # id -> update time

    def get_node(self, node_id):
        if node_id not in self.nodes.keys():
            debug('node not found, id:', node_id)
            return None
        return self.nodes[node_id]

    def add_node(self, node):
        self.nodes[node.id] = node
        self.update_times[node.id] = time.time()

    def set_node(self, node):
        self.nodes[node.id] = node
        self.update_times[node.id] = time.time()

    def set_node_status(self, node_id, status):
        self.nodes[node_id] = status
        self.update_times[node_id] = time.time()

    def get_node_neighbours(self, node_id):
        if node_id not in self.nodes.keys():
            return []
        if self.nodes[node_id].neighbours is None:
            return []
        return self.nodes[node_id].neighbours

    '''
    graph = {'a': {'b': 14, 'c': 9, 'd': 7},
             'b': {'a': 14, 'c': 2, 'e': 9},
             'c': {'a': 9, 'b': 2, 'd': 10, 'f': 11},
             'd': {'a': 7, 'c': 10, 'f': 15},
             'e': {'b': 9, 'f': 6},
             'f': {'c': 11, 'd': 15, 'e': 6}}
    '''
    def get_graph(self):
        graph_dict = dict()
        for n_id in self.nodes:
            if n_id not in self.nodes.keys():
                continue
            node = self.get_node(n_id)
            if node.neighbours:
                graph_dict[n_id] = node.neighbours
        return graph_dict

    '''
    if other node info has 3s without update, remove that node.
    '''
    def update_graph(self):
        expire_nodes = []
        for node_id in self.nodes.keys():
            if node_id == NODE_ID:
                continue
            if self.update_times[node_id] + NODE_EXPIRE_TIME < time.time():
                expire_nodes.append(node_id)
        # debug('expire node:', expire_nodes)

        for n_id in expire_nodes:
            # remove node from nodes
            del self.nodes[n_id]

            # remove node from nodes neighbours & ports
            for nn in self.nodes.values():
                if n_id in nn.neighbours.keys():
                    del nn.neighbours[n_id]
                if n_id in nn.neighbour_ports.keys():
                    del nn.neighbour_ports[n_id]

            # remove node from update_time dict
            del self.update_times[n_id]

    def __repr__(self):
        return 'graph:' + self.nodes


'''
msg format: node_id, neighbours, from_nodes. dict -> json -> byte.
'''
def encode_msg(node_id, neighbours, ports, from_nodes):
    msg_dict = dict()
    msg_dict['id'] = node_id
    msg_dict['neighbours'] = neighbours
    msg_dict['ports'] = ports
    msg_dict['from'] = from_nodes
    return json.dumps(msg_dict).encode()


'''
decode msg from socket, byte -> json -> dict.
return node_id, neighbours
'''
def decode_msg(data):
    msg_dict = json.loads(data.decode())
    return msg_dict


'''
target address (port)
'''
def send_link_state(ports, data, rcvfrom):
    for target in ports:
        if str(rcvfrom) != str(target):
            addr = ('127.0.0.1', int(target))
            sock.sendto(data, addr)
            debug('[send] node:', NODE_ID, 'data:', data, 'to:', addr)
    return


def send_self():
    neighbours = graph.get_node_neighbours(NODE_ID)
    ports = graph.get_node(NODE_ID).neighbour_ports
    from_nodes = [NODE_ID]
    data = encode_msg(NODE_ID, neighbours, ports, from_nodes)
    send_link_state(ports.values(), data, None)
    return


'''
transmit route to indirect connected neighbours.
a -> b -> c, b transmit a's route to c.
'''
def transmit_msg(msg_dict):
    node_origin_id = msg_dict['id']
    from_nodes = msg_dict['from']
    self_neighbours = graph.get_node_neighbours(NODE_ID)
    ports = []

    for nid in self_neighbours:
        # filter origin sender.
        if nid == node_origin_id:
            continue

        # filter already through node.
        if nid in from_nodes:
            continue

        n_node = graph.get_node(nid)
        if n_node is None:
            continue

        # filter current neighbour node has connection with origin node.
        if n_node.is_neighbour(node_origin_id):
            continue

        ports.append(n_node.port)

    if ports:
        from_nodes.append(NODE_ID)
        data = encode_msg(node_origin_id, msg_dict['neighbours'], msg_dict['ports'], from_nodes)
        send_link_state(ports, data, None)


def send_loop():
    while not thread_stop_flag:
        time.sleep(UPDATE_INTERVAL)
        send_self()
    return


def wait_packet_thread():
    while not thread_stop_flag:
        try:

            data, (ip, port) = sock.recvfrom(1024)
            msg_dict = decode_msg(data)
            # debug('[rcv] current:', NODE_ID, 'data:', data, 'from port:', port, 'node_from', msg_dict['from'])
            node = Node(msg_dict['id'], port, msg_dict['neighbours'], msg_dict['ports'])
            debug('rcv node', node)
            if node not in graph.nodes.values():
                graph.add_node(node)
            else:
                graph.set_node(node)

            # transmit msg.
            transmit_msg(msg_dict)

        except socket.timeout as e:
            print('[wait_thread]', e)

        except Exception as err:
            debug('[wait_thread]', err)
            debug("Unexpected error:", sys.exc_info()[0], sys.exc_info()[1])
            debug(traceback.print_exc())

        # update graph node
        graph.update_graph()

    return


def make_routes():
    global graph
    while not thread_stop_flag:
        count = 0
        while count < ROUTE_UPDATE_INTERVAL and thread_stop_flag == False:
            time.sleep(1)
            count += 1
        if count == ROUTE_UPDATE_INTERVAL:
            print('#####################################################')
            debug('[graph]', graph.nodes)
            if len(graph.nodes) > 1:
                for n_id in graph.nodes.keys():
                    if n_id == NODE_ID:
                        continue
                    cal_path(n_id)
            else:
                print("[routes] this is the only node in network")
            print('#####################################################')
            print()
    return


def cal_path(end):
    path, cost = shortest_path(graph.get_graph(), NODE_ID, end)
    path_str = ''
    for p in path:
        path_str += str(p)
    print('least-cost path to node', end, ':', path_str, 'and the cost is', float(cost))


'''
    use for debug.
'''
def debug(*args):
    if DEBUG_MODE:
        msg = ''
        for arg in args:
            msg += ' ' + str(arg)
        print('[DEBUG]', msg)


def init_graph(graph, config, id, port):
    with open(config) as f:
        lines = f.readlines()

    lines = [line.rstrip('\n') for line in lines]
    debug(lines)
    num_neighbours = int(lines[0])
    debug(num_neighbours)
    neighbours_dict = dict()
    neighbour_ports_dict = dict()
    for i in range(1, num_neighbours + 1):
        tmp = lines[i].split()
        neighbours_dict[tmp[0]] = int(tmp[1])
        neighbour_ports_dict[tmp[0]] = tmp[2]
    node = Node(id, port, neighbours_dict, neighbour_ports_dict)
    graph.add_node(node)
    debug(node.neighbours)
    debug(neighbours_dict)
    debug(node)


def dijkstra(graph_dict, start, end):

    if start not in graph_dict:
        raise ValueError('missing {0}'.format(start))
    if end not in graph_dict:
        raise ValueError('missing {0}'.format(end))

    nodes = []
    for key in graph_dict.keys():
        nodes.append(key)

    f = float('inf')
    dist_from_start = {n: f for n in nodes}
    dist_from_start[start] = 0
    predecessors = {n: None for n in nodes}

    while len(nodes) > 0:
        candidates = {n: dist_from_start[n] for n in nodes}
        closest = min(candidates, key=candidates.get)

        for n in graph_dict[closest]:
            if n not in dist_from_start:
                msg = 'missing node {0} (neighbor of {1})'.format(n, closest)
                raise ValueError(msg)
            dist_to_n = graph_dict[closest][n]
            if dist_to_n < 0:
                msg = 'negative distance from {0} to {1}'.format(closest, n)
                raise ValueError(msg)
            d = dist_from_start[closest] + dist_to_n
            if dist_from_start[n] > d:
                dist_from_start[n] = d
                predecessors[n] = closest

        nodes.remove(closest)

    return dist_from_start, predecessors


def shortest_path(graph_dict, start, end):
    distances, predecessors = dijkstra(graph_dict, start, end)

    if predecessors[end] is None and start != end:
        return [], distances[end]

    path = [end]
    while path[-1] != start:
        path.append(predecessors[path[-1]])
    path.reverse()

    return path, distances[end]


try:
    NODE_ID = sys.argv[1]
    NODE_PORT = int(sys.argv[2])
    CONFIG = sys.argv[3]
    debug(NODE_ID, NODE_PORT, CONFIG)
except Exception as e:
    print(e)
    print("invalid input arguments, usage:")
    print("python lsr node_id node_port config")
    sys.exit(1)

# init socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(3)  # 3 seconds
sock.bind(("", NODE_PORT))

# init graph.
graph = Graph()
init_graph(graph, CONFIG, NODE_ID, NODE_PORT)

# init thread
thread_stop_flag = False

print('Node:', NODE_ID, 'Port:', NODE_PORT)

send_self()
loop_thread = threading.Thread(target=send_loop)
loop_thread.start()
listen_thread = threading.Thread(target=wait_packet_thread)
listen_thread.start()
route_thread = threading.Thread(target=make_routes)
route_thread.start()

# start thread
try:
    while 1:
        next
except KeyboardInterrupt:
    thread_stop_flag = True
    loop_thread.join()
    listen_thread.join()





















    