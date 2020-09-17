import asyncio
import hashlib
import random
import sys

from common import create_discovery_header, get_ip, name_to_cipher, name_to_ip, send
from config import DISCOVERY_PORT, MESSAGE_PORT, USERNAME


@asyncio.coroutine
def interact(loop):
    reader = asyncio.StreamReader()
    reader_protocol = asyncio.StreamReaderProtocol(reader)

    yield from loop.connect_read_pipe(lambda: reader_protocol, sys.stdin)
    while True:
        try:
            print('Choose action:')
            print('    [0] Discover users')
            print('    [1] Send message')
            action = yield from reader.readline()
            if action == b'0\n':
                my_ip = get_ip()
                first_octets = '.'.join(my_ip.split('.')[:3]) + '.'
                tasks = []
                for i in range(2, 255):
                    target = first_octets + str(i)
                    if target != my_ip:
                        tasks.append(loop.create_task(send(create_discovery_header(0, my_ip, USERNAME, target, ''),
                                                           target, DISCOVERY_PORT, loop)))
                yield from asyncio.wait(tasks)

            else:
                i = 0
                names = []
                print('User list:')
                for name in name_to_ip:
                    print("    [{}] {}".format(i, name))
                    names.append(name)
                    i += 1
                print('User index')
                index = yield from reader.readline()
                try:
                    user = names[int(index.decode())]
                except:
                    print("Invalid index!")
                    continue
                yield
                print('Your msg:')
                msg = yield from reader.readline()
                cipher = name_to_cipher[user] if user in name_to_cipher else str(random.randint(1, 1012342))
                message = '{};{};{};'.format(get_ip(), cipher, msg.decode())
                yield from send(message.encode(), name_to_ip[user], MESSAGE_PORT, loop)
                name_to_cipher[user] = hashlib.md5(cipher.encode()).hexdigest()

        except Exception as e:
            print(str(e))
            pass
