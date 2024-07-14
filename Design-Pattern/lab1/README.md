# Lab(设计模式)
# 0. 文件目录树形结构

```
└── lab1/
    ├── __init__.py
    ├── CommandTest.py
    ├── lab1.md
    ├── main.py
    ├── README.md
    ├── bookmarks/
    |   ├── cloud.bmk
    |   ├── cloud1.bmk
    |   ├── cloud2.bmk
    |   └── test/
    |       └── test.bmk
    ├── main/
    |   ├── __init__.py
    |   ├── AddBookmarkCommand.py
    |   ├── AddTitleCommand.py
    |   ├── BookmarkManager.py
    |   ├── Command.py
    |   ├── CommandHandler.py
    |   ├── CommandHistory.py
    |   ├── DeleteCommand.py
    |   ├── Node.py
    |   └── Plugin.py
    └── test/
        ├── __init__.py
        ├── AddTest.py
        ├── DeleteTest.py
        ├── LsTest.py
        ├── ReadTest.py
        ├── SaveOpenTest.py
        ├── ShowTest.py
        ├── Test.py
        └── UndoRedoTest.py
```

#  1. 系统总体设计

本项目实现了一个基于命令行的网站书签编辑器。系统设计采用面向对象的方法，通过多种设计模式提高灵活性，并使用自动化测试来验证设计和实现的有效性。此应用允许用户通过命令行界面管理其网站书签，支持创建、修改、删除和检索书签信息，所有这些都在一个结构化的书签树中进行管理。

### 主要组件

- **`BookmarkManager`**: 作为核心组件，`BookmarkManager`负责书签的整体管理，包括添加、删除、修改书签或标题。`BookmarkManager`维护一个书签树，其中每个节点都是一个`Node`对象，可以是一个书签或一个书签组 `title`。此外，`BookmarkManager`还负责加载和保存书签数据到文件，保证了文件的长期存储。
- **`CommandHandler`**: 该组件的主要职责是解析用户从命令行输入的命令，并调用相应的命令对象来执行这些命令。`CommandHandler`使用工厂模式来根据输入的命令文本创建相应的`Command`对象，并管理这些命令对象的执行历史，允许撤销和重做操作。
- **`Command`**: `Command`是一个抽象基类，定义了所有命令对象必须实现的`execute`和`undo`接口。这个设计允许系统以统一的方式处理各种命令，同时也支持命令的撤销和重做。
- **具体命令**: 具体命令类如`AddBookmarkCommand`, `AddTitleCommand`, `DeleteCommand`等实现了`Command`接口。每个类封装了具体的业务逻辑，如添加一个新的书签节点或删除一个已存在的标题节点。这些命令对象通过`CommandHandler`执行，并可被`CommandHistory`管理。
- **`CommandHistory`**: 此组件管理命令的执行历史，实现命令的撤销（undo）和重做（redo）功能。它通过维护两个栈（一个撤销栈`undo_stack`和一个重做栈`redo_stack`）来追踪命令的状态，允许用户回退或重复执行操作。
- **`Node`**: `Node`类代表书签和标题的数据结构，支持复合结构，允许书签和标题以树状层次组织。每个节点可以有多个子节点，并且可以是书签或其他标题。这种设计使用组合模式来统一处理书签和书签组。
- **`PluginManager和Plugin`**: `PluginManager`负责管理插件，如`CountPlugin`和`FilePlugin`等。这些插件提供了额外的功能，例如统计每个书签的访问次数或修改文件的附加属性。`PluginManager`使用观察者模式来在节点状态发生变化时动态激活插件，例如当书签被读取时更新读取计数。

# 2. 设计模式的应用

本项目中使用了多种设计模式，以增加代码的灵活性和可维护性。

### 使用的设计模式

