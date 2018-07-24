#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: SendMail.py 
@time: 2017/11/28 19:16 
@describe: 
"""

import datetime
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.header import Header

mail_host = "smtp.163.com"
mail_user = "scuxia"
mail_pass = "mail163"

sender = 'scuxia@163.com'
receivers = ['173795499@qq.com', '3263548205@qq.com', '215112048@qq.com']  # 接收邮件，可设置为你的QQ邮箱或者其他邮箱

message = MIMEMultipart()
message['From'] = sender
message['To'] = Header("Position Report", 'utf-8')
subject = 'Python SMTP 邮件测试'
message['Subject'] = Header(subject, 'utf-8')

now = datetime.datetime.now()
str_date = now.strftime('%Y-%m-%d')
message.attach(MIMEText(str_date+' Position Report:', 'plain', 'utf-8'))

# 构造附件1，传送当前目录下的 test.txt 文件
att1 = MIMEText(open('long.csv', 'rb').read(), 'base64', 'utf-8')
att1["Content-Type"] = 'application/octet-stream'
# 这里的filename可以任意写，写什么名字，邮件中显示什么名字
att1["Content-Disposition"] = 'attachment; filename="long.csv"'
message.attach(att1)

try:
    smtpObj = smtplib.SMTP()
    smtpObj.connect(mail_host, 25)  # 25 为 SMTP 端口号
    smtpObj.login(mail_user, mail_pass)
    smtpObj.sendmail(sender, receivers, message.as_string())
    print "邮件发送成功"
except smtplib.SMTPException, e:
    print str(e)
    print "Error: 无法发送邮件"

