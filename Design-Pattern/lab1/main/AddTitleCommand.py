from lab1.main.Command import Command


class AddTitleCommand(Command):
    def __init__(self, manager, title, parent_title="个人收藏"):
        self.manager = manager
        self.title = title
        self.parent_title = parent_title
        self.node = None

    def execute(self):
        self.node = self.manager.add_title(self.title, self.parent_title)
        return self.node

    def undo(self):
        if self.node:
            self.manager.delete_title(self.node.name)