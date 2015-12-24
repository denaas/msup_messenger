from twisted.internet import reactor
from twisted.internet.protocol import ServerFactory 
from twisted.protocols.basic import LineOnlyReceiver 

from itertools import compress

#import database

users = {}
port = 8000

class ChatProtocol(LineOnlyReceiver): 

	name = "" 
	state = 0 # 1 - auth

	def getName(self): 
		if self.name != "" : 
			return self.name 
		return self.transport.getPeer().host 

	def connectionMade(self): 
		print ("New connection from " + self.getName()) 
		self.sendLine("Send '/NAME [new name]' to change your name.") 
		self.sendLine("Send '/EXIT' to quit.") 
		self.factory.sendMessageToAllClients(self.getName()+" has joined the party.") 
		self.factory.clientProtocols.append(self)

	def connectionLost(self, reason): 
		print ("Lost connection from "+self.getName()) 
		#self.factory.clientProtocols.remove(self) 
		#self.factory.clientAuthProt.remove(self) 
		self.factory.sendMessageToAllClients(self.getName()+" has disconnected.") 

	def register(self, name, password):
		if not users.get(name) is None:
			return False
		self.name = name
		users[name] = password
		return True

	def auth(self, name, password):
		if users.get(name) == password:
			self.state = 1
			idx = map(lambda x: x.getName(), self.factory.clientProtocols).index(name)
			return name
		return False

	def lineReceived(self, line): 
		print (self.getName()+" said "+line) 
		if line[:5]=="/NAME": 
			oldName = self.getName() 
			self.name = line[5:].strip() 
			self.factory.sendMessageToAllClients(oldName+" changed name to "+self.getName()) 
		elif line=="/EXIT": 
			self.transport.loseConnection() 
		elif line[:len("/REGISTER")]=="/REGISTER":
			args = line.split(' ')[1:]
			if len(args) < 2:
				self.sendLine("/REGISTER FAILURE ARGS")
			elif self.register(args[0], args[1]):
				self.sendLine("/REGISTER OK")
			else:
				self.sendLine("/REGISTER FAILURE EXISTS")
		elif line[:len("/AUTH")]=="/AUTH":
			args = line.split(' ')[1:] + ['', '']
			auth = self.auth(args[0], args[1])
			if auth is False:
				self.sendLine("/AUTH FAILURE")
			else:
				self.sendLine("/AUTH OK")
				self.factory.sendMessageToAuthClients("Say hello to " + self.name)
		elif self.name != "" and self.state != 0: 
			self.factory.sendMessageToAuthClients(self.getName()+" says: "+line) 
		else:
			self.sendLine("Unknown command")
			

	def sendLine(self, line): 
		self.transport.write(line+"\r\n") 

class ChatProtocolFactory(ServerFactory): 

	protocol = ChatProtocol 

	def __init__(self): 
		self.clientProtocols = [] 

	def sendMessageToAllClients(self, mesg): 
		for client in self.clientProtocols:
			client.sendLine(mesg) 
	
	def sendMessageToAuthClients(self, mesg) : 
		for client in [p for p in self.clientProtocols if p.auth != 0]: 
			client.sendLine(mesg)

def main():
	print ("Starting Server")
	factory = ChatProtocolFactory()
	reactor.listenTCP(port, factory)
	reactor.run()


if __name__ == '__main__':
	main()
