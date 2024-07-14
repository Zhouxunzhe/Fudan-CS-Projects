from lab1.main.Command import Command


class AddBookmarkCommand(Command):
    def __init__(self, manager, title, url, parent_title="个人收藏"):
        self.manager = manager
        self.title = title
        self.url = url
        self.parent_title = parent_title
        self.node = None

    def execute(self):
        self.node = self.manager.add_bookmark(self.title, self.url, self.parent_title)
        return self.node

    def undo(self):
        if self.node:
            self.manager.delete_bookmark(self.node.name)
