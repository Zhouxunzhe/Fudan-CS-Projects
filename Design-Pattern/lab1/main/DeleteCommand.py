from lab1.main.Command import Command


class DeleteCommand(Command):
    def __init__(self, manager, title):
        self.manager = manager
        self.title = title
        self.deleted_node = []
        self.parent_node = []
        self.children = []  # 用于保存删除的节点的所有子节点
        self.cnt = 0  # 用于保存当前节点的undo/redo的次数

    def execute(self):
        self.cnt += 1
        self.deleted_node.append(self.manager.nodes.get(self.title))
        if self.deleted_node[-1]:
            self.parent_node = self.deleted_node[-1].parent
            self._store_children(self.deleted_node[-1])
            self.manager.delete_title(
                self.title) if not self.deleted_node[-1].is_bookmark else self.manager.delete_bookmark(self.title)

    def _store_children(self, node):
        # store children nodes recursively
        for child in node.children:
            self.children.append((self.cnt, child.name, child.is_bookmark, child.url, child.parent.name))
            self._store_children(child)

    def undo(self):
        if self.deleted_node[-1]:
            if self.deleted_node[-1].is_bookmark:
                restored_node = self.manager.add_bookmark(self.deleted_node[-1].name, self.deleted_node[-1].url, self.parent_node.name)
            else:
                restored_node = self.manager.add_title(self.deleted_node[-1].name, self.parent_node.name)
            # restore children nodes recursively
            self._restore_children(restored_node, self.children)
            self.children = [child for child in self.children if child[0] != self.cnt]
            self.deleted_node.pop()

    def _restore_children(self, parent_node, children_info):
        for _, name, is_bookmark, url, parent_name in children_info:
            if parent_node.name == parent_name:
                if is_bookmark:
                    new_node = self.manager.add_bookmark(name, url, parent_name)
                else:
                    new_node = self.manager.add_title(name, parent_name)
                self._restore_children(new_node, children_info)
