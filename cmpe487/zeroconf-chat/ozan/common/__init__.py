import asyncio
import socket

from config import SEND_TIMEOUT

name_to_ip = dict()
ip_to_name = dict()
name_to_cipher = dict()


def get_ip():
    # stackoverflow \m/
    return (([ip for ip in socket.gethostbyname_ex(socket.gethostname())[2] if not ip.startswith("127.")] or [
        [(s.connect(("8.8.8.8", 53)), s.getsockname()[0], s.close()) for s in
         [socket.socket(socket.AF_INET, socket.SOCK_DGRAM)]][0][1]]) + ["no IP found"])[0]


def create_discovery_header(req_type, src_ip, src_name, dst_ip, dst_name):
    return "{};{};{};{};{}".format(req_type, src_ip, src_name, dst_ip, dst_name).encode()


@asyncio.coroutine
def send(message, target, port, loop):
    try:
        fut = asyncio.open_connection(target, port, loop=loop)
        _, writer = yield from asyncio.wait_for(fut, timeout=SEND_TIMEOUT)
        print('Sent: {}'.format(message))
        writer.write(message)
        writer.close()
    except Exception as e:
        print("Failed to send!" + str(e))
