import socket
import os
import subprocess
from threading import Thread
import math


FILE_SEND_PORT = 3001
FILE_RECEIVE_PORT = 3002
FILE_LISTEN_PORT = 3000
HOST = subprocess.check_output("ifconfig " + "en0" + " | awk '/inet / {print $2}'", shell=True).decode().split("\n")[0]
NAME = "ycd"
UGURCAN = "192.168.4.23"



def receive_file(seederIP, fileName):
	with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
		s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
		s.bind(("", FILE_RECEIVE_PORT))
		file_pck = "2;" + HOST + ";" + NAME + ";" + fileName + ";5;"
		s.sendto(file_pck.encode(),(seederIP, LISTEN_SEEDER_DISCOVERY_PORT))
		index = 0
		s.setblocking(True)
		while (True):
			try:
				data = s.recv(4096)
				packet_num = data.decode().split(";")[0] #sends packet no; data
				if packet_num == "EOF":
					s.sendto("ACKEOF",(seederIP, FILE_SEND_PORT))
					break
				received_file.append("".join(data.decode().split(";")[1]))
				if(index == int(packet_num)): # if we received the chunk in order
					packet = "ACK;"+ str(rwnd)+";"+ packet_num
					send_pck((seeder,discover_port), packet)
					index += 1
			except error as e:
				print("Socket error ", e)
		return received_file

def send_file(client_ip, file_name, rec_rwnd):
	with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
		s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
		s.bind(("", FILE_SEND_PORT))

		file = open("./" + file_name, "rb")
		file_size = getSize(file)
		chunk_number = math.ceil(file_size / 1500.0)
		chunk_index = 0
		in_flight = 0
		s.settimeout(1)
		send_Notfinished = True
		total_sent_packet = 0
		while (True):
			while (in_flight < int(rec_rwnd) and send_Notfinished):
				if (chunk_index + in_flight < chunk_number):
					to_send = file.read(1500*chunk_index)
					msg = str(chunk_index + in_flight) + ";" + to_send.decode()
					s.sendto(msg.encode(), (client_ip, FILE_RECEIVE_PORT))
					in_flight += 1
					total_sent_packet += 1
				if (total_sent_packet == chunk_number):
					send_Notfinished = False
					socket.sendto(b'EOF', (client_ip, client_port))
					break
				# Wait for ACK
				try:
					data, addr = s.recvfrom(4096)
					if data.decode().split(";")[0] == "ACK":
						chunk_index = int(data.decode().split(";")[1]) + 1
						rec_rwnd = int(data.decode().split(";")[2])
						in_flight -= 1
					if data.decode().split(";")[0] == "ACKEOF":
						return
				except socket.timeout as e:
					print("Try to send again")
					in_flight = 0
				except socket.error as e:
					print("Socket error: {}".format(e))
					file.close()
			return True