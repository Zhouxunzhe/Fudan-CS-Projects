# Finance-Quant-Programming
## Introduction

**Quant Programming Midterm Project 2024 Spring**

- Class *Investment* is the base for all kinds of the investment products.
- Inherit the *Investment* class and implement the *calculate_indicators* and *execute_strategy* methods to realize the investment strategies for various investment products.
- You can find some commonly used technical indicators and risk management classes in the *TechnicalLib* module. 
- The *Portfolio* class will help you build your own investment portfolio with these investment targets and perform backtesting. Enjoy designing and testing your strategy with this project!

## Quickstart

Clone repo:

```bash
$ git clone https://github.com/Zhouxunzhe/QuantProgramming.git
```

Install requirements:

```bash
$ cd QuantProgramming
$ pip install --upgrade pip
$ pip install -r requirements.txt
```

Go to code source:

```bash
$ cd src
```

Run the code below to evaluate all the investment (10 for each):

```bash
$ python main.py
```

run the code below the evaluate a specific investment you like:

```bash
$ python investment/Stock.py # you can modify the file name
```

## Troubleshooting

可能文件相对路径会存在问题，如果无法正常在命令行中运行的话，请打开`Python IDE`，如`PyCharm`，设置`QuantProgramming`为项目文件，运行`main.py`和其他投资品文件即可。（已修复，两种方式都可正常运行）

# 量化投资编程
量化程序设计期中项目 2024春

- *Investment*类是所有其他投资标的的基类。
- 继承*Investment*类并实现其中的*calculate_indicators*、*execute_strategy*方法来实现各个投资品的投资策略。
- 你可以在*TechnicalLib*模块中找到一些常用的技术指标以及风险管理类。
- *Portfolio*将帮助你利用这些投资标的构建自己的投资组合，并进行回测。欢迎使用这个项目设计并测试你的投资策略！