- **工厂模式**: 在`CommandHandler`的`execute`方法中，根据命令字符串动态创建具体命令对象的逻辑，可以看作是一种简单的工厂模式。根据输入决定实例化哪个命令类，这里虽然没有形成独立的工厂类，但使用条件语句来创建特定的命令对象体现了工厂方法的思想。
- **单例模式**: 主要用于`CommandHandler`和`BookmarkManager`的实例管理，`bookmark_manager`和`command_handler`实例在被`__init__.py`文件导入时创建，并在模块范围内全局存在，确保全局唯一性。
- **适配器模式**: `CommandHandler`在某种程度上起到了适配器的作用，它将字符串形式的命令转换为可以执行的命令对象。这一行为在将用户输入适配到系统内部可以执行的操作上体现了适配器模式的精神。
- **组合模式** `BookmarkManager`使用了组合模式，通过`Node`类来管理书签和标题的层级结构。在这种模式中，`Node`对象可以拥有子节点，这些子节点本身也是`Node`对象。这使得单个节点和节点的组合可以被统一处理。
- **外观模式**: `BookmarkManager`提供了一个高级接口，简化了与书签和标题层级结构的交互。其中`BookmarkManager`作为客户端与系统内部更复杂的`Node`结构之间的中介。
- **命令模式**: `CommandHandler`类的核心功能基于命令模式。这个设计模式将一个请求或简单操作封装为一个对象，从而允许用户支持用户发起的操作的所有记录、处理和撤销操作。这里，不同的命令如添加标题、添加书签、删除等都被封装为命令对象（如`AddTitleCommand`, `AddBookmarkCommand`, `DeleteCommand`等），这些命令都继承自一个共同的接口或抽象类`Command`。
- **观察者模式**: 通过`PluginManager`和其插件（如`CountPlugin`和`FilePlugin`），实现了观察者模式。`BookmarkManager`在运行时可能会通知`PluginManager`，后者再运行所有已注册的插件，这些插件会对书签管理器的状态或操作作出响应。
- **模板方法模式**: `Command`类中使用了模板方法模式。这是一种基于继承的模式，用于定义一个操作中的算法的骨架，同时允许子类为一个或多个步骤提供具体的实现。通过这种方式，算法的结构被保存在一个地方，而具体的步骤可以在子类中自定义，这样做可以减少代码的冗余。
- **策略模式**: 通过插件系统的实现，`BookmarkManager`允许在运行时改变应用的某些行为，例如如何处理额外的节点信息。每个插件都可以看作是一个具体的策略，`PluginManager`根据上下文动态选择使用哪个插件。
- **责任链模式**: `CommandHandler`中的`execute`方法中的决策逻辑可以视为责任链模式的一种形式。在这种模式中，一个请求沿着一个链传递，直到某个处理器对象负责处理它。在这里，`execute`方法根据命令类型决定调用哪个具体的命令对象处理请求，类似责任链中的处理器选择逻辑。
- **备忘录模式**: 虽然不是传统的备忘录模式，但`CommandHistory`通过保存命令对象的状态（在栈中），间接地实现了备忘录模式的核心思想：捕捉并外部化对象的内部状态，以便以后可以将对象恢复到该状态。在这种情况下，命令对象本身封装了其执行的状态，而`CommandHistory`管理这些状态的历史记录。
- **状态模式**: 通过切换`undo_stack`和`redo_stack`中命令对象的状态，`CommandHistory`在某种程度上实现了状态模式。这里的状态是指命令的执行状态，而`CommandHistory`负责在这些状态之间进行切换。

# 3. 类的详细设计

### 1. BookmarkManager 类

#### 属性：

- `root`: 根节点，类型为 `Node`，代表书签栏的根结点。
- `nodes`: 字典，存储节点名称和对应的节点对象，用于快速查找节点。
- `file_path`: 字符串，表示当前文件的路径。
- `opened_files`: 集合，存储已打开的文件路径。
- `current_file`: 字符串，表示当前正在编辑的文件路径。
- `plugin_manager`: `PluginManager` 对象，管理插件的注册和运行。
- `is_saved`: 布尔值，表示当前工作空间是否已保存。

#### 方法：

