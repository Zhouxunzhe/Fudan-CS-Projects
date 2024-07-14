import random

from tests.BasicTest import BasicTest


class SackDupTest(BasicTest):
    def __init__(self, forwarder, input_file):
        super(SackDupTest, self).__init__(forwarder, input_file, sackMode=True)

    def handle_packet(self):
        for p in self.forwarder.in_queue:
            if random.choice([True, False]):
                for i in range(2):
                    self.forwarder.out_queue.append(p)
            else:
                self.forwarder.out_queue.append(p)
            

        # empty out the in_queue
        self.forwarder.in_queue = []
