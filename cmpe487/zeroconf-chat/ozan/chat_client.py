import asyncio

from config import DISCOVERY_PORT, HOST, MESSAGE_PORT
from handlers.discovery_handler import handle_discovery_request
from handlers.message_handler import handle_message
from controller import interact

loop = asyncio.get_event_loop()

discovery_job = asyncio.start_server(handle_discovery_request, HOST, DISCOVERY_PORT, loop=loop)
discovery_server = loop.run_until_complete(discovery_job)

message_job = asyncio.start_server(handle_message, HOST, MESSAGE_PORT, loop=loop)
message_server = loop.run_until_complete(message_job)

loop.run_until_complete(interact(loop))

try:
    loop.run_forever()
except KeyboardInterrupt:
    pass

discovery_server.close()
message_server.close()
loop.run_until_complete(discovery_server.wait_closed())
loop.run_until_complete(message_server.wait_closed())

loop.close()
