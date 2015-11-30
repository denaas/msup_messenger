from twisted.internet.protocol import Factory
from twisted.protocols.basic import LineReceiver
from twisted.internet import reactor
from twisted.internet.protocol import Protocol
import sys
import StringIO

class Echo(Protocol):
	
    def connectionMade(self):
	print "new connection"
	    
    """def get_message(self):
	    message = self.recv(sys.getsizeof(message)) // sizeof(message)
	    print "########## " + sys.getsizeof(message)
	    return message
    """
    
    def dataReceived(self, data):
	print 'data received'
	#print str(data) , sys.getsizeof(data)
	print str(data) , len(data)
	
	self.transport.write(data)
	data = 0
	return data
		

class Handle(Protocol):	
    def handle_1(self, data):
	print "good"
	login = self.get_message()
	password = self.get_message()
	data_base.append(login, password)
	print login
	self.transport.write("done")
	    
    def handle_2(self, data):
	login = self.get_message()
	password = self.get_message()
	if((login, password) in data_base):
	    self.transport.write("OK 1")
	else:
	    self.transport.write("ERROR")
		

"""
def dataReceived(self, data):
        print 'data received'
        out = StringIO.StringIO(data)
        print cPickle.load(out)
        print cPickle.load(out)
        out.close()
        print data
"""

class Chat(LineReceiver):

    def __init__(self, users):
        self.users = users
        self.name = None
        self.state = "GETNAME"
   
    
    name = "" 
    def getName(self): 
        if self.name!="": 
            return self.name 
        return self.transport.getPeer().host 

	def connectionMade(self):
		print "new connection from " + self.getName() 
		self.sendLine("What's your name?")


	def get_message(self):
		lineReceived("GETNAME")

    def connectionLost(self, reason):
        if self.name in self.users:
            del self.users[self.name]

    def lineReceived(self, line):
		print "dkalk"
		if self.state == "GETNAME":
			self.handle_GETNAME(line)
		else:
			self.handle_CHAT(line)
	
	
	
    def handle_GETNAME(self, name):
		print "WOW , it works!"
		self.sendLine("hi, Kan")
		if name in self.users:
			self.sendLine("Name taken, please choose another.")
			return
		self.sendLine("Welcome, %s!" % (name,))
		self.name = name
		self.users[name] = self
		self.state = "CHAT"

    def handle_CHAT(self, message):
        message = "<%s> %s" % (self.name, message)
        for name, protocol in self.users.iteritems():
            if protocol != self:
				self.sendLine("hi")
				protocol.sendLine(message)
    
    def sendLine(self, line): 
        self.transport.write(line + "\r\n") 


class ChatFactory(Factory):
    
    def __init__(self):
        self.users = {} # maps user names to Chat instances

    def buildProtocol(self, addr):
        return Chat(self.users)


def main():
    print "Starting Server"
    data_base = []
    
    f = Factory()
    f.protocol = Echo
    
   # Echo.dataReceived()
    """if(int(data1) == 1):
	    self.handle_1(data_)
	    
    if(data1[:1] == "2"):
	    self.handle_2(data_)
    """
    reactor.listenTCP(8000, f)
    reactor.run()

if __name__ == '__main__':
    main()
