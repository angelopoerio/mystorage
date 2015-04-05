import socket

__author__ = "Angelo Poerio <angelo.poerio@gmail.com>"
__all__ = ['MyStorage']

class MyWrapperConn(object):
	def __init__(self, host, port=7777):
		self.host = host
		self.port = port
		self.s = socket.socket()

	def connect(self):
		self.s.connect((self.host, self.port))

	def get(self, key, key_size=1024):
		self.s.send("GET {0}\n".format(key), key_size)
		return self.s.recv(key_size)

	def put(self, key, value):
		return self.s.send("PUT {0} {1}\n".format(key, value))

	def list(self, key_size=1024):
		entries = ''
		entry = ''
		self.s.send("LIST\n")
		while 'END_LIST\n' not in entry:
			entry = self.s.recv(key_size)
			entries += entry
		return [{e.split(":")[0]: e.split(":")[1]} for e in entries.split("\n")[:-2]]

	def quit(self):
		self.s.send("QUIT\n")
		self.s.close()


class MyStorage(object):
	def __init__(self, host, port=7777):
		self.__wrap = MyWrapperConn(host, port)

	def get_connection(self):
		return self.__wrap

	def __enter__(self):
		self.__wrap.connect()
		return self.__wrap

	def __exit__(self, type, value, traceback):
		self.__wrap.quit()

if __name__ == "__main__":
	print "import this module"
