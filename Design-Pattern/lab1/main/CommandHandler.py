import re
import os
from lab1.main.BookmarkManager import BookmarkManager
from lab1.main.CommandHistory import CommandHistory
from lab1.main.AddTitleCommand import AddTitleCommand
from lab1.main.AddBookmarkCommand import AddBookmarkCommand
from lab1.main.DeleteCommand import DeleteCommand


class CommandHandler:
    def __init__(self, manager=None):
        self.manager = manager if manager else BookmarkManager()
        self.history = CommandHistory()

    def execute(self, command_str):
        parts = self._split_string(command_str)
        action = parts[0]
        if action == "add-title":
            title = parts[1].strip('"')
            parent_title = parts[2] if "at" in command_str else "个人收藏"
            command = AddTitleCommand(self.manager, title, parent_title)
            self.manager.is_saved = False
        elif action == "add-bookmark":
            title = parts[1]
            url = parts[2]
            parent_title = parts[3] if "at" in command_str else "个人收藏"
            command = AddBookmarkCommand(self.manager, title, url, parent_title)
            self.manager.is_saved = False
        elif action == "delete-title" or action == "delete-bookmark":
            title = parts[1]
            command = DeleteCommand(self.manager, title)
            self.manager.is_saved = False
        elif action == "undo":
            self.history.undo()
            self.manager.is_saved = False
            return
        elif action == "redo":
            self.history.redo()
            self.manager.is_saved = False
            return
        elif action == "open" or action == "bookmark" or action == "edit":
            file_path = parts[1]
            self.manager.open_file(file_path)
            return
        elif action == "close":
            if len(parts) > 1:
                file_path = parts[1]
                self.manager.close_file(file_path)
            else:
                self.manager.close_file()
            return
        elif action == "save":
            file_path = parts[1] if len(parts) > 1 else None
            self.manager.save_bookmarks(file_path)
            return
        elif action == "show-tree":
            self.manager.show_tree()
            return
        elif action == "ls-tree":
            workspace_dir = os.path.dirname(self.manager.current_file)
            self.manager.list_directory_tree(workspace_dir)
            return
        elif action == "read-bookmark":
            bookmark_name = parts[1]
            self.manager.read_bookmark(bookmark_name)
            self.manager.is_saved = False
            return
        else:
            raise ValueError(f"Invalid command: {command_str}")
        self.history.execute(command)

    def _split_string(self, command):
        new_command = re.sub(r'@| at ', '', command)
        parts = re.findall(r'"[^"]*"|\S+', new_command)
        parts = [part.strip('"') for part in parts]
        return parts
