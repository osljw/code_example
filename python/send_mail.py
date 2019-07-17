#-*- coding:utf-8 -*-

import smtplib
from email.header import Header
from email.utils import parseaddr, formataddr
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email.mime.image import MIMEImage
from email import encoders
import sys
import os
from datetime import *
import time


class SendMail:
    def __init__(self):
        self.mail_host="localhost"
        self.mail_sender="google@google.com"
        self.mail_receiver = ["google@google.com"]

    def send(self, msg):
        try:
            #smtp=smtplib.SMTP_SSL(self.mail_host, 465)
            #smtp.login(mailuser, self.mail_passwd)
            smtp=smtplib.SMTP(self.mail_host)
            smtp.sendmail(self.mail_sender, self.mail_receiver, msg.as_string())
            smtp.close()
        except smtplib.SMTPException as e:
            print(e)


if __name__ == '__main__':
    sm = SendMail()

    text = "<font color="#FF0000">hello world</font>"
    msg = MIMEText(text, _subtype="html", _charset="utf-8")
    # 邮件标题
    msg['Subject'] = Header("邮件标题", 'utf-8').encode()
    sm.send(msg)

