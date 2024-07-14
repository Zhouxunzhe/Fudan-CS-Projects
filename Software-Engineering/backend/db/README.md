该模块用于创建数据库<br>
修改options.py中的参数以适配你的主机, 也可以使用默认配置<br>
进入mysql root用户后，<br>
首先使用`create database Web_Project;`创建数据库<br>
使用`use Web_Project;`进入数据库<br>
`source 你的路径/db.sql;` 导入建表文件<br>
然后使用 `grant all on Web_Project.* to soft_engineer@'localhost' identified by '123456';` 命令以按照默认设置配置数据库。<br>

单独运行admin_register.py可以初始化管理员。