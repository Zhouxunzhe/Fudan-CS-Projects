from function import *
import numpy as np
from models.VWAP import calc_vwap


class AutoTrader:
    def __init__(self):
        """Initialise a new instance of the AutoTrader class."""
        self.bids = []
        self.asks = []
        self.bids_hedge = set()
        self.asks_hedge = set()
        self.ask_id = self.ask_price = self.bid_id = self.bid_price = self.position = 0

    def work(self):
        order_book_data = get_order_book_depth()
        self.bids = order_book_data['bids']
        self.asks = order_book_data['asks']
        self.position = get_position_size()
        # print('Bids:', self.bids)
        # print('Asks:', self.asks)
        print('Position:', self.position)

        new_ask_price = round(calc_vwap(self.asks), price_precision)
        new_bid_price = round(calc_vwap(self.bids), price_precision)

        # if(new_ask_price <= new_bid_price):
        #     return

        print("new_ask_price:", new_ask_price, "new_bid_price:", new_bid_price)
        # new_ask_price += MARGIN
        # new_bid_price -= MARGIN

        should_notify_open_long = False
        should_notify_open_short = False

        if np.abs(self.bid_price - new_bid_price) > self.bid_price * cancel_price_diff \
                and self.position < POSITION_LIMIT:
            should_notify_open_long = True

        if np.abs(self.ask_price - new_ask_price) > self.ask_price * cancel_price_diff \
                and self.position > -POSITION_LIMIT:
            should_notify_open_short = True

        # bid
        if new_bid_price > 0 and should_notify_open_long:
            quantity = round(min(MAX_ORDER, POSITION_LIMIT - self.position), quantity_precision)
            # quantity = 100
            new_bid_price = round(new_bid_price, price_precision)

            send_cancel_order(self.bid_id)
            send_cancel_order(self.ask_id)
            self.bid_price = new_bid_price
            self.bid_id = place_limit_order(SYMBOL, 'BUY', 'LONG',
                                            quantity, new_bid_price)

        # ask
        if new_ask_price > 0 and should_notify_open_short:
            quantity = round(min(MAX_ORDER, POSITION_LIMIT + self.position), quantity_precision)
            # quantity = 100
            new_ask_price = round(new_bid_price, 1)
            send_cancel_order(self.bid_id)
            send_cancel_order(self.ask_id)
            self.ask_price = new_ask_price
            self.ask_id = place_limit_order(
                SYMBOL, 'SELL', 'SHORT', quantity, new_ask_price)
