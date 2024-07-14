from abc import ABC, abstractmethod


class RequestOperation(ABC):
    @abstractmethod
    def add_request(self, data):
        pass

    @abstractmethod
    def check_request(self, data):
        pass

    @abstractmethod
    def show_requests(self, data):
        pass
