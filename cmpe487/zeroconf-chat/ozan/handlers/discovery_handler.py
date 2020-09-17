import asyncio

from common import create_discovery_header, get_ip, ip_to_name, name_to_ip, send
from config import DISCOVERY_PORT, HOME, MAX_MESSAGE_SIZE, TYPE_ANSWER, TYPE_REQUEST, USERNAME


@asyncio.coroutine
def handle_discovery_request(reader, writer):
    try:
        data = yield from reader.read(MAX_MESSAGE_SIZE)
        decoded = data.decode()
        req_type, src_ip, src_name = decoded.split(';')[:3]
        addr = writer.get_extra_info('peername')[0]
        if (addr == HOME or src_ip == addr) and (req_type == TYPE_ANSWER or req_type == TYPE_REQUEST):
            name_to_ip[src_name] = src_ip
            ip_to_name[src_ip] = src_name
            if req_type == TYPE_REQUEST:
                print("Received discovery request from {} {}".format(src_ip, src_name))

                yield from send(create_discovery_header(TYPE_ANSWER, get_ip(),USERNAME, src_ip, src_name),
                                addr, DISCOVERY_PORT, asyncio.get_event_loop())
        else:
            print("Invalid request dropped {}".format(decoded))
    except:
        print("Error parsing request")
    writer.close()
