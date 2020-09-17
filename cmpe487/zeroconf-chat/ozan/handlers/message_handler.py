import asyncio
import hashlib

from common import ip_to_name, name_to_cipher
from config import HOME, MAX_MESSAGE_SIZE


@asyncio.coroutine
def handle_message(reader, writer):
    try:
        data = yield from reader.read(MAX_MESSAGE_SIZE)
        decoded = data.decode()
        src_ip, cipher = decoded.split(';')[:2]
        msg = decoded[len(src_ip) + len(cipher) + 2:-2]
        addr = writer.get_extra_info('peername')[0]
        if addr == HOME or src_ip == addr:
            if src_ip not in ip_to_name:
                print("Kimsin sen!")
                raise Exception
            name = ip_to_name[src_ip]
            if name not in name_to_cipher or name_to_cipher[name] == cipher:
                print("Message from {}: {}".format(name, msg))
            else:
                print("Message dropped!")
                raise Exception
            name_to_cipher[name] = hashlib.md5(cipher.encode()).hexdigest()
        else:
            print("Invalid request dropped {}".format(decoded))
    except Exception as e:
        print(str(e))
        print("Error parsing request")
    writer.close()
