from lab1.main.BookmarkManager import BookmarkManager
from lab1.main.CommandHandler import CommandHandler
from io import StringIO
import unittest
from unittest.mock import patch

tree1 = """└── bookmarks/
    ├── cloud.bmk[close]
    ├── cloud1.bmk[edit]
    ├── cloud2.bmk[close]
    └── test/
        └── test.bmk[close]"""

tree2 = """└── bookmarks/
    ├── cloud.bmk[edit]
    ├── cloud1.bmk[close]
    ├── cloud2.bmk[open]
    └── test/
        └── test.bmk[close]"""


class LsTest(unittest.TestCase):
    def __init__(self, methodName):
        super(LsTest, self).__init__(methodName)
        self.bookmark_manager = BookmarkManager()
        self.command_handler = CommandHandler(self.bookmark_manager)

    def test_ls_tree(self):
        self.command_handler.execute('save')
        self.command_handler.execute('open "../bookmarks/cloud1.bmk"')
        with patch('sys.stdout', new_callable=StringIO) as mock_stdout:
            self.command_handler.execute('ls-tree')
            output = mock_stdout.getvalue().strip()
            self.assertEqual(output, tree1)
            self.command_handler.execute('open "../bookmarks/cloud2.bmk"')
            self.command_handler.execute('open "../bookmarks/cloud.bmk"')
            self.command_handler.execute('close "../bookmarks/cloud1.bmk"')
            self.command_handler.execute('ls-tree')
            output = mock_stdout.getvalue().strip()
            self.assertEqual(output, tree1 + '\n' + tree2)


if __name__ == "__main__":
    unittest.main()
