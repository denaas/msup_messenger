from twisted.internet import reactor
from twisted.internet.protocol import ServerFactory 
from twisted.protocols.basic import LineOnlyReceiver 

users = {}

class ChatProtocol(LineOnlyReceiver): 

    name = "" 

    def getName(self): 
        if self.name!="": 
            return self.name 
        return self.transport.getPeer().host 

    def connectionMade(self): 
        print "New connection from "+self.getName() 
        self.sendLine("Welcome to my my chat server.") 
        self.sendLine("Send '/NAME [new name]' to change your name.") 
        self.sendLine("Send '/EXIT' to quit.") 
        self.factory.sendMessageToAllClients(self.getName()+" has joined the party.") 
        self.factory.clientProtocols.append(self)

    def connectionLost(self, reason): 
        print "Lost connection from "+self.getName() 
        self.factory.clientProtocols.remove(self) 
        self.factory.sendMessageToAllClients(self.getName()+" has disconnected.") 

    def register(self, name, password):
        if not users.get(name) is None:
            return False
        users[name] = password
        return True

    def auth(self, name, password):
        if users.get(name) == password:
            return name
        return False

    def lineReceived(self, line): 
        print self.getName()+" said "+line 
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
                self.name = auth
                self.factory.sendMessageToAllClients("Say hello to " + self.name)
        elif self.name != "": 
            self.factory.sendMessageToAllClients(self.getName()+" says: "+line) 
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

print "Starting Server"
factory = ChatProtocolFactory()
reactor.listenTCP(12345, factory)
reactor.run()
