#coding=utf-8
import requests
from bs4 import BeautifulSoup
from time import sleep
from time import clock
import re
import random
import csv

url="https://book.douban.com/tag/?icn=index-nav"
wb_data=requests.get(url)               
soup=BeautifulSoup(wb_data.text,"lxml")  
tags=soup.select("#content > div > div.article > div > div > table > tbody > tr > td > a")
url = []
for tag in tags:    
    tag=tag.get_text()    
    helf="https://book.douban.com/tag/"   
    url.append(helf+str(tag))
#print(url)   
    
#-------------------------------------------------------------------------------------------

def mains(url):
    
    with open('data6.csv', 'a+', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
    
        wb_data = requests.get(url)
        soup = BeautifulSoup(wb_data.text.encode("utf-8"), "lxml")

        tag=url.split("?")[0].split("/")[-1] 
        
        book_url = soup.select("#subject_list > ul > li > div.info > h2 > a")
        for u in book_url:
            author = ''
            yizhe = ''
            publish = ''
            time = ''
            isbn = '0000000000000'
            paper = '0'
            orig_name = ''
            scoe = ''
            
            try:        
                subwb_data = requests.get(u['href'])
                s = BeautifulSoup(subwb_data.text.encode(encoding='utf_8'), "lxml")
            except:
                print('bad request')
                continue
            
            try:
                img = s.find_all('a', class_='nbg')[0]['href']
            except:
                img = ['']
                print('img lost')
                
            try:
                title = s.find_all('a', class_='nbg')[0]['title']
            except:
                title = ''
                print('title lost')
            
            try:    
                brief = s.find_all('div', class_='intro')[0].get_text()
                brief = brief[1:-6]
                brief = brief.replace('\n', ' ')
            except:
                brief = '暂无'
                print('brief lost')
            
            try:
                scoe = s.find_all('strong', class_='rating_num')[0].get_text()
            except:
                print('rating lost')
                scoe = '0.0'
                
            try:   
                info = s.find_all('div', class_='subject clearfix')[0].find_all('div', id='info')
                
                for a in info[0].find_all('span'):
                    if (a.get_text() == '出版社:'):
                        publish = a.next_sibling
                    elif (a.get_text() == '页数:'):
                        paper = a.next_sibling
                    elif (a.get_text() == 'ISBN:'):
                        isbn = a.next_sibling
                    elif (a.get_text() == '出版年:'):
                        time = a.next_sibling
                    elif (a.get_text() == '原作名:'):
                        orig_name = a.next_sibling
                    elif (a.get_text() == '译者:'):
                        temp = a.next_sibling.next_sibling
                        try:
                            yizhe = temp.get_text().split('\n')[1].strip() + temp.get_text().split('\n')[2].strip()
                        except:
                            try:
                                yizhe = temp.get_text().split('\n')[1].strip()
                            except:
                                print('yizhe lost')
                                print(temp)
                    elif (a.get_text() == '作者:'):
                        temp = a.next_sibling.next_sibling
                        try:
                            author = temp.get_text().split('\n')[1].strip() + temp.get_text().split('\n')[2].strip()
                        except:
                            author = temp.get_text().split('\n')[1].strip()
                
            except:
                print('somgthing wrong')
                continue
            
            l = []
            l.append((title, orig_name, publish, author, yizhe, scoe, tag, isbn, paper, time, img, brief))        
            print(l)
            writer.writerows(l)
            sleep(int(format(random.randint(1,2))))
        

#-------------------------------------------------------------------------------------------
'''
u = 'https://book.douban.com/tag/小说?start=80&type=T'
mains(u)
'''
start = clock()
del url[0:125]

'''
urll = [url[0] + "?start={}&type=T".format(str(i)) for i in range(460,980,20)]
for u in urll:
    print(u)
    mains(u)
    sleep(1)
    
del url[0]
'''

for ur in url: 
    urlss=[ur + "?start={}&type=T".format(str(i)) for i in range(0,980,20)] 
    for u in urlss:
        print(u)       
        mains(u)       
        sleep(int(format(random.randint(1,2)))) # avoding IP block
end = clock()
print('Time Usage:', end - start)

