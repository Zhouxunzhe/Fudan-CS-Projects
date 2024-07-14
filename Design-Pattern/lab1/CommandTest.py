from lab1 import command_handler

if __name__ == "__main__":
    # add
    command_handler.execute('add-title "课程"')
    command_handler.execute('add-title "参考资料"')
    # command_handler.execute('undo')
    command_handler.execute('add-title "函数式" at "参考资料"')
    command_handler.execute('save "bookmarks/cloud1.bmk"')
    command_handler.execute('undo')
    command_handler.execute('undo')
    command_handler.execute('undo')
    command_handler.execute('redo')
    command_handler.execute('redo')
    command_handler.execute('redo')
    command_handler.execute('add-title "OOP" at "参考资料"')
    command_handler.execute('add-title "待阅读"')
    command_handler.execute('add-bookmark "elearning"@"https://elearning.fudan.edu.cn/courses" at "课程"')
    command_handler.execute('add-bookmark "Markdown Guide"@"https://www.markdownguide.org" at "参考资料"')
    command_handler.execute('add-bookmark "Markdown 数学公式"@"https://www.markdownmath.org" at "参考资料"')
    command_handler.execute('save "bookmarks/cloud2.bmk"')
    command_handler.execute(
        'add-bookmark "The essence of functional programming"@"https://www.cambridge.org/core/journals/functional-programming" at "函数式"')
    command_handler.execute(
        'add-bookmark "JFP"@"https://www.cambridge.org/core/journals/journal-of-functional-programming" at "函数式"')
    command_handler.execute(
        'add-bookmark "Category Theory"@"http://www.appliedcategorytheory.org/what-is-applied-category-theory/" at "待阅读"')

    # delete
    # command_handler.execute('delete-title "参考资料"')
    # command_handler.execute('undo')
    # command_handler.execute('delete-bookmark "Markdown Guide"')

    # load/save
    command_handler.execute('save "bookmarks/cloud.bmk"')
    command_handler.execute('open "bookmarks/cloud1.bmk"')
    command_handler.execute('open "bookmarks/cloud2.bmk"')
    command_handler.execute('open "bookmarks/cloud.bmk"')
    command_handler.execute('close "bookmarks/cloud1.bmk"')
    # command_handler.execute('edit "bookmarks/cloud.bmk"')
    # bookmark_manager = BookmarkManager("bookmarks/cloud.bmk")
    # command_handler = CommandHandler(bookmark_manager)
    # command_handler.execute('save "bookmarks/cloud.bmk"')

    # read bookmarks
    command_handler.execute('show-tree')
    command_handler.execute('read-bookmark "elearning"')
    command_handler.execute('show-tree')

    # show ls-tree
    command_handler.execute('ls-tree')
