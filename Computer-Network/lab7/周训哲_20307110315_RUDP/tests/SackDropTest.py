import random

from tests.BasicTest import BasicTest


class SackDropTest(BasicTest):
    def __init__(self, forwarder, input_file):
        super(SackDropTest, self).__init__(forwarder, input_file, sackMode=True)

    def handle_packet(self):
        for p in self.forwarder.in_queue:
            if random.choice([True, False]):
                self.forwarder.out_queue.append(p)

        # empty out the in_queue
        self.forwarder.in_queue = []
