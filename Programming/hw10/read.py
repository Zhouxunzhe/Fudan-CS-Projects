# 读取上传的文件
file_path = './三行情诗'

# 打开并读取文件内容
with open(file_path, 'r', encoding='utf-8') as file:
    content = file.read()

# 输出文件内容
print(content)