- `__init__(self, file_path=None, opened_files=None)`: 初始化方法，初始化属性，加载书签文件并注册插件。
- `add_title(self, title, parent_title="个人收藏")`: 添加标题节点到指定父节点下。
- `add_bookmark(self, title, url, parent_title="个人收藏")`: 添加书签节点到指定父节点下。
- `delete_title(self, title)`: 删除指定标题节点及其子节点。
- `delete_bookmark(self, title)`: 删除指定书签节点。
- `_delete_node(self, node)`: 递归删除节点及其子节点的辅助方法。
- `load_bookmarks(self, file_path)`: 从文件中加载书签树。
- `save_bookmarks(self, file_path=None)`: 将书签树保存到文件。
- `_write_node(self, node, file, depth)`: 递归写入节点及其子节点到文件的辅助方法。
- `_get_level(self, node)`: 获取节点的层级。
- `show_tree(self, node=None, indent=0, last_nodes=None)`: 以树形结构显示书签树。
- `read_bookmark(self, bookmark_name)`: 将指定书签标记为已读。
- `open_file(self, file_path)`: 打开指定文件。
- `close_file(self, file_path=None)`: 关闭指定文件或当前文件。
- `list_directory_tree(self, path, prefix='', is_root=True)`: 以树形结构显示目录树。
- `has_title(self, title)`: 判断是否存在指定标题。
- `has_bookmark(self, bookmark_name, parent_title="个人收藏")`: 判断是否存在指定书签。
- `trees_equal(self, other_manager)`: 判断两个书签管理器的书签树是否相等。

### 2. Plugin 抽象类

#### 方法：

- `add_info(self, info)`: 抽象方法，用于向节点添加额外信息。

### 3. PluginManager 类

#### 属性：

- `plugins`: 列表，存储注册的插件对象。

#### 方法：

- `__init__(self)`: 初始化方法，初始化插件列表。
- `register_plugin(self, plugin)`: 注册插件。
- `run_plugins(self, info)`: 运行所有插件，向节点添加额外信息。

### 4. CountPlugin 类

#### 属性：

- `use_count_plugin`: 布尔值，表示是否使用计数插件。

#### 方法：

- `__init__(self, use_count_plugin=True)`: 初始化方法，设置是否使用计数插件。
- `add_info(self, node)`: 向书签节点添加阅读计数信息。

### 5. FilePlugin 类

#### 属性：

- `use_file_plugin`: 布尔值，表示是否使用文件插件。

#### 方法：

- `__init__(self, use_file_plugin=True)`: 初始化方法，设置是否使用文件插件。
- `add_info(self, info)`: 向节点信息添加文件插件信息。

### 6. CommandHandler 类

#### 属性：

- `manager`: `BookmarkManager` 对象，用于管理书签。
- `history`: `CommandHistory` 对象，用于记录和执行历史命令。

#### 方法：

- `__init__(self, manager=None)`: 初始化方法，接受一个可选的 `BookmarkManager` 对象作为参数，默认为 `None`。
- `execute(self, command_str)`: 解析并执行命令字符串。
- `_split_string(self, command)`: 将命令字符串拆分成多个部分，返回一个字符串列表。

#### execute 方法内部逻辑：

1. 解析命令字符串，提取命令动作和参数。
2. 根据命令动作，执行相应的操作：
   - `add-title`: 添加标题命令。
   - `add-bookmark`: 添加书签命令。
   - `delete-title` 或 `delete-bookmark`: 删除标题或书签命令。
   - `undo`: 撤销上一步操作。
   - `redo`: 重做上一步撤销的操作。
   - `open`, `bookmark` 或 `edit`: 打开文件命令。
   - `close`: 关闭文件命令。
   - `save`: 保存命令。
   - `show-tree`: 显示书签树命令。
   - `ls-tree`: 显示目录树命令。
   - `read-bookmark`: 标记书签为已读命令。
   - 其他: 抛出 ValueError 异常。
3. 根据命令动作创建对应的命令对象，并执行该命令。
4. 将命令添加到历史记录中。

### 7. CommandHistory 类

#### 属性：

- `undo_stack`: 栈，存储执行过的命令，用于撤销操作。
- `redo_stack`: 栈，存储撤销过的命令，用于重做操作。

#### 方法：

