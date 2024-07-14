from lab1.main.BookmarkManager import BookmarkManager
from lab1.main.CommandHandler import CommandHandler
from io import StringIO
import unittest
from unittest.mock import patch

init_tree = """└── 个人收藏
    ├── 课程
    |   └── [elearning]
    ├── 参考资料
    |   ├── [Markdown Guide]
    |   ├── 函数式
    |   |   └── [JFP]
    |   └── 面向对象
    └── 待阅读
        └── [Category Theory]"""

once_tree = """└── 个人收藏
    ├── 课程
    |   └── [*elearning[1]]
    ├── 参考资料
    |   ├── [Markdown Guide]
    |   ├── 函数式
    |   |   └── [JFP]
    |   └── 面向对象
    └── 待阅读
        └── [Category Theory]"""

twice_tree = """└── 个人收藏
    ├── 课程
    |   └── [*elearning[2]]
    ├── 参考资料
    |   ├── [Markdown Guide]
    |   ├── 函数式
    |   |   └── [JFP]
    |   └── 面向对象
    └── 待阅读
        └── [Category Theory]"""


class ReadTest(unittest.TestCase):
    def __init__(self, methodName):
        super(ReadTest, self).__init__(methodName)
        self.bookmark_manager = BookmarkManager()
        self.command_handler = CommandHandler(self.bookmark_manager)
        self.command_handler.execute('add-title "课程"')
        self.command_handler.execute('add-title "参考资料"')
        self.command_handler.execute('add-title "函数式" at "参考资料"')
        self.command_handler.execute('add-title "面向对象" at "参考资料"')
        self.command_handler.execute('add-title "待阅读"')
        self.command_handler.execute('add-bookmark "elearning"@"https://elearning.fudan.edu.cn/courses" at "课程"')
        self.command_handler.execute('add-bookmark "Markdown Guide"@"https://www.markdownguide.org" at "参考资料"')
        self.command_handler.execute(
            'add-bookmark "JFP"@"https://www.cambridge.org/core/journals/journal-of-functional-programming" at "函数式"')
        self.command_handler.execute(
            'add-bookmark "Category Theory"@"http://www.appliedcategorytheory.org/what-is-applied-category-theory/" at "待阅读"')

    def test_show_tree(self):
        with patch('sys.stdout', new_callable=StringIO) as mock_stdout:
            self.command_handler.execute('show-tree')
            output1 = mock_stdout.getvalue().strip()
            self.assertEqual(output1, init_tree)
            self.command_handler.execute('read-bookmark "elearning"')
            self.command_handler.execute('show-tree')
            output2 = mock_stdout.getvalue().strip()
            self.assertEqual(output2, init_tree + '\n' + once_tree)
            self.command_handler.execute('read-bookmark "elearning"')
            self.command_handler.execute('show-tree')
            output3 = mock_stdout.getvalue().strip()
            self.assertEqual(output3, init_tree + '\n' + once_tree + '\n' + twice_tree)


if __name__ == "__main__":
    unittest.main()
