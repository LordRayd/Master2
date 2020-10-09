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
from elasticsearch import Elasticsearch

# ------
# uptime
# -----

#uptime = check_output(['uptime'])
print("\n")
print('-------------------------------------------------------------')
#print(uptime.strip())
print('-------------------------------------------------------------')
print("\n")

class Item_RSS:
    """
    Représente un item Rss obtenu depuis le flux 
    """
    id = None
    """"
    
    """
    title = None
    """"
    
    """
    summary = None
    """"
    
    """
    description = None
    """"
    
    """
    all_links = None
    """"
    
    """
    source_post = None
    """"
    
    """
    source_feed = None
    """"
    L url de la source du flux
    """
    local_url = None
    """"
    l url du fichier local contenant la page de l'item Rss
    """
    lang = None
    """
    La langue utilisé dans le texte de l'item Rss
    """
    date = None
    """"
    La date de l'item Rss
    """
    target_data = None
    """"
    Le contenu de la page source de l'item Rss
    """

    def initWithPost(self, post, feed, database_name='database'):
        """
        Initialise l item rss a partir des données récupérés depuis le flux

        Paramètres:
            post : L'item Rss recupéré depuis le flux
            feed : Les elements decrivants le flux
        """
        if  hasattr(post, 'title'):
            self.tile = post.title
            self.lang = langdetect.detect(post.title)
        if  hasattr(post, 'summary'):
            self.summary = post.summary
        if  hasattr(post, 'description'):
            self.description = post.description
        if hasattr(post, 'links'):
            self.all_links = post.links
        if hasattr(feed, 'link'):
            self.source_feed = feed.link
        self.integrity()
        if  hasattr(post, 'link'):
            self.source_post = post.link
            self.id = hashlib.sha224(post.link.encode(encoding='UTF-8')).hexdigest()
            try:
                self.local_url = './pages/' + post.link.replace('/','').replace(':','')
                html = urllib.request.urlopen(post.link)
                soup = BeautifulSoup(html, features="html.parser")
                self.target_data = str(soup.prettify())
                if self.checkIntegrity(database_name) == False :
                    self.write_target_data_In_File()
            except urllib.error.HTTPError as e:
                self.target_data = None
                self.local_url = None
            except urllib.error.URLError as e:
                self.target_data = None
                self.local_url = None

    def affichage(self):
        """
        Affiche tous les éléments de l'item Rss si ils ne sont pas vides
        """
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

    def write_target_data_In_File(self):
        """
        Ecrit dans un fichier contenu à l'adresse local la page web qui est source de l'item Rss
        """
        f = open(self.local_url, "w", encoding="utf-8")
        f.write(self.target_data)
        f.close()

    def integrity(self):
        """
        Calcul le hash qui déterminera si un element à changé au court du temps
        """
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
        """"
        Sauvegarde l'item Rss dans la base de données

        Paramètres :
            database_name : Le nom de la base de données dans laquelle sauvegarder l'item
        """
        d = shelve.open(database_name, 'c')
        if d.__contains__(self.id) == False:
            d[self.id] = self
        else : 
            if self.integrity != d[self.id].integrity :
                d[self.id] = self
        d.close()

    def checkIntegrity(self, database_name):
        """"
        Permet la vérification de l'exactitude des informations précédement enregistrer par rapport au données actuels

        Paramètres :
            database_name : Le nom de la base de données utilisé pour faire la vérification de l'intégrité
        Retour :
            Retourne Vrai si les données n'ont pas changé et Faux autrement
        """
        ret = True
        d = shelve.open(database_name, 'c')
        if d.__contains__(self.id) == False:
            ret = False
        else : 
            if self.integrity != d[self.id].integrity :
                ret = False
        d.close()
        return ret
    

# --------------------
# CNN Collector (feedparser)
# --------------------
es = Elasticsearch()
print(es.info())
d = feedparser.parse("http://rss.cnn.com/rss/edition.rss")

# print all posts
count = 1
blockcount = 1
print('size = ', len(d.entries))
for post in d.entries:
    if count % 5 == 1:
        print("\n" + time.strftime("%a, %b %d %I:%M %p") + '  ((( CNN - ' + str(blockcount) + ' )))')
        print("-----------------------------------------\n")
        blockcount += 1
    elem = Item_RSS()
    elem.initWithPost(post,d.feed)
    elem.save('database')
    count += 1