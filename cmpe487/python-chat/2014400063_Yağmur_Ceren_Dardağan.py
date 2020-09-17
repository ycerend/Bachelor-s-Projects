import time as t
import hashlib
import threading
import subprocess
import socket

host = subprocess.check_output("ifconfig " + "en0" + " | awk '/inet / {print $2}'", shell=True).decode().split("\n")[0]
port = 5000
nıck = "ceren"
buffer_sıze = 1024
people = {}

def receive_discovery(port):
    discovery_receive_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    discovery_receive_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    discovery_receive_socket.bind((host, port))
    while True:
        message, (ip, port) = discovery_receive_socket.recvfrom(buffer_sıze)
        print("message : " + ip)
        if message:
            message_decoded = message.decode().split(";")
            if message_decoded[0] == '1':
                people[message_decoded[1]] = [message_decoded[2],"none",[]]
                print(message_decoded[1] + "  " + people[message_decoded[1]][0])
            if message_decoded[0] == '0':
                people[message_decoded[1]] = [message_decoded[2], "none", []]
                print(message_decoded[1] + "  " + people[message_decoded[1]][0])
                messagetosend = str(1) + ";" + host + ";" + nıck + ";" + ip + ";" + message_decoded[2]
                discovery_receive_socket.sendto(messagetosend.encode(), (ip,5000))



def send_discovery():
    discovery_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    discovery_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    ip_addr = host.split(".")
    ip_base = ".".join([ip_addr[0], ip_addr[1], ip_addr[2]])
    for x in range(2, 254):
        ip_addr_real = ip_base + "." + str(x)
        messagetosend = str(0) + ";" + host + ";" + nıck + ";" + "0" + "0"
        discovery_socket.sendto(messagetosend.encode(), (ip_addr_real,5000))

def receive_message(port):
    receive_message_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    receive_message_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    receive_message_socket.bind((host, port))
    while True:
        message,(ip,port) = receive_message_socket.recvfrom(buffer_sıze)
        if message:
            real_message = message.decode().split(";")
            ip_sender = real_message[0]
            cypher_from_sender = real_message[1]
            message_content = real_message[2]
            cypher_previous = people[ip_sender][1]

            if cypher_previous == "none" :
                people[ip_sender][1] = hashlib.md5(cypher_from_sender.encode('utf-8')).hexdigest()
                people[ip_sender][2].append(people[ip_sender][0] + " : " + message_content)
            else :
                cypher_to_be = hashlib.md5(cypher_previous.encode('utf-8')).hexdigest()
                if cypher_to_be == cypher_from_sender :
                    people[ip_sender][1] = hashlib.md5(cypher_from_sender.encode('utf-8')).hexdigest()
                else :
                    print("message interrupted,information is going to be deleted")
                    people.pop(ip_sender)



def send_message():
    send_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    send_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    for ip,info_array in people.items():
        print(info_array[0])
    sender_ip = ""
    client_or_quit = input("type a name to chat or type exit to exit : ")
    if client_or_quit == "exit":
        return True
    for ip,info_array in people.items():
        if info_array[0] == client_or_quit:
            sender_ip = ip
    if sender_ip == "":
        print("this person is not in the list")
        return False
    print("chat story :")
    for val in people[sender_ip][2] :
        print(val)
    messagetosend = input("type your message: ")
    people[sender_ip][2].append(people[host][0] + " : " + messagetosend)
    messagetosend_toencode = host + ";" + people[sender_ip][1] + ";" + messagetosend
    send_socket.sendto(messagetosend_toencode.encode(), (sender_ip,5000))
    return False

if __name__ == '__main__':
    main_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    main_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    send_discovery()
    receive_discovery_thread = threading.Thread(name='receive_discovery', target=receive_discovery, args=[5000])
    receive_discovery_thread.start()
    t.sleep(10)
    receive_message_thread = threading.Thread(name='receive_message', target=receive_message, args=[5001])
    receive_message_thread.start()
    while True:
        
        if send_message():
            break

    


