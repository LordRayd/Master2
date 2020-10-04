#!/usr/bin/python3

import hashlib
import shelve
import time
import urllib.request
from subprocess import check_output

import feedparser
import langdetect
import chardet
from bs4 import BeautifulSoup

# ------
# uptime
# -----

#uptime = check_output(['uptime'])
print("\n")
print('-------------------------------------------------------------')
#print(uptime.strip())
print('-------------------------------------------------------------')
print("\n")

class Feed_Element:
    id = None
    title = None
    summary = None
    description = None
    all_links = None
    source_post = None
    source_feed = None
    local_url = None
    lang = None
    date = None
    target_data = None

    def initWithPost(self, post, feed):

        if  hasattr(post, 'title'):
            self.tile = post.title
            self.lang = langdetect.detect(post.title)
        if  hasattr(post, 'summary'):
            self.summary = post.summary
        if  hasattr(post, 'description'):
            self.description = post.description
        if  hasattr(post, 'link'):
            self.source_post = post.link
            self.id = hashlib.sha224(post.link.encode(encoding='UTF-8')).hexdigest()
        if hasattr(post, 'links'):
            self.all_links = post.links
        try:
            self.local_url = './pages/' + post.link.replace('/','').replace(':','')
            html = urllib.request.urlopen(post.link)
            soup = BeautifulSoup(html, features="html.parser")
            f = open(self.local_url, "w", encoding="utf-8")
            self.target_data = str(soup.prettify())
            f.write(self.target_data)
            f.close()
            #html = urllib.request.urlopen(post.link)
            #soup = BeautifulSoup(html, features="html.parser", from_encoding="utf-8")
            #self.target_data = str(soup.prettify())
            #self.target_data = self.target_data.encode('utf-8')
            #print(self.target_data)
            #f = open(self.local_url, "w")
            #f.write(self.target_data)
            #f.close()
        except urllib.error.HTTPError as e:
            self.target_data = None
            self.local_url = None
        except urllib.error.URLError as e:
            self.target_data = None
            self.local_url = None
        if hasattr(feed, 'link'):
            self.source_feed = feed.link

    def affichage(self):
        if self.id != None:
            print('id : ', self.id, '\n')
        if self.title != None:
            print('title : ', self.title, '\n')
        if self.summary != None:
            print('summary : ', self.summary, '\n')
        if self.description != None:
            print('description : ', self.description, '\n')
        if self.source_post != None:
            print('source_post : ', self.source_post, '\n')
        if self.source_feed != None:
            print('source_feed : ', self.source_feed, '\n')
        if self.lang != None:
            print('lang : ', self.lang, '\n')
        if self.date != None:
            print('date : ', self.date, '\n')
        if self.target_data != None:
            print('target_data : ', self.target_data, '\n')
        
    def integrity(self):
        integrity = ''
        if self.title != None:
            integrity += self.title
        if self.summary != None:
            integrity += self.summary
        if self.description != None:
            integrity += self.description
        if self.target_data != None:
            integrity += self.target_data
        self.integrity = hashlib.sha224(integrity.encode(encoding='UTF-8')).hexdigest()

    def save(self, database_name):
        d = shelve.open(database_name, 'c')
        if d.__contains__(self.id) == False:
            d[self.id] = self
        else : 
            print('existe deja')
        d.close()
    

# --------------------
# CNN Collector (feedparser)
# --------------------

d = feedparser.parse("http://rss.cnn.com/rss/edition.rss")

# print all posts
count = 1
blockcount = 1
ident = 0
print('size = ', len(d.entries))
for post in d.entries:
    if count % 5 == 1:
        print("\n" + time.strftime("%a, %b %d %I:%M %p") + '  ((( CNN - ' + str(blockcount) + ' )))')
        print("-----------------------------------------\n")
        blockcount += 1
    elem = Feed_Element()
    elem.initWithPost(post,d.feed)
    if(ident == 0):
        ident = elem.id
        print(ident)
    #elem.affichage()
    #elem.save('database')
    count += 1
# id = titre + description + url source + url dist + text distant
