from lab1.main.Node import Node
import os
from lab1.main.Plugin import PluginManager, CountPlugin, FilePlugin


class BookmarkManager:
    def __init__(self, file_path=None, opened_files=None):
        self.root = Node("个人收藏")
        self.nodes = {"个人收藏": self.root}
        if file_path is not None:
            file_path = file_path.replace('\\', '/')
        self.file_path = file_path
        if file_path and os.path.exists(file_path):
            self.load_bookmarks(file_path)
        if opened_files is not None:
            self.opened_files = opened_files
        else:
            self.opened_files = set()
        self.current_file = None
        self.plugin_manager = PluginManager()
        self.plugin_manager.register_plugin(CountPlugin(True))
        self.plugin_manager.register_plugin(FilePlugin(True))
        self.is_saved = True

    def add_title(self, title, parent_title="个人收藏"):
        if parent_title in self.nodes:
            parent_node = self.nodes[parent_title]
            new_node = Node(title, parent=parent_node)
            parent_node.add_child(new_node)
            self.nodes[title] = new_node
            return new_node
        else:
            raise ValueError(f"Parent title '{parent_title}' not found.")

    def add_bookmark(self, title, url, parent_title="个人收藏"):
        if parent_title in self.nodes:
            parent_node = self.nodes[parent_title]
            new_node = Node(title, parent=parent_node, is_bookmark=True, url=url)
            parent_node.add_child(new_node)
            self.nodes[title] = new_node
            return new_node
        else:
            raise ValueError(f"Parent title '{parent_title}' not found.")

    def delete_title(self, title):
        if title in self.nodes:
            node_to_delete = self.nodes[title]
            self._delete_node(node_to_delete)
            del self.nodes[title]
        else:
            raise ValueError(f"Title '{title}' not found.")

    def delete_bookmark(self, title):
        if title in self.nodes:
            node_to_delete = self.nodes[title]
            if node_to_delete.is_bookmark:
                self._delete_node(node_to_delete)
                del self.nodes[title]
            else:
                raise ValueError(f"'{title}' is not a bookmark.")
        else:
            raise ValueError(f"Bookmark '{title}' not found.")

    def _delete_node(self, node):
        for child in list(node.children):
            self._delete_node(child)
            if child.name in self.nodes:
                del self.nodes[child.name]
        node.parent.children.remove(node)

    def load_bookmarks(self, file_path):
        with open(file_path, 'r', encoding='utf-8') as file:
            current_node = self.root
            for line in file:
                line = line.strip()
                if line.startswith('#'):
                    # Determine the level of the heading
                    level = line.count('#')
                    title = line.strip('# ').strip()
                    # Navigate up to the correct parent level
                    while level <= self._get_level(current_node):
                        current_node = current_node.parent
                    # Add new node
                    new_node = Node(title, parent=current_node)
                    current_node.add_child(new_node)
                    current_node = new_node
                    self.nodes[title] = current_node
                elif line.startswith('['):
                    # Parse bookmark
                    title_end = line.index(']')
                    title = line[1:title_end]
                    url = line[title_end + 2:-1]
                    new_node = Node(title, parent=current_node, is_bookmark=True, url=url)
                    current_node.add_child(new_node)
                    self.nodes[title] = new_node

    def save_bookmarks(self, file_path=None):
        if file_path is not None:
            with open(file_path, 'w', encoding='utf-8') as file:
                file.truncate(0)
                self._write_node(self.root, file, 0)
        self.is_saved = True

    def _write_node(self, node, file, depth):
        if node != self.root:
            if node.is_bookmark:
                file.write(f'[{node.name}]({node.url})\n')
            else:
                file.write(f'{"#" * depth} {node.name}\n')
        for child in node.children:
            self._write_node(child, file, depth + 1)

    def _get_level(self, node):
        level = 0
        while node.parent:
            node = node.parent
            level += 1
        return level

    def show_tree(self, node=None, indent=0, last_nodes=None):
        if node is None:
            node = self.root
        if last_nodes is None:
            last_nodes = []
        if node == self.root:
            print(f"└── {node.name}")
        else:
            prefix = "    "
            for is_last in last_nodes[:-1]:
                prefix += '    ' if is_last else '|   '
            prefix += '└── ' if last_nodes and last_nodes[-1] else '├── '
            read_marker = '*' if node.is_read else ''
            extra_info = getattr(node, 'extra_info', '')
            if node.is_bookmark:
                print(prefix + f"[{read_marker}{node.name}{extra_info}]")
            else:
                print(prefix + read_marker + f"{node.name}")
        child_last_nodes = last_nodes + [False]
        num_children = len(node.children)
        for index, child in enumerate(node.children):
            self.plugin_manager.run_plugins(child)
            child_last_nodes[-1] = index == num_children - 1
            self.show_tree(child, indent + 1, child_last_nodes)

    def read_bookmark(self, bookmark_name):
        if bookmark_name in self.nodes:
            node = self.nodes[bookmark_name]
            if node.is_bookmark:
                node.mark_as_read()
                # print(f"Bookmark '{bookmark_name}' has been marked as read. Total reads: {node.read_count}")
            else:
                raise ValueError(f"'{bookmark_name}' is not a bookmark.")
        else:
            raise ValueError(f"Bookmark '{bookmark_name}' not found.")

    def open_file(self, file_path):
        file_path = file_path.replace('\\', '/')
        if not self.is_saved:
            raise ValueError("Current workspace is not saved. Please save before opening a new file.")

        self.root = Node("个人收藏")  # Reset the root node
        self.nodes = {"个人收藏": self.root}
        if os.path.exists(file_path):
            self.load_bookmarks(file_path)
        else:
            print("File does not exist. Starting with a new blank file.")
        self.current_file = file_path
        self.opened_files.add(file_path)
        self.is_saved = True
        # print(f"Opened file: {file_path}")

    def close_file(self, file_path=None):
        if file_path:
            file_path = file_path.replace('\\', '/')
            if file_path in self.opened_files:
                if file_path == self.current_file:
                    self.current_file = None
                self.opened_files.remove(file_path)
                self.save_bookmarks(file_path)
                # print(f"Closed specified file: {file_path}")
            else:
                raise ValueError(f"File {file_path} was not open.")
        else:
            if self.current_file:
                # print(f"Closing current file: {self.current_file}")
                self.opened_files.remove(self.current_file)
                self.current_file = None
                self.save_bookmarks(file_path)
            else:
                raise ValueError("No file is currently open.")

    def list_directory_tree(self, path, prefix='', is_root=True, depth=0):
        if is_root:
            print(f"└── {os.path.basename(path)}/")
            prefix = "    "
        items = os.listdir(path)
        items.sort()
        last_index = len(items) - 1

        if depth <= 1:
            for index, item in enumerate(items):
                full_path = os.path.join(path, item)
                if full_path is not None:
                    full_path = full_path.replace('\\', '/')
                info = 'close'
                if full_path in self.opened_files:
                    info = 'open'
                if full_path == self.current_file:
                    info = 'edit'
                extra_info = self.plugin_manager.run_plugins(info)
                if os.path.isdir(full_path):
                    if index == last_index:
                        print(f"{prefix}└── {item}/")
                        new_prefix = prefix + "    "
                    else:
                        print(f"{prefix}├── {item}/")
                        new_prefix = prefix + "|   "
                    self.list_directory_tree(full_path, new_prefix, is_root=False, depth=depth+1)
                else:
                    if index == last_index:
                        print(f"{prefix}└── {item}" + extra_info)
                    else:
                        print(f"{prefix}├── {item}" + extra_info)

    def has_title(self, title):
        return self._dfs_search_title(self.root, title)

    def _dfs_search_title(self, node, title):
        if node is None:
            return False
        if node.name == title:
            return True
        for child in node.children:
            if self._dfs_search_title(child, title):
                return True
        return False

    def has_bookmark(self, bookmark_name, parent_title="个人收藏"):
        if parent_title in self.nodes:
            return self._dfs_search_bookmark(self.nodes[parent_title], bookmark_name)
        return False

    def _dfs_search_bookmark(self, node, bookmark_name):
        if node is None:
            return False
        if node.name == bookmark_name and node.is_bookmark:
            return True
        for child in node.children:
            if self._dfs_search_bookmark(child, bookmark_name):
                return True
        return False

    def trees_equal(self, other_manager):
        return self._compare_nodes(self.nodes.get("个人收藏"), other_manager.nodes.get("个人收藏"))

    def _compare_nodes(self, node1, node2):
        if node1 is None and node2 is None:
            return True
        if node1 is None or node2 is None:
            return False
        if node1.name != node2.name or node1.is_bookmark != node2.is_bookmark:
            return False
        if len(node1.children) != len(node2.children):
            return False
        for child1, child2 in zip(node1.children, node2.children):
            if not self._compare_nodes(child1, child2):
                return False
        return True
