from twisted.internet import reactor
from twisted.internet.protocol import ServerFactory 
from twisted.protocols.basic import LineOnlyReceiver 

from itertools import compress

#import database

users = { 'test': '1', 'protest': '1'}
port = 8001

class ChatProtocol(LineOnlyReceiver): 

	name = "" 
	state = 0 # 1 - auth
	receiver = None

	def getName(self): 
		if self.name != "" : 
			return self.name 
		return self.transport.getPeer().host 

	def connectionMade(self): 
		print ("New connection from " + self.getName()) 
		#self.factory.sendMessageToAllClients("/SERTIFICATE")
		self.factory.sendMessageToAllClients(self.getName()+" has joined the party.") 
		self.factory.clientProtocols.append(self)

	def connectionLost(self, reason): 
		print ("Lost connection from "+self.getName()) 
		self.factory.clientProtocols.remove(self) 
		self.factory.sendMessageToAllClients(self.getName()+" has disconnected.") 

	def register(self, name, password):
		if not users.get(name) is None:
			return False
		self.name = name
		users[name] = password
		return True

	def auth(self, name, password):
		if users.get(name) == password:
			self.name = name
			self.state = 1
			return name
		self.state = 0
		return False

	def dataReceived(self, line): 
		print ("<" + self.getName() + "> " + line)
		args =  line.split(' ')
		print(len(line))
		command = args[0]
		if command=="/EXIT": 
			self.transport.loseConnection() 
		elif command=="/REGISTER":
			if len(args) != 3:
				self.sendLine("/REGISTER FAILURE ARGS")
			elif self.register(args[1], args[2]):
				self.sendLine("/REGISTER OK")
			else:
				self.sendLine("/REGISTER FAILURE EXISTS")
		elif command=="/AUTH":
			if len(args) != 3 or self.auth(args[1], args[2]) is False:
				if len(args)!= 3:
					print("!=3")
				else :
					print("not found")
				self.sendLine("/AUTH FAILURE")
			else:
				self.sendLine("/AUTH OK")
				#self.factory.sendMessageToAuthClients(args[1], "Say hello to " + self.name)
		elif command=="/MESSAGE":
			if self.name != "" and self.state != 0 and args[1] != None:
				self.factory.sendMessageToAuthClients(args[1], "<" + self.getName() + "> : " + " ".join(args[2:])) 
			else :
				self.sendLine("error: you are not online. You should authorised")
		elif command=="/GROUP_CHAT":
			pass
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
	
	def findUser(self, name):
		for client in self.clientProtocols:
			if client.getName() == name:
				return client
		return None
	
	def sendMessageToAuthClients(self, name, mesg) :  
		user = self.findUser(name)
		if user != None and user.state != 0:
			user.sendLine(mesg)
		#elif user == None or (user != None and user.state == 0):
			#self.sendLine("user is not online or doesn't exist")
		
			
	def sendToYou(self, us, message):
		user = self.findUser(us)
		if user != None : 
			user.sendLine(message)

def main():
	print ("Starting Server")
	factory = ChatProtocolFactory()
	reactor.listenTCP(port, factory)
	reactor.run()


if __name__ == '__main__':
	main()
