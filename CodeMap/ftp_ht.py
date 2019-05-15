from ftplib import FTP
import time

ftp = FTP("demo.wftpserver.com")

ftp.login(user="demo-user", passwd="demo-user")

ftp.cwd("upload")

while True:

    ftp.retrbinary("RETR coor_gps", open("coor_gps", "wb").write)
    time.sleep(5)

ftp.quit()