- `__init__(self)`: 初始化方法，创建 `undo_stack` 和 `redo_stack`。
- `execute(self, command)`: 执行命令，并将命令添加到 `undo_stack` 中，清空 `redo_stack`。
- `undo(self)`: 撤销上一步操作，从 `undo_stack` 中弹出最后一个命令，执行其撤销操作，并将该命令添加到 `redo_stack` 中。
- `redo(self)`: 重做上一步撤销的操作，从 `redo_stack` 中弹出最后一个命令，执行其执行操作，并将该命令添加到 `undo_stack` 中。

### 8. Node 类

#### 属性：

- `name`: 字符串，表示节点的名称。
- `parent`: Node 对象，表示节点的父节点，如果是根节点则为 None。
- `children`: 列表，存储节点的子节点。
- `is_bookmark`: 布尔值，表示节点是否为书签。
- `url`: 字符串，表示书签节点对应的 URL 地址。
- `is_read`: 布尔值，表示书签节点是否已读。
- `read_count`: 整数，表示书签节点的阅读次数。

#### 方法：

- `__init__(self, name, parent=None, is_bookmark=False, url=None, is_read=False)`: 初始化方法，初始化节点的属性。
- `add_child(self, child)`: 将指定节点添加为当前节点的子节点。
- `mark_as_read(self)`: 将书签节点标记为已读，并增加阅读次数。

### 9. AddBookmarkCommand 类

#### 属性：

- `manager`: `BookmarkManager` 对象，表示书签管理器。
- `title`: 字符串，表示要添加的书签标题。
- `url`: 字符串，表示要添加的书签 URL 地址。
- `parent_title`: 字符串，表示要添加书签的父节点标题，默认为 "个人收藏"。
- `node`: Node 对象，表示添加的书签节点。

#### 方法：

- `__init__(self, manager, title, url, parent_title="个人收藏")`: 初始化方法，接受书签管理器、书签标题、URL 地址和父节点标题作为参数。
- `execute(self)`: 执行添加书签的操作，调用书签管理器的 `add_bookmark` 方法添加书签，并返回添加的书签节点。
- `undo(self)`: 撤销添加书签的操作，如果添加过书签，则调用书签管理器的 `delete_bookmark` 方法删除该书签。

### 10. AddTitleCommand 类

#### 属性：

- `manager`: `BookmarkManager` 对象，表示书签管理器。
- `title`: 字符串，表示要添加的标题。
- `parent_title`: 字符串，表示要添加标题的父节点标题，默认为 "个人收藏"。
- `node`: Node 对象，表示添加的标题节点。

#### 方法：

- `__init__(self, manager, title, parent_title="个人收藏")`: 初始化方法，接受书签管理器、标题和父节点标题作为参数。
- `execute(self)`: 执行添加标题的操作，调用书签管理器的 `add_title` 方法添加标题，并返回添加的标题节点。
- `undo(self)`: 撤销添加标题的操作，如果添加过标题，则调用书签管理器的 `delete_title` 方法删除该标题。


这段代码定义了一个名为 `DeleteCommand` 的类，用于表示删除节点的命令。

### 11. DeleteCommand 类

#### 属性：

- `manager`: `BookmarkManager` 对象，表示书签管理器。
- `title`: 字符串，表示要删除的节点的标题。
- `deleted_node`: 列表，存储已删除的节点。
- `parent_node`: Node 对象，表示要删除节点的父节点。
- `children`: 列表，用于保存删除的节点的所有子节点的信息。
- `cnt`: 整数，用于保存当前节点的撤销/重做次数。

#### 方法：

- `__init__(self, manager, title)`: 初始化方法，接受书签管理器和要删除的节点的标题作为参数。
- `execute(self)`: 执行删除节点的操作，将要删除的节点添加到 `deleted_node` 中，并调用书签管理器的 `delete_title` 或 `delete_bookmark` 方法删除节点。
- `_store_children(self, node)`: 私有方法，递归地保存要删除节点的所有子节点的信息。
- `undo(self)`: 撤销删除节点的操作，根据已删除节点的信息和保存的子节点信息，逐步恢复删除的节点和其子节点。

