import random

from tests.BasicTest import BasicTest


class SackOutOfOrderTest(BasicTest):
    def __init__(self, forwarder, input_file):
        super(SackOutOfOrderTest, self).__init__(forwarder, input_file, sackMode=True)

    def handle_packet(self):
        if random.choice([True, False]):
            random.shuffle(self.forwarder.in_queue)
            for p in self.forwarder.in_queue:
                self.forwarder.out_queue.append(p)
        else:
            for p in self.forwarder.in_queue:
                self.forwarder.out_queue.append(p)
                self.forwarder.out_queue.append(p)
        # empty out the in_queue
        self.forwarder.in_queue = []
