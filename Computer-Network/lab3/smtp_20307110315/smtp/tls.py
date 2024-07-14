import os
import ssl
from tkinter import *
from tkinter import ttk
from email.base64mime import body_encode
from socket import *
import pickle

# TLS加密
# context = ssl.SSLContext(ssl.PROTOCOL_TLS)
context = ssl.create_default_context()

# 初始化窗口
root = Tk()
root.title("Send your email from here !")

# 尝试读取联系人
try:
    with open('contacts.pkl', 'rb') as f:
        contacts = pickle.load(f)
except:
    # 默认联系人为自己的邮箱
    contacts = ['1170543554@qq.com']

# 尝试读取已发送邮件
try:
    with open('history.pkl', 'rb') as f:
        history = pickle.load(f)
except:
    history = []

# 尝试读取已发送邮件
try:
    with open('garbage.pkl', 'rb') as f:
        garbage = pickle.load(f)
except:
    garbage = []

account = StringVar()
password = StringVar()
receiver = StringVar()
subject = StringVar()
msgbody_content = ""

# 尝试读取之前存储的草稿
try:
    with open('draft.pkl', 'rb') as f:
        info = pickle.load(f)
        account.set(info[0])
        password.set(info[1])
        receiver.set(info[2])
        subject.set(info[3])
        msgbody_content = info[4]
except:
    pass


# 尝试读取之前存储的账号密码
try:
    with open('account.pkl', 'rb') as f:
        info = pickle.load(f)
        account.set(info[0])
        password.set(info[1])
except:
    pass


# 创建窗口
mainframe = ttk.Frame(root, padding="3 3 12 12")
mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
mainframe.columnconfigure(0, weight=1)
mainframe.rowconfigure(0, weight=1)

# 增加文本框用于输入发件人邮箱
ttk.Label(mainframe, text="Your Email Account: ").grid(column=0, row=1, sticky=W)
account_entry = ttk.Entry(mainframe, width=30, textvariable=account)
account_entry.grid(column=4, row=1, sticky=(W, E))

# 增加文本框用于输入密码
ttk.Label(mainframe, text="Your Password: ").grid(column=0, row=2, sticky=W)
password_entry = ttk.Entry(mainframe, show="*", width=30, textvariable=password)
password_entry.grid(column=4, row=2, sticky=(W, E))


# 选择联系人，或输入新联系人
def contact():
    # 打开新窗口
    tk_contact = Toplevel()

    # 每个联系人生成一个复选框
    CheckButton = {}
    for k in contacts:
        CheckButton[k] = IntVar()
        Checkbutton(tk_contact, text=k, variable=CheckButton[k]).pack(anchor=W)

    # 新联系人通过一行文本输入，用分号 ';' 隔开，空格会被自动过滤
    Label(tk_contact, text="New Contact: ").pack(anchor=W)
    new_contactVar = StringVar()
    Entry(tk_contact, textvariable=new_contactVar).pack(anchor=W)

    # 读取外部已经设置的收件人，将其解析，同步到复选框和文本框
    receiver_list = [x.strip() for x in receiver.get().strip().split(';')]
    receiver_remain = []
    for k in receiver_list:
        if k in CheckButton:
            CheckButton[k].set(1)
        else:
            receiver_remain.append(k)
    new_contactVar.set('; '.join(receiver_remain))

    # 选择好联系人后，将其关闭，并同步到外部收件人文本框
    def contact_close():
        receiver_list = []
        for k, v in CheckButton.items():
            if v.get() == 1:
                receiver_list.append(k)

        for _ in new_contactVar.get().strip().split(";"):
            if _.strip() != "":
                receiver_list.append(_.strip())

        receiver.set(';\n'.join(receiver_list))

        tk_contact.destroy()

    Button(tk_contact, text="Accept", command=contact_close).pack(anchor=E)

    tk_contact.mainloop()


# 增加按钮用于选择收件人，旧联系人通过复选框选择，新联系人通过文本框输入
ttk.Label(mainframe, text="Recepient's Email Account: ").grid(column=0, row=3, sticky=W)
receiver_entry = ttk.Button(mainframe, width=30, textvariable=receiver, command=contact)
receiver_entry.grid(column=4, row=3, sticky=(W, E))

# 增加文本框用于输入标题
ttk.Label(mainframe, text="Subject: ").grid(column=0, row=6, sticky=W)
subject_entry = ttk.Entry(mainframe, width=30, textvariable=subject)
subject_entry.grid(column=4, row=6, sticky=(W, E))

# 增加多行文本框用于输入正文
ttk.Label(mainframe, text="Message Body: ").grid(column=0, row=7, sticky=W)
msgbody = Text(mainframe, width=30, height=10)
msgbody.insert("end", msgbody_content)
msgbody.grid(column=4, row=7, sticky=(W, E))


textvar = StringVar()
ttk.Label(mainframe, textvariable=textvar).grid(column=4, row=9, sticky=W)


