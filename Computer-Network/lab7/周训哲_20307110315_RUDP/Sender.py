import sys
import getopt
import Checksum
import BasicSender

'''
This is a skeleton sender class. Create a fantastic transport protocol here.
'''

maxDataSize = 1000


class Sender(BasicSender.BasicSender):

    def __init__(self, dest, port, filename, debug=False, sackMode=False):
        super(Sender, self).__init__(dest, port, filename, debug)
        self.msg_win = {}
        (self.dup_seq, self.dup_counter) = (-1, -1)
        self.flag = False
        if filename == None:
            self.infile = sys.stdin
        else:
            self.infile = open(filename,"rb")

    # Main sending loop.
    def start(self):
        msg = self.infile.read(maxDataSize)
        self.msg_win[0] = msg
        self.send_pkt(0, msg)
        seq = 1
        if sackMode:
            while True:
                response = self.receive(0.5)
                if response is None:
                    self.handle_timeout()
                    continue
                response = response.decode()
                sum_ack, sacks = self.handle_response(response)
                if debug:
                    print(self.flag, "sum_ack =", sum_ack, "sack =", sacks)
                if sum_ack is not None:
                    if sum_ack < seq:
                        for i in sacks:
                            self.send_pkt(i, self.msg_win[i])
                    elif sum_ack == seq:
                        if self.flag:
                            break
                        self.msg_win.clear()
                        self.handle_new_ack(seq)
                        seq += 1
                    elif sum_ack > seq:
                        print("PANIC: Seq Too Large")
                        return
                else:
                    if self.handle_new_ack(seq):
                        seq += 1
        else:
            while True:
                response = self.receive(0.5)
                if response is None:
                    self.handle_timeout()
                    continue
                response = response.decode()
                ack_seq = self.handle_response(response)
                if debug:
                    print(self.flag, "seq =", seq, "ack =", ack_seq)
                if ack_seq is not None:
                    if ack_seq < seq:
                        if self.check_dup_ack(ack_seq):
                            self.handle_dup_ack(ack_seq)
                    elif ack_seq == seq:
                        if self.flag:
                            break
                        self.msg_win.clear()
                        self.handle_new_ack(seq)
                        seq += 1
                    elif ack_seq > seq:
                        print("PANIC: Seq Too Large")
                        return
                else:
                    if self.handle_new_ack(seq):
                        seq += 1

        self.infile.close()

    def send_pkt(self, seq, msg):
        msg_type = 'data'
        if seq == 0:
            msg_type = 'start'
        elif not msg:
            msg_type = 'end'
        packet = self.make_packet(msg_type, seq, msg)
        # print(msg)
        self.send(packet)
        if self.debug:
            print("sent: %s|%d|%s|%s" % (msg_type, seq, msg[:5], packet.split('|')[-1]))

    def split_ack(self, message):
        pieces = message.split('|')
        msg_type, seqno = pieces[0:2]  # first two elements always treated as msg type and seqno
        checksum = pieces[-1]  # last is always treated as checksum
        # data = '|'.join(pieces[2:-1]) # everything in between is considered data
        return msg_type, seqno, checksum

    def handle_response(self, response):
        if debug:
            print("Sender: received", response)
        msg_type, seqno, checksum = self.split_ack(response)
        if not Checksum.validate_checksum(response):
            print("Checksum Error")
            return None
        if msg_type != 'ack' and msg_type != 'sack':
            print("PANIC: Response Format Error")
            return None
        if sackMode:
            sum_ack, sacks = seqno.split(';')
            sacks = sacks.split(',')
            sack = []
            for i in sacks:
                if i != '':
                    sack.append(int(i))
            return int(sum_ack), sack
        else:
            return int(seqno)

    def handle_timeout(self):
        for key in self.msg_win.keys():
            self.send_pkt(key, self.msg_win[key])

    def check_dup_ack(self, seq):
        if seq == self.dup_seq:
            self.dup_counter += 1
        else:
            self.dup_seq = seq
            self.dup_counter = 1
        if self.dup_counter >= 3:
            return True
        return False

    def handle_new_ack(self, seq):
        if len(self.msg_win) >= 5:
            return False
        msg = self.infile.read(maxDataSize)
        if not msg:
            self.flag = True
        self.msg_win[seq] = msg
        self.send_pkt(seq, msg)
        return True

    def handle_dup_ack(self, ack_seq):
        self.send_pkt(ack_seq, self.msg_win[ack_seq])

    def log(self, msg):
        if self.debug:
            print(msg)


'''
This will be run if you run this script from the command line. You should not
change any of this; the grader may rely on the behavior here to test your
submission.
'''
if __name__ == "__main__":
    def usage():
        print("RUDP Sender")
        print("-f FILE | --file=FILE The file to transfer; if empty reads from STDIN")
        print("-p PORT | --port=PORT The destination port, defaults to 33122")
        print("-a ADDRESS | --address=ADDRESS The receiver address or hostname, defaults to localhost")
        print("-d | --debug Print debug messages")
        print("-h | --help Print this usage message")
        print("-k | --sack Enable selective acknowledgement mode")


    try:
        opts, args = getopt.getopt(sys.argv[1:],
                                   "f:p:a:dk", ["file=", "port=", "address=", "debug=", "sack="])
    except:
        usage()
        exit()

    port = 33122
    dest = "localhost"
    filename = None
    debug = False
    sackMode = False

    for o, a in opts:
        if o in ("-f", "--file="):
            filename = a
        elif o in ("-p", "--port="):
            port = int(a)
        elif o in ("-a", "--address="):
            dest = a
        elif o in ("-d", "--debug="):
            debug = True
        elif o in ("-k", "--sack="):
            sackMode = True

    s = Sender(dest, port, filename, debug, sackMode)
    try:
        s.start()
    except (KeyboardInterrupt, SystemExit):
        exit()
