
def download(self, file_info):
    file_id = file_info['id']
    chunks = (1.0 * file_info['size']) / CHUNK_SIZE
    chunks = math.ceil(chunks)

    chunk_set = {i for i in range(chunks)}
    chunks_on_flight = 0
    threads = []
    while len(chunk_set) != 0 or chunks_on_flight != 0:
        file_info = get_file_info(file_id)
        clients = file_info['clients']
        while len(chunk_set) != 0 and chunks_on_flight < self.chunks_at_a_time:
            chunk_id = chunk_set.pop()
            client = clients[randint(0, len(clients) - 1)]
            args = (get_idle_port(), client['ip'], client['port'], file_info['name'], chunk_id)
            thread = Thread(target=receive, args=args)
            thread.start()
            chunks_on_flight += 1
            threads.append(thread)

        for thread in list(threads):
            if not thread.is_alive():
                print('ended')
                chunks_on_flight -= 1
                threads.remove(thread)


def get_idle_port():
    result = 1
    tries = 1000
    port = 0
    while result != 0 and tries > 0:
        port = randint(PORT_RANGE[0], PORT_RANGE[1])
        sock = s.socket(s.AF_INET, s.SOCK_DGRAM)
        result = sock.connect_ex(('127.0.0.1', port))
        tries -= 1

    if tries == 0:
        raise Exception()
    return port

def receive(port_to_bind, host_ip, host_port, file_name, chunk_id):
    socket = s.socket(s.AF_INET, s.SOCK_DGRAM)
    socket.bind(("", get_idle_port()))
    # Start connection
    msg = "Info;" + str(file_name) + ";" + str(chunk_id) + ";" + str(port_to_bind)
    socket.sendto(msg.encode(), (host_ip, host_port))
    # Wait for ACK
    data, addr = socket.recvfrom(4096)
    # Set rwnd
    if data.decode().split(";")[0] == "ACK":
        receive_chunk(port_to_bind)
    socket.close()
    print('Download started')
    print('{} {} {} {} {}'.format(port_to_bind, host_ip, host_port, file_name, chunk_id))
    # send_info()
    # receive_chunk(port_to_bind)
    sleep(randint(1, 10))
    print('Download complete')
    return True


def receive_chunk(PORT_TO_BIND):
    chunk = []
    # Initialize rwnd
    rwnd = 5
    # Initialize socket as Datagram socket.
    socket = s.socket(s.AF_INET, s.SOCK_DGRAM)
    socket.bind(("", PORT_TO_BIND))
    # Wait for start of the connection.
    data, addr = socket.recvfrom(4096)
    received_chunk = data.decode().split(";")[1]
    if data.decode().split(";")[0] == "Start":
        socket.sendto(("ACK;" + str(rwnd)).encode(), addr)
    index = 0
    socket.setblocking(True)
    while (True):
        try:
            # Receive data
            data = socket.recv(4096)
            print(data.decode())
            packet_num = data.decode().split(";")[0]
            if (packet_num == "EOF"):
                socket.sendto(b'ACKEOF;;', addr)
                break
            chunk.append(data.decode().split(";")[1])
            print(index, packet_num)
            if (index == int(packet_num)):
                # rwnd += 1
                # Send ACK
                print("Sent ACK {}".format(packet_num))
                packet = "ACK;" + str(rwnd) + ";" + packet_num
                socket.sendto(packet.encode(), addr)
                index += 1
        except s.error as e:
            print("Socket error: {}".format(e))
    return chunk