# 4. 自动化测试

### 1. AddTest 类

#### 属性：

- `bookmark_manager`: `BookmarkManager` 对象，表示书签管理器，用于执行添加操作。
- `command_handler`: `CommandHandler` 对象，表示命令处理器，用于执行添加命令。

#### 方法：

- `__init__(self, methodName)`: 初始化方法，接受一个参数 `methodName`，用于调用父类的初始化方法。在初始化过程中，先创建了 `BookmarkManager` 和 `CommandHandler` 对象，然后执行了一系列添加标题和添加书签的命令。
- `test_add_title(self)`: 测试添加标题功能的方法，通过断言检查各个标题是否已经被正确添加。
- `test_add_bookmark(self)`: 测试添加书签功能的方法，通过断言检查各个书签是否已经被正确添加到相应的标题下。

### 2. DeleteTest 类

#### 属性：

- `bookmark_manager`: `BookmarkManager` 对象，表示书签管理器，用于执行删除操作。
- `command_handler`: `CommandHandler` 对象，表示命令处理器，用于执行删除命令。

#### 方法：

- `__init__(self, methodName)`: 初始化方法，接受一个参数 `methodName`，用于调用父类的初始化方法。在初始化过程中，先创建了 `BookmarkManager` 和 `CommandHandler` 对象，然后执行了一系列添加标题和添加书签的命令。
- `test_delete_title(self)`: 测试删除标题功能的方法，通过执行删除标题的命令后，断言检查相关标题和书签是否已经被正确删除。
- `test_delete_bookmark(self)`: 测试删除书签功能的方法，通过执行删除书签的命令后，断言检查相关书签是否已经被正确删除。

### 3. LsTest 类

#### 属性：

- `bookmark_manager`: `BookmarkManager` 对象，表示书签管理器，用于执行保存和打开文件等操作。
- `command_handler`: `CommandHandler` 对象，表示命令处理器，用于执行 `ls-tree` 命令。

#### 方法：

- `__init__(self, methodName)`: 初始化方法，接受一个参数 `methodName`，用于调用父类的初始化方法。在初始化过程中，创建了 `BookmarkManager` 和 `CommandHandler` 对象。
- `test_ls_tree(self)`: 测试 `ls-tree` 命令的方法。在该方法中，首先执行了 `save` 和 `open` 命令，然后通过 `patch` 函数模拟了标准输出，执行 `ls-tree` 命令，并断言输出结果与预期输出相符。接着进行了一系列操作后再次执行 `ls-tree` 命令，再次断言输出结果与预期输出相符。

### 4. ReadTest 类

#### 属性：

- `bookmark_manager`: `BookmarkManager` 对象，表示书签管理器，用于添加书签和执行命令。
- `command_handler`: `CommandHandler` 对象，表示命令处理器，用于执行命令。

#### 方法：

- `__init__(self, methodName)`: 初始化方法，接受一个参数 `methodName`，用于调用父类的初始化方法。在初始化过程中，创建了 `BookmarkManager` 和 `CommandHandler` 对象，并执行了一系列添加书签的命令。
- `test_show_tree(self)`: 测试 `show-tree` 命令的方法。在该方法中，首先通过 `patch` 函数模拟了标准输出，执行 `show-tree` 命令，并断言输出结果与预期输出相符。接着执行了两次 `read-bookmark` 命令，再次执行 `show-tree` 命令，并断言输出结果与预期输出相符。

### 5. SaveOpenTest 类

#### 属性：

- `bookmark_manager`: `BookmarkManager` 对象，表示书签管理器，用于添加书签和执行命令。
- `command_handler`: `CommandHandler` 对象，表示命令处理器，用于执行命令。
- `file_path`: 字符串，表示测试文件的路径。

#### 方法：

