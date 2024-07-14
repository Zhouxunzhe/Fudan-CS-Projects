from abc import ABC, abstractmethod
from lab1.main.Node import Node


class Plugin(ABC):
    @abstractmethod
    def add_info(self, info):
        pass


class PluginManager:
    def __init__(self):
        self.plugins = []

    def register_plugin(self, plugin):
        self.plugins.append(plugin)

    def run_plugins(self, info):
        for plugin in self.plugins:
            result = plugin.add_info(info)
            if result is not None:
                return result


class CountPlugin(Plugin):
    def __init__(self, use_count_plugin=True):
        self.use_count_plugin = use_count_plugin

    def add_info(self, node):
        if isinstance(node, Node) and node.is_bookmark and self.use_count_plugin:
            node.extra_info = f"[{node.read_count}]" if node.is_read else ''
            return f"[{node.read_count}]" if node.is_read else ''


class FilePlugin(Plugin):
    def __init__(self, use_file_plugin=True):
        self.use_file_plugin = use_file_plugin

    def add_info(self, info):
        if isinstance(info, str) and self.use_file_plugin:
            return f"[{info}]"
