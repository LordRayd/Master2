#!/usr/bin/python3

import feedparser
import langdetect
import time
from subprocess import check_output
import hashlib
import textract
import urllib.request
# ------
# uptime
# -----

uptime = check_output(['uptime'])
print("\n")
print('-------------------------------------------------------------')
print(uptime.strip())
print('-------------------------------------------------------------')
print("\n")

class Feed_Element:
    id = None
    title = None
    summary = None
    description = None
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
            self.id = hashlib.sha224(post.title.encode(encoding='UTF-8')).hexdigest()
        if  hasattr(post, 'summary'):
            self.summary = post.summary
        if  hasattr(post, 'description'):
            self.description = post.description
        if  hasattr(post, 'link'):
            self.source_post = post.link
        try:
            urllib.request.urlopen(post.link)
            self.local_url = './pages/' + post.link.replace('/','')
            urllib.request.urlretrieve(post.link,  self.local_url)
        except urllib.error.HTTPError as e:
            self.target_data = None
        except urllib.error.URLError as e:
            self.target_data = None
        else:
            self.target_data = textract.process(self.local_url, encoding='ascii')
        if hasattr(feed, 'link'):
            self.source_feed = feed.link

    def affichage(self):
        if self.id != None:
            print('id : ' + self.id + '\n')
        if self.title != None:
            print('title : ' + self.title + '\n')
        if self.summary != None:
            print('summary : ' + self.summary + '\n')
        if self.description != None:
            print('description : ' + self.description + '\n')
        if self.source_post != None:
            print('source_post : ' + self.source_post + '\n')
        if self.source_feed != None:
            print('source_feed : ' + self.source_feed + '\n')
        if self.lang != None:
            print('lang : ' + self.lang + '\n')
        if self.date != None:
            print('date : ' + self.date + '\n')
        if self.target_data != None:
            print('target_data : ' + self.target_data + '\n')
    

# --------------------
# CNN Collector (feedparser)
# --------------------

d = feedparser.parse("http://rss.cnn.com/rss/edition.rss")

# print all posts
count = 1
blockcount = 1
for post in d.entries:
    if count % 5 == 1:
        print("\n" + time.strftime("%a, %b %d %I:%M %p") + '  ((( CNN - ' + str(blockcount) + ' )))')
        print("-----------------------------------------\n")
        blockcount += 1
    elem = Feed_Element()
    elem.initWithPost(post,d.feed)
    #elem.affichage()
    count += 1
# id = titre + description + url source + url dist + text distant