import sys
import socket
import time

def main(args):
	server_address = ('localhost', int(args[0]))

	for i in range(3000):
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

		sock.connect(server_address)

		time.sleep(0.001)

		sock.close()
      
if __name__ == '__main__':
    main(sys.argv[1:])