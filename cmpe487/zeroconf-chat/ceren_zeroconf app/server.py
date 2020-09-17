from socket import *
from threading import Thread
from subprocess import check_output, call
from config import *
from sys import stdin
import os
from hashlib import md5

def get_input():
    return stdin.readline().rstrip(" \n\r")

def discover_broadcast():
    server = socket(AF_INET,SOCK_DGRAM)
    server.setsockopt(SOL_SOCKET,SO_BROADCAST, 1)
    # Set a timeout so the socket does not block
    # indefinitely when trying to receive data.
    server.settimeout(0.2)
    #server.bind(("", discover_port))
    discover_pck = "0;" + host + ";" + host_name + ";" + "" + ";"
    try:
        server.sendto(bytes(discover_pck,"utf8"), ('<broadcast>', discover_port))
    except ConnectionRefusedError:
        pass
        print("connection refused")
    except timeout:
        pass
        print("connection timeout")
    except OSError:
        pass
    server.close()

def send_pck(to,pck):
    sock=socket(AF_INET, SOCK_STREAM)
    sock.settimeout(20)
    print(to)
    try:
        sock.connect(to)
        sock.send(pck)
    except ConnectionRefusedError:
        pass
    except timeout:
        pass
    except OSError:
        pass
    sock.close()



def accept_discovery():
    s = socket(AF_INET, SOCK_DGRAM)
    s.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)
    s.bind(('', discover_port))
    print("hellooo")
    while True:
#        print("hello?")
        try:
            data, addr = s.recvfrom(1024)
            try:
                mod, senderip, sendername, recvip, recvname = (data.decode()).split(";")
            except ValueError:
                pass
#                print("im sad")
            print(str(sendername) + " IS NOW ONLINE!")
            users.append(sendername)
            addresses[sendername] = senderip
            message_list[sendername] = []
            if mod == "0":
                discover_pck = "1;" + host + ";" + host_name + ";" + senderip + ";" + sendername
                send_pck(addr,discover_pck)
            discovery_receive_socket.close()
        except Exception as e:
            pass
#            print(e)
def accept_message():
    while True:
        client, client_address = msg_server.accept()
        client_ip = client_address[0]
        if not client_ip in addresses.values():
            break
        else:
            Thread(target=handle_client, args=(client, client_ip)).start()
def send_message(name, msg):
    sock = socket(AF_INET, SOCK_STREAM)
    sock.connect((addresses[name], 5001))
    pck = ""
    if name in message_encode:
        pck = host + ";" + md5(message_encode[name].encode("utf-8")).hexdigest() + ";" + msg
        message_encode[name] = md5(message_encode[name].encode("utf-8")).hexdigest()
    else:
        pck = host + ";" + md5("".encode("utf-8")).hexdigest() + ";" + msg
        message_encode[name] = md5("".encode("utf-8")).hexdigest()
    try:
        sock.send(bytes(pck,"utf8"))
        sock.settimeout(5)
        message_list[name].append("<You>: " + msg)
    except Exception as e:
        print(e)
        del message_encode[name]
        Thread(target=discover_broadcast).start()
        pass
    sock.close()

def handle_client(client, client_ip):
    name = {v: k for k, v in addresses.items()}[client_ip]
    while True:
        msg = client.recv(buffer_size).decode("utf8")
        if msg != "":
            msg = msg.split(";")
            if len(msg) != 3:
                client.close()
                break
            if name in message_encode:
                if md5(message_encode[name].encode("utf-8")).hexdigest() == msg[1]:
                    message_list[name].append("<"+name+">: " + msg[2])
                    message_encode[name] = msg[1]
                else:
                    print("Someone is pretending to be "+name + "!")
                    client.close()
                    break
            else:
                message_list[name].append("<"+name+">: " + msg[2])
                message_encode[name] = msg[1]
        else:
            client.close()
            break

#def handle_client(client, client_ip):
#    name = {v: k for k, v in addresses.items()}[client_ip]
#    print("connection established with " + name)
#    while True:
#        msg = client.recv(buffer_size)
#        if msg != bytes("{q}", "utf8"):
#            pass
#            write_to_chatapp(msg, name+": ")
#        else:
#            client.send(bytes("{q}", "utf8"))
#            client.close()
#            write_to_chatapp(bytes("%s has left the chat." % name, "utf8"))
#            break

users = []
clients = {}
addresses = {}
message_list = {}
message_encode = {}

#chatapp = socket(AF_INET, SOCK_STREAM)
#while True:
#    try:
#        chatapp.connect(chatapp_addr)
#        break
#    except ConnectionRefusedError:
#        t = call('clear', shell=True)
#        print("Waiting for ChatApp start...")
#        continue

msg_server.bind(msg_addr)
discover_server.bind(discover_addr)
msg_server.listen(30)
discover_server.listen(30)
print("Waiting for connection...")

# Start all the threads!
threads = []
threads.append(Thread(target=discover_broadcast))
threads.append(Thread(target=accept_discovery))
threads.append(Thread(target=accept_message))
for t in threads:
    t.start()

while True:
    #t = call(clear, shell=True)
    print("1. Online users")
    print("2. Message rooms")
    print("3. Discovery")
    print("4. Exit")
    input = get_input()
    if input == "1":
        print("# Users")
        for u in message_list:
            print("#   " + u)
        stdin.readline()
    elif input == "2":
        print("With whom?")
        
        for u in message_list:
            print("#   " + u)
        input = get_input()
    
        if input in users:
            while True:
                t = call(clear, shell=True)
                print("### Room - %s" % input +" ###")
                for msg in message_list[input]:
                    print(msg)
                print("----------------")
                msg = get_input()
                if msg != "{q}":
                    #message_list[input].append("<You>: " + msg)
                    send_message(input, msg)
                else:
                    break
        else:
            print("user is not online")
            stdin.readline()
    elif input == "3":
        Thread(target=discover_broadcast).start()
    elif input == "4":
        os._exit(0)
    else:
        continue
    
    print("deneme")

# Ending the process
for t in threads:
    t.join()
msg_server.close()
discover_server.close()
