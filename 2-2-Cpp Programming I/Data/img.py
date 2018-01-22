#coding=utf-8
import requests
from bs4 import BeautifulSoup
from time import sleep
from time import clock
import re
import random
import csv


with open('doubanUTF8.txt', 'r', encoding='utf-8') as f:
    reader = csv.reader(f)
    pre = 'C:\\Users\\Xiao Yunming\\Documents\\eclipse\\Douban\\img\\'
    for row in reader:
        u = row[11]
        temp = u.split('/')[-1]
        if temp == 'update_image':
            pass
        else:
            print(u)
            filename = pre + temp
            im = requests.get(u)
            with open(filename, 'wb') as newfile:
                newfile.write(im.content)
