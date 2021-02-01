  
import socket
import os
from threading import Thread
import myparser as parser
import magmarok

class Proxy2Server(Thread):

    def __init__(self, host, port):
        super(Proxy2Server, self).__init__()
        self.game = None # game client socket not known yet
        self.port = port
        self.host = host
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.connect((host, port))

    # run in thread
    def run(self):
        while True:
            data = self.server.recv(4096)
            if data:
                #print "[{}] <- {}".format(self.port, data[:100].encode('hex'))
                try:
                    parser.parse(data, self.port, 'server')
                    if len(parser.client_stash) > 0 :
                        OAO = parser.client_stash.pop()
                        self.game.sendall( OAO )
                except Exception as e:
                    print 'server[{}]'.format(self.port), e
                # forward to client
                self.game.sendall(data)

class Game2Proxy(Thread):

    def __init__(self, host, port):
        super(Game2Proxy, self).__init__()
        self.server = None # real server socket not known yet
        self.port = port
        self.host = host
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((host, port))
        sock.listen(1)
        # waiting for a connection
        self.game, addr = sock.accept()

    def run(self):
        while True:
            data = self.game.recv(4096)
            if data:
                #print "[{}] -> {}".format(self.port, data[:100].encode('hex'))
                try:
                    # reload( parser )
                    parser.parse(data, self.port, 'client')
                    if len(parser.server_stash) > 0 :
                        OAO = parser.server_stash.pop()
                        self.server.sendall( OAO )
                except Exception as e:
                    print 'client[{}]'.format(self.port), e
                # forward to server
                self.server.sendall(data)

class Proxy(Thread):

    def __init__(self, from_host, to_host, port):
        super(Proxy, self).__init__()
        self.from_host = from_host
        self.to_host = to_host
        self.port = port
        self.running = False

    def run(self):
        while True:
            print "[proxy({})] setting up".format(self.port)
            self.g2p = Game2Proxy(self.from_host, self.port) # waiting for a client
            self.p2s = Proxy2Server(self.to_host, self.port)
            print "[proxy({})] connection established".format(self.port)
            self.g2p.server = self.p2s.server
            self.p2s.game = self.g2p.game
            self.running = True

            self.g2p.start()
            self.p2s.start()


master_server = Proxy('hackyour.pw', '140.122.185.173', 3333)
master_server.start()

game_servers = []
for port in range(3000, 3006):
    _game_server = Proxy('hackyour.pw', '140.122.185.173', port)
    _game_server.start()
    game_servers.append(_game_server)

def attack():
    reload( magmarok )
    QQ = magmarok.action()
    for server in game_servers:
        if server.running:
            server.p2s.server.sendall( QQ )
    return

def snowball():
    reload( magmarok )
    QQ = magmarok.snowball()
    for server in game_servers:
        if server.running:
            server.p2s.server.sendall( QQ )
    return

def fireball():
    reload( magmarok )
    QQ = magmarok.fireball()
    for server in game_servers:
        if server.running:
            server.p2s.server.sendall( QQ )
    return

while True:
    try:
        cmd = raw_input('$ ')
        if cmd[:4] == 'quit':
            os._exit(0)
        if cmd[:2] == 's ':
            for server in game_servers:
                if server.running:
                    server.p2s.server.sendall( cmd[2:].decode( 'hex' ) )
        if cmd[:6] == 'attack':
            attack()
        if cmd[:8] == 'snowball':
            snowball()
        if cmd[:8] == 'fireball':
            fireball()

    except Exception as e:
        print e