# 展示历史已发送文件，可选择其中之一并读取，或直接关闭窗口退出
def show_history():
    tk_history = Toplevel()
    max_name_length = 0
    for info in history:
        max_name_length = max(max_name_length, len(info[0]))
    max_name_length = min(max_name_length, 40)

    for index, info in enumerate(history):
        # 对齐发件人的邮箱长度
        _account = info[0]
        if len(_account) > max_name_length:
            _account = _account[:max_name_length - 3] + '...'
        if len(_account) < max_name_length:
            _account = _account + ' ' * (max_name_length - len(_account))

        _subject = info[3]
        if len(_subject) > max_name_length:
            _subject = _subject[:max_name_length - 3] + '...'

        # 使用闭包函数生成对应的 command 处理函数
        def select_history():
            _info = info

            def f():
                account.set(_info[0])
                password.set(_info[1])
                receiver.set(_info[2])
                subject.set(_info[3])
                msgbody.delete('0.0', END)
                msgbody.insert('end', _info[4])

                tk_history.destroy()

            return f

        def delete_history():
            _info = info

            def f():
                history.remove(_info)
                garbage.append(_info)
                with open('history.pkl', 'wb') as h:
                    pickle.dump(history, h)
                with open('garbage.pkl', 'wb') as g:
                    pickle.dump(garbage, g)

                tk_history.destroy()
                show_history()

            return f

        # 显示 发件人 和 邮件标题，根据这些信息选择历史邮件，读取其中内容，并退出
        Button(tk_history, width=30, height=1, text="{}: {}".format(_account, _subject), command=select_history(),
               anchor=W, justify=LEFT).grid(row=index, column=0)
        # 对选中的历史邮件进行删除，并且重新打开窗口
        Button(tk_history, width=5, height=1, text="delete", command=delete_history(),
               anchor=E, justify=RIGHT).grid(row=index, column=1)

    tk_history.mainloop()


# 增加按钮用于选择历史已发送邮件
ttk.Button(mainframe, text="History", command=show_history).grid(column=0, row=8, sticky=W)


# 展示垃圾箱，实现恢复操作
def show_garbage():
    tk_garbage = Toplevel()
    max_name_length = 0
    for info in garbage:
        max_name_length = max(max_name_length, len(info[0]))
    max_name_length = min(max_name_length, 40)

    for index, info in enumerate(garbage):
        # 对齐发件人的邮箱长度
        _account = info[0]
        if len(_account) > max_name_length:
            _account = _account[:max_name_length - 3] + '...'
        if len(_account) < max_name_length:
            _account = _account + ' ' * (max_name_length - len(_account))

        _subject = info[3]
        if len(_subject) > max_name_length:
            _subject = _subject[:max_name_length - 3] + '...'


        def recover():
            _info = info

            def f():
                garbage.remove(_info)
                history.append(_info)
                with open('history.pkl', 'wb') as h:
                    pickle.dump(history, h)
                with open('garbage.pkl', 'wb') as g:
                    pickle.dump(garbage, g)

                tk_garbage.destroy()
                show_garbage()

            return f

        # 显示 发件人 和 邮件标题，根据这些信息选择垃圾邮件，读取其中内容，并恢复
        Button(tk_garbage, width=30, height=1, text="{}: {}".format(_account, _subject), command=recover(),
               anchor=W, justify=LEFT).grid(row=index, column=0)

    tk_garbage.mainloop()


# 增加按钮用于选择历史已发送邮件
ttk.Button(mainframe, text="Garbage", command=show_garbage).grid(column=1, row=8, sticky=W)


# 保存草稿，下次打开时如存在未发送的草稿，则自动恢复
def save_draft():
    info = [account.get(), password.get(), receiver.get(), subject.get(), msgbody.get('1.0', 'end')]
    with open('draft.pkl', 'wb') as f:
        pickle.dump(info, f)

    textvar.set("Draft saved successfully")


# 增加按钮用于保存草稿
ttk.Button(mainframe, text="Save Draft", command=save_draft).grid(column=2, row=8, sticky=E)


def save_account():
    info = [account.get(), password.get()]
    with open('account.pkl', 'wb') as f:
        pickle.dump(info, f)

    textvar.set("Account & Password saved successfully")


# 增加按钮用于保存草稿
ttk.Button(mainframe, text="Save Account & Password", command=save_account).grid(column=3, row=8, sticky=E)


