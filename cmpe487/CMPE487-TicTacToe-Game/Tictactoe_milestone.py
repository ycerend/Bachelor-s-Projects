import time as t
import hashlib
import threading
import subprocess
import socket
from sys import stdin
from random import randint
import os

host = subprocess.check_output("ifconfig " + "en0" + " | awk '/inet / {print $2}'", shell=True).decode().split("\n")[0]
discover_port = 5000
nickname = "ceren"
buffer_sıze = 1024
USERS = []
ADRESSES = {}
isAvailable = True

def get_input():
    return stdin.readline().rstrip(" \n\r")


def receive_discovery():
    global isAvailable
    discovery_receive_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    discovery_receive_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    discovery_receive_socket.bind((host, discover_port))
    while True:
        message,address = discovery_receive_socket.recvfrom(buffer_sıze)
        if message:
            try:
                mod, senderip, sendername, recv, order = message.decode().split(";")
            except ValueError:
                print("value error")
            if mod == "0":
                discover_pck = "1;" + host + ";" + nickname + ";" + senderip + ";" + sendername
                discovery_receive_socket.sendto(discover_pck.encode(), (senderip,discover_port))
                if sendername not in USERS:
                    USERS.append(sendername)
                    ADRESSES[sendername] = senderip
            if mod == "1":
                if sendername not in USERS:
                    USERS.append(sendername)
                    ADRESSES[sendername] = senderip
            if mod == "2":
                input == "R"
                print( sendername + " wants to play a game with you!! A for accept, R for reject")
                decide_input = get_input()
                isAvailable = True
                print(isAvailable)
                if isAvailable and decide_input == "A":
                    rand = randint(0, 9)
                    if rand > int(recv) :
                        discover_pck = "3;" + host + ";" + nickname + ";" + senderip + ";0"
                    else :
                        discover_pck = "3;" + host + ";" + nickname + ";" + senderip + ";1"
                    discovery_receive_socket.sendto(discover_pck.encode(), (senderip,discover_port))

                    #acceptse availability i false edicek mesaj 4 le dönücek
                else:
                    discover_pck = "4;" + host + ";" + nickname + ";" + senderip + ";0"
                    discovery_receive_socket.sendto(discover_pck.encode(), (senderip,discover_port))
                    print(sendername + " is not available")
                print(discover_pck)
            if mod == "3":
                print(sendername + " has accepted your invitation")
                isAvailable = False
                if order == "0":
                    #önce karşı taraf başlıcak demek dinlemeye göndericek
                    print(sendername + " is going to start as X")
                if order == "1":
                    #biz başlıyoruz
                    print("Your turn!")
            if mod == "4":
                print(sendername + " is not available")
            if mod == "-1":
                #user listesinden sendername i çıkarıcaz, kül et bu userı
                print(sendername + " is not online")

def send_invitation(input):
    invitation_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    invitation_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    rand = randint(0, 9)
    print("girdi " + str(rand))
    try:
        ip_addr = ADRESSES[input]
        messagetosend = str(2) + ";" + host + ";" + nickname + ";" + str(rand) + ";0"
        invitation_socket.sendto(messagetosend.encode(), (ip_addr,discover_port))
    except Exception as e:
        pass
        print("not able to send")

def send_discovery():
    discovery_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    discovery_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    ip_addr = host.split(".")
    ip_base = ".".join([ip_addr[0], ip_addr[1], ip_addr[2]])
    #print(host)
    for x in range(2, 254):
        ip_addr_real = ip_base + "." + str(x)
        messagetosend = str(0) + ";" + host + ";" + nickname + ";" + "0" +";"+ "0"
        discovery_socket.sendto(messagetosend.encode(), (ip_addr_real,discover_port))

def exit():
    print("Sending a message with mod -1 to everyone")
    exit_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    exit_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    for username in USERS:
        ip_addr = ADRESSES[username]
        messagetosend = str(-1) + ";" + host + ";" + nickname + ";" + "0;" + "0"
        exit_socket.sendto(messagetosend.encode(), (ip_addr,discover_port))


print("Please enter your nickname:")
nickname = get_input()
print("WELCOME ", nickname)
if __name__ == '__main__':
    main_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    main_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    receive_discovery_thread = threading.Thread(name='receive_discovery', target=receive_discovery)
    receive_discovery_thread.start()
    send_discovery()
while True:
    print("1. Show online users")
    print("2. Rediscover")
    print("3. Play game")
    print("4. Exit")
    input = get_input()
    if input == "1":
        print("# Users")
        for u in USERS: #users
            print("#   " + u)
        stdin.readline()
    elif input == "2":
        send_discovery()
    elif input == "3":
        print("With whom?")
        for u in USERS:
            print("#   " + u)
        input = get_input()
        if input in USERS:
            send_invitation(input)
            #receive threadi cevabı bekliycek cevap 4 se kabul; değilse reddettik
            print("Implement:), first send invitation to that user then start playing if that user accepts it")
        # while True:
        #     print("### Room - %s" % input +" ###")
        #     for msg in message_list[input]:
        #         print(msg)
        #     print("----------------")
        #     msg = get_input()
        #     if msg != "{q}":
        #         #message_list[input].append("<You>: " + msg)
        #         send_message(input, msg)
        #     else:
        #         break
        else:
            print("user is not online")
        stdin.readline()

    elif input == "4":
        exit()
        print("###### GOODBYE ", nickname," ######")
        os._exit(0)
    else:
        continue
