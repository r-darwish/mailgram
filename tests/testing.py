import sys
import socket

def main():
    sock = socket.socket()
    sock.connect((sys.argv[1], int(sys.argv[2])))
    sock.sendall("""EHLO testing\r
MAIL FROM:<from@somewhere.com>\r
RCPT TO:<to@somewhere.com>\r
DATA\r
Date: Wed, 07 Feb 2018 21:38:35 +0200\r
From: from@somewhere.com\r
To: to@somewhere.com\r
Subject: Test Notification\r
Priority: normal\r
MIME-Version: 1.1\r
Content-Type: text/plain; charset=us-ascii\r
\r
Success! You have properly configured your email notification settings\r
.\r
QUIT\r
""".encode("ASCII"))

if __name__ == '__main__':
    sys.exit(main())