# 使用 SMTP 发送邮件，仅支持 qq 邮箱
def sendemail():
    # 打开新窗口
    tk_send = Toplevel()

    # 发送或取消后，将其关闭
    def send_close():
        tk_send.destroy()

    def contact_send():
        try:
            fromAddress = account.get()

            # 输入
            toAddress = [x.strip() for x in receiver.get().split(';')]

            subject_ = subject.get()
            msg = "\r\n" + msgbody.get('1.0', 'end')
            endMsg = "\r\n.\r\n"

            mailServer = "smtp.qq.com"

            # 发送方，验证信息
            username = account.get()  # 输入自己的用户名对应的编码
            password_ = password.get()  # 此处不是自己的密码，而是开启SMTP服务时对应的授权码

            # 创建客户端套接字并建立连接
            serverPort = 465  # SMTP SSL使用465号端口
            with create_connection((mailServer, serverPort)) as sock:
                with context.wrap_socket(sock, server_hostname=mailServer) as clientSocket:

                    # 从客户套接字中接收信息
                    recv = clientSocket.recv(1024).decode()
                    print(recv)
                    if '220' != recv[:3]:
                        raise Exception('220 reply not received from server.\nInfo: \n{}'.format(recv))

                    # 发送 HELO 命令并且打印服务端回复
                    # 开始与服务器的交互，服务器将返回状态码250,说明请求动作正确完成
                    heloCommand = 'HELO alice\r\n'
                    clientSocket.send(heloCommand.encode())  # 随时注意对信息编码和解码
                    recvHelo = clientSocket.recv(1024).decode()
                    print(recvHelo)
                    if '250' != recvHelo[:3]:
                        raise Exception('250 reply not received from server.\nInfo: \n{}'.format(recvHelo))

                    # 发送"AUTH PLAIN"命令，验证身份.服务器将返回状态码334（服务器等待用户输入验证信息）
                    user_pass_encode64 = body_encode(f"\0{username}\0{password_}".encode('ascii'), eol='')
                    clientSocket.sendall(f'AUTH PLAIN {user_pass_encode64}\r\n'.encode())
                    recvAuth = clientSocket.recv(1024).decode()
                    print(recvAuth)
                    if '235' != recvAuth[:3]:
                        raise Exception('235 reply not received from server.\nInfo: \n{}'.format(recvAuth))

                    # 发送"MAIL FROM"命令, 向服务器发送 邮件发送地址
                    fromCommand = 'MAIL FROM: <' + fromAddress + '>\r\n'
                    clientSocket.sendall(fromCommand.encode())
                    recvFrom = clientSocket.recv(1024).decode()
                    print(recvFrom)
                    if '250' != recvFrom[:3]:
                        raise Exception('250 reply not received from server.\nInfo: \n{}'.format(recvFrom))

                    # 发送"RCPT TO"命令, 向服务器发送 邮件目的地
                    for to in toAddress:
                        rcptCommand = 'RCPT TO: <' + to + '>\r\n'
                        clientSocket.sendall(rcptCommand.encode())
                        recvTo = clientSocket.recv(1024).decode()  # 注意UDP使用sendto，recvfrom
                        print(recvTo)
                        if '250' != recvTo[:3]:
                            raise Exception('250 reply not received from server.\nInfo: \n{}'.format(recvTo))

                    # 发送"DATA"命令, 向服务器发送 数据址
                    clientSocket.sendall('DATA\r\n'.encode())
                    recvData = clientSocket.recv(1024).decode()
                    print(recvData)
                    if '354' != recvData[:3]:
                        raise Exception('354 reply not received from server.\nInfo: \n{}'.format(recvData))

                    # 编辑邮件信息，发送数据
                    contentType = "text/plain"

                    message = 'from:' + fromAddress + '\r\n'
                    for to in toAddress:
                        message += 'to:' + to + '\r\n'
                    message += 'subject:' + subject_ + '\r\n'
                    message += 'Content-Type:' + contentType + '\t\n'
                    message += '\r\n' + msg
                    clientSocket.sendall(message.encode())

                    # 以"."结束。请求成功返回 250
                    clientSocket.sendall(endMsg.encode())
                    recvEnd = clientSocket.recv(1024).decode()
                    print(recvEnd)
                    if '250' != recvEnd[:3]:
                        raise Exception('250 reply not received from server.\nInfo: \n{}'.format(recvEnd))

                    # 发送"QUIT"命令, 向服务器发送断开连接
                    clientSocket.sendall('QUIT\r\n'.encode())

                    clientSocket.close()

                    textvar.set("Email sent successfully")

            # 成功发送后，更新联系人名单并保存
            for _ in toAddress:
                if _ != "" and _ not in contacts:
                    contacts.append(_)
            with open('contacts.pkl', 'wb') as f:
                pickle.dump(contacts, f)

            # 保存到已发送邮件中
            history.append([account.get(), password.get(), receiver.get(), subject.get(), msgbody.get('1.0', 'end')])
            with open('history.pkl', 'wb') as f:
                pickle.dump(history, f)

            # 如果当前邮件存在草稿，则将其删除
            if os.path.exists('draft.pkl'):
                os.remove('draft.pkl')

        except Exception as e:
            textvar.set(str(e))

        tk_send.destroy()

    Button(tk_send, text="Send", command=contact_send).grid(column=0, row=0, sticky=W)
    Button(tk_send, text="Cancel", command=send_close).grid(column=1, row=0, sticky=E)


# 增加按钮用于发送邮件
ttk.Button(mainframe, text="Send Email", command=sendemail).grid(column=4, row=8, sticky=E)

for child in mainframe.winfo_children(): child.grid_configure(padx=5, pady=5)

account_entry.focus()

root.mainloop()
