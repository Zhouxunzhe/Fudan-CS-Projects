from socket import *
from email.base64mime import body_encode
import ssl


msg = "\r\n I love computer networks!"
endMsg = "\r\n.\r\n"
# 选择一个邮件服务
mailServer = "smtp.qq.com"
# 发送方地址和接收方地址，from 和 to
fromAddress = "1170543554@qq.com"
toAddress = "1170543554@qq.com"
# 发送方，验证信息，由于邮箱输入信息会使用base64编码，因此需要进行编码
username = "1170543554@qq.com"  # 输入自己的用户名对应的编码
password = "rjqrtlsxdlyahiec"  # 此处不是自己的密码，而是开启SMTP服务时对应的授权码


# 创建客户端套接字并建立连接
serverPort = 465  # SSL使用465号端口

# 生成SSL上下文
context = ssl.SSLContext(ssl.PROTOCOL_TLS)
# context = ssl.create_default_context()
# 加载信任根证书
# context.load_verify_locations('C:\\Users\\ZhouXunZhe\\Desktop\\key\\ca.crt')

# 建立连接并打包为tls socket
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((mailServer, serverPort))  # connect只能接收一个参数
clientSocket = context.wrap_socket(clientSocket, server_hostname=mailServer)

# 从客户套接字中接收信息
recv = clientSocket.recv(1024).decode()
print(recv)
if '220' != recv[:3]:
    print('220 reply not received from server.')

# 发送 HELO 命令并且打印服务端回复
# 开始与服务器的交互，服务器将返回状态码250,说明请求动作正确完成
heloCommand = 'HELO zhouxunzhe\r\n'
clientSocket.send(heloCommand.encode())  # 随时注意对信息编码和解码
recvHelo = clientSocket.recv(1024).decode()
print(recvHelo)
if '250' != recvHelo[:3]:
    print('250 reply not received from server.')

# 发送"AUTH PLAIN"命令，验证身份.服务器将返回状态码334（服务器等待用户输入验证信息）
user_pass_encode64 = body_encode(f"\0{username}\0{password}".encode('ascii'), eol='')
clientSocket.sendall(f'AUTH PLAIN {user_pass_encode64}\r\n'.encode())
recvAuth = clientSocket.recv(1024).decode()
print(recvAuth)
if '235' != recvAuth[:3]:
    print('235 reply not received from server')

# # 发送"VRFY"命令, 需要对收件人名字进行验证
# clientSocket.sendall(('VRFY: ' + username + '\r\n').encode())
# recvVrfy = clientSocket.recv(1024).decode()
# print(recvVrfy)
# if '250' != recvVrfy[:3]:
#     print('250 reply not received from server')

# 发送"MAIL FROM"命令, 向服务器发送 邮件发送地址
fromCommand = 'MAIL FROM: <' + fromAddress + '>\r\n'
clientSocket.sendall(fromCommand.encode())
recvFrom = clientSocket.recv(1024).decode()
print(recvFrom)
if '250' != recvFrom[:3]:
    print('250 reply not received from server')

# 发送"RCPT TO"命令, 向服务器发送 邮件目的地
rcptCommand = 'RCPT TO: <' + toAddress + '>\r\n'
clientSocket.sendall(rcptCommand.encode())
recvTo = clientSocket.recv(1024).decode()
print(recvTo)
if '250' != recvTo[:3]:
    print('250 reply not received from server')

# 发送"DATA"命令, 向服务器发送 数据址
clientSocket.sendall('DATA\r\n'.encode())
recvData = clientSocket.recv(1024).decode()
print(recvData)
if '354' != recvData[:3]:
    print('354 reply not received from server')

# 编辑邮件信息，发送数据
subject = "mail from computer networks"
contentType = "text/plain"

message = 'from:' + fromAddress + '\r\n'
message += 'to:' + toAddress + '\r\n'
message += 'subject:' + subject + '\r\n'
message += 'Content-Type:' + contentType + '\t\n'
message += '\r\n' + msg
clientSocket.sendall(message.encode())

# 以"."结束。请求成功返回 250
clientSocket.sendall(endMsg.encode())
recvEnd = clientSocket.recv(1024).decode()
print(recvEnd)
if '250' != recvEnd[:3]:
    print('250 reply not received from server')

# 发送"QUIT"命令, 向服务器发送断开连接
clientSocket.sendall('QUIT\r\n'.encode())

clientSocket.close()
