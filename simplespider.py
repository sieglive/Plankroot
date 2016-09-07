# -*- coding: utf-8 -*-
from urllib.request import urlopen
from bs4 import BeautifulSoup
import re
import os
import time

categaries = {}
person = {}
items = {}
log = []
PATH = os.getcwd()
forbidchar = r'<|>|/|\\|\||:|"|\*|\?'

def getperson(link,):
    categaries_cp = categaries.copy
    for i in range(10):
        time.sleep(1)
        try:
            html = urlopen(link, timeout=100)
            break
        except Exception:
            time.sleep(2)
            print("Url Erroe")

    for i in range(10):
        try:
            bsObj = BeautifulSoup(html, "html.parser")
            break
        except Exception:
            print("Soup Error")

    for boxs in bsObj.findAll("div", {"class":"boxs"}):
        for li in boxs.findAll("li"):
            try:
                for a in li.findAll('p'):
                    print(a.getText())
                    name = re.search(re.compile(r'^模特：(.*?)$'), a.getText())
                    if name:
                        psn = a.find('a')
                        person[psn.getText()] = psn.attrs['href']
            except:
                print("downloading item..."+name.group(1))
                item = li.find('p', {"class":"p_title"}).find("a")
                print(item.getText())
                if not os.path.exists(name.group(1)):
                    os.mkdir(name.group(1))
                os.chdir(name.group(1))
                print(name.group(1))
                name = item.getText()
                name = re.split(forbidchar, name)
                name = '.'.join(name)
                if not os.path.exists(name):
                    os.mkdir(name)
                os.chdir(name)
                downloaditem(item.attrs['href'])
                os.chdir(PATH)


    time.sleep(3)
    page = bsObj.find("div", {"id":"pages"})
    for a in page.findAll("a", {"class":"a1"}):
        if re.search(re.compile(r'下一页'), a.getText()):
            number = re.search(re.compile(r"http://www\.meitulu\.com/t/.*?([0-9]*?)\.html"), a.attrs['href'])
            link = number.group(0)
            number = number.group(1)
            if number != page.find('span').getText():
                print("scrap deeper...")
                getperson(link)
                break

def downloadperson(link, name):
    name = re.split(forbidchar, name)
    name = '.'.join(name)
    personitems = {}

    if not os.path.exists(name):
        os.mkdir(name)
    os.chdir(name)

    for i in range(10):
        time.sleep(1)
        try:
            html = urlopen(link, timeout=100)
            break
        except Exception:
            time.sleep(2)
            print("Url Erroe")

    for i in range(10):
        try:
            bsObj = BeautifulSoup(html, "html.parser")
            break
        except Exception:
            print("Soup Error")

    for boxs in bsObj.findAll("div", {"class":"boxs"}):
        for li in boxs.findAll("li"):
            try:
                for p in li.findAll('p', {"class":"p_title"}):
                    print('\n',p,'\n')
                    psn = p.find('a')
                    personitems[psn.getText()] = psn.attrs['href']
            except:
                print("Find Error")

    PATHtmp = os.getcwd()
    for key in personitems:
        print('\n', "downloading ", key, '\n')
        if not os.path.exists(key):
            os.mkdir(key)
        os.chdir(key)
        downloaditem(personitems[key])
        os.chdir(PATHtmp)

    os.chdir(PATH)

def downloaditem(link, ):
    for i in range(10):
        time.sleep(1)
        try:
            html = urlopen(link, timeout=100)
            break
        except Exception:
            print("Url Erroe")
            time.sleep(2)
    else:
        return False

    for i in range(10):
        try:
            bsObj = BeautifulSoup(html, "html.parser")
            break
        except Exception:
            print("Soup Error")
    else:
        return False


    for center in bsObj.findAll("center"):
        for img in center.findAll("img"):
            boola = downloadimg(img.attrs['src'], img.attrs['alt'])

    time.sleep(2)
    page = bsObj.find("div", {"id":"pages"})
    for a in page.findAll("a", {"class":"a1"}):
        if re.search(re.compile(r'下一页'), a.getText()):
            number = re.search(re.compile(r"http://www\.meitulu\.com/.*?_([0-9]*?)\.html"), a.attrs['href'])
            if number:
                link = number.group(0)
                number = number.group(1)
                if number != page.find('span').getText():
                    print("download deeper...")
                    downloaditem(link)
    return True

def downloadimg(link, name):
    name = re.split(forbidchar, name)
    name = '.'.join(name)
    for i in range(10):
        time.sleep(0.5)
        try:
            data = urlopen(link, timeout=10)
            tname = name+".jpg"
            with open(tname, "ab") as code:
                code.write(data.read())
            print(tname+" is done.")
            break
        except Exception:
            time.sleep(3)
    else:
        return False
    return True

if __name__ == "__main__":
    for i in range(10):
        time.sleep(1)
        try:
            html = urlopen("http://www.meitulu.com", timeout=100)
            break
        except Exception:
            print("Url Erroe")
            time.sleep(2)
    else:
        return False

    for i in range(10):
        try:
            bsObj = BeautifulSoup(html, "html.parser")
            break
        except Exception:
            print("Soup Error")
    else:
        return False

    for a in bsObj.find("li", {"id":"tag"}).find("ul", {"id":"tag_ul"}).findAll("a"):
        categaries[a.getText()] = a.attrs['href']
    i = 0
    for key in categaries:
        i+=1
        time.sleep(3)
        print(i,"loading page..."+key)
        getperson(categaries[key])

    # print(person)
    i = 0
    for key in person:
        i+=1
        downloadperson(person[key], key)