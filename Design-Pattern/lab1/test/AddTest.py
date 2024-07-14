from lab1.main.BookmarkManager import BookmarkManager
from lab1.main.CommandHandler import CommandHandler
import unittest


class AddTest(unittest.TestCase):
    def __init__(self, methodName):
        super(AddTest, self).__init__(methodName)
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

    def test_add_title(self):
        self.assertTrue(self.bookmark_manager.has_title("课程"))
        self.assertTrue(self.bookmark_manager.has_title("参考资料"))
        self.assertTrue(self.bookmark_manager.has_title("函数式"))
        self.assertTrue(self.bookmark_manager.has_title("面向对象"))
        self.assertTrue(self.bookmark_manager.has_title("待阅读"))

    def test_add_bookmark(self):
        self.assertTrue(self.bookmark_manager.has_bookmark("elearning", "课程"))
        self.assertTrue(self.bookmark_manager.has_bookmark("Markdown Guide", "参考资料"))
        self.assertTrue(self.bookmark_manager.has_bookmark("JFP", "函数式"))
        self.assertTrue(self.bookmark_manager.has_bookmark("Category Theory", "待阅读"))


if __name__ == "__main__":
    unittest.main()
