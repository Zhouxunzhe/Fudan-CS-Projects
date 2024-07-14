from lab1.main.BookmarkManager import BookmarkManager
from lab1.main.CommandHandler import CommandHandler
import unittest


class DeleteTest(unittest.TestCase):
    def __init__(self, methodName):
        super(DeleteTest, self).__init__(methodName)
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

    def test_delete_title(self):
        self.command_handler.execute('delete-title "参考资料"')
        # check add-title
        self.assertFalse(self.bookmark_manager.has_title("参考资料"))
        self.assertFalse(self.bookmark_manager.has_title("函数式"))
        self.assertFalse(self.bookmark_manager.has_bookmark("Markdown Guide", "参考资料"))
        self.assertFalse(self.bookmark_manager.has_bookmark("JFP", "函数式"))

    def test_delete_bookmark(self):
        self.command_handler.execute('delete-bookmark "Markdown Guide"')
        self.command_handler.execute('delete-bookmark "Category Theory"')
        # check add-bookmark
        self.assertFalse(self.bookmark_manager.has_bookmark("Markdown Guide", "参考资料"))
        self.assertFalse(self.bookmark_manager.has_bookmark("Category Theory", "待阅读"))


if __name__ == "__main__":
    unittest.main()