- `__init__(self, methodName)`: 初始化方法，接受一个参数 `methodName`，用于调用父类的初始化方法。在初始化过程中，创建了 `BookmarkManager` 和 `CommandHandler` 对象，并执行了一系列添加书签的命令。
- `test_save_file(self)`: 测试文件保存功能的方法。在该方法中，执行了保存文件的命令，并读取保存的文件内容。然后断言读取的文件内容与预期内容相符，并删除测试文件。
- `test_open_file(self)`: 测试文件打开功能的方法。在该方法中，首先保存了文件，然后创建了一个新的 `BookmarkManager` 和 `CommandHandler` 对象，并执行了打开文件的命令。最后断言打开的书签管理器与原始的书签管理器相等，并删除测试文件。

### 6. ShowTest 类

#### 属性：

- `bookmark_manager`: `BookmarkManager` 对象，表示书签管理器，用于添加书签和执行命令。
- `command_handler`: `CommandHandler` 对象，表示命令处理器，用于执行命令。

#### 方法：

- `__init__(self, methodName)`: 初始化方法，接受一个参数 `methodName`，用于调用父类的初始化方法。在初始化过程中，创建了 `BookmarkManager` 和 `CommandHandler` 对象，并执行了一系列添加书签的命令。
- `test_show_tree(self)`: 测试显示书签树功能的方法。在该方法中，使用 `patch` 装饰器模拟了 `sys.stdout`，然后执行了显示书签树的命令，并断言输出的书签树与预期的书签树相符。

### 7. UndoRedoTest 类

#### 属性：

- `bookmark_manager`: `BookmarkManager` 对象，表示书签管理器，用于添加书签和执行命令。
- `command_handler`: `CommandHandler` 对象，表示命令处理器，用于执行命令。

#### 方法：

- `__init__(self, methodName)`: 初始化方法，接受一个参数 `methodName`，用于调用父类的初始化方法。在初始化过程中，创建了 `BookmarkManager` 和 `CommandHandler` 对象。
- `test_add_undo_redo(self)`: 测试添加书签并执行撤销和重做功能的方法。在该方法中，执行了一系列添加书签的命令，并在每次添加后执行撤销和重做命令，断言书签是否被正确添加、删除和恢复。
- `test_delete_undo_redo(self)`: 测试删除书签并执行撤销和重做功能的方法。在该方法中，执行了一系列添加和删除书签的命令，并在每次删除后执行撤销和重做命令，断言书签是否被正确删除和恢复。

### 8. **总结**

**测试用例设计**：在自动化测试中根据软件需求和功能设计编写了一系列测试用例，包括针对添加、删除、保存、打开、撤销重做、展示和列出书签等操作的测试用例。每个测试用例都覆盖了不同的功能和边界条件，以确保系统在各种情况下的正确性和稳定性。

**模块化测试**：在自动化测试中按照模块的功能对测试用例进行了分组，每个测试文件对应一个模块，例如添加、删除、保存、打开等。这种模块化的测试方法使得测试用例更易于管理和维护，并且使得在特定模块出现问题时更容易定位和修复。具体实现了以下测试：

- **单元测试**: 每个类的关键功能都经过详细测试，确保其按预期工作。
- **集成测试**: 测试不同组件间的交互，确保整个系统协同工作。

**测试框架选择**：在自动化测试中选择了 Python 自带的 unittest 框架作为自动化测试的工具，因为它是 Python 标准库的一部分，易于使用和集成。unittest 提供了丰富的断言方法和测试运行器，支持测试套件的组织和管理，适用于各种规模的项目。

**Mocking 和 Patching**：在一些需要模拟外部依赖或者环境的测试中，使用了 unittest.mock 模块提供的 Mock 和 patch 工具。这些工具允许模拟对象的行为或替换系统调用，使得测试更加独立和可控，同时避免了对外部资源的依赖。

# 5. 总结

本实验通过建立一个结构化的面向对象模型和使用多种设计模式，成功实现了一个灵活且功能完备的书签管理系统。通过自动化测试的广泛应用，确保了代码的质量和功能的正确性。

# 6. 使用方法

运行`main.py`，使用命令行进行指令操作

```
python main.py
```

运行`Test.py`，执行自动化集成测试，运行对应的测试文件，执行对应的自动化单元测试

```
cd test
python Test.py
python <YourTest.py>
```

运行`CommandTest.py`，执行样例测试代码

```
python CommandTest.py
```