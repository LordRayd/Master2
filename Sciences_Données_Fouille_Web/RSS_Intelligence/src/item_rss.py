import hashlib
import urllib.request
from datetime import datetime

import langdetect
from bs4 import BeautifulSoup

import lxml
from lxml.html.clean import Cleaner
import io
import os

class Item_RSS:
    """
    Représente un item Rss obtenu depuis le flux 

    source_feed : L url de la source du flux
    local_url : L url du fichier local contenant la page de l'item Rss
    lang : La langue utilisé dans le texte de l'item Rss
    date : La date de l'item Rss
    target_data : Le contenu de la page source de l'item Rss
    """
    id = None
    title = None
    summary = None
    description = None
    all_links = None
    source_post = None
    source_feed = None
    lang = None
    date = None
    target_data = None
    type_flux = None

    def __init__(self, post, feed, tool=None, type_flux=['default']):
        """
        Initialise l item rss a partir des données récupérés depuis le flux

        Paramètres:
            post : L'item Rss recupéré depuis le flux
            feed : Les elements decrivants le flux
        """
        self.tool = tool
        self.type_flux = type_flux
        self.type_predit = 'default'
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
        self.integrity_construct()
        if  hasattr(post, 'link'):
            self.source_post = post.link
            self.id = hashlib.sha224(post.link.encode(encoding='UTF-8')).hexdigest()
            self.set_target_data_from_url(post.link)
        self.date = datetime.now()

    def set_target_data_from_url(self, url):
        cleaner = Cleaner()
        cleaner.javascript = True # Enleve les balises script et leur contenu
        cleaner.style = True      # Enleve les balises style et leur contenu
        
        try:
            html = urllib.request.urlopen(url)
            soup = BeautifulSoup(html, features="html.parser")
            # ====== PLUS DE BALISE SCRIPT & STYLE ==========
            target_data = lxml.html.tostring(cleaner.clean_html(lxml.html.document_fromstring(str(soup.prettify())))) 

            s = BeautifulSoup(target_data, features='lxml')
            self.target_data = os.linesep.join([s.lstrip() for s in s.getText().splitlines() if s and s.strip() != ''])
        except urllib.error.HTTPError as e:
            self.target_data = None
        except urllib.error.URLError as e:
            self.target_data = None

    def affichage(self):
        """
        Affiche tous les éléments de l'item Rss si ils ne sont pas vides
        """
        self.print_id()
        self.print_title()
        self.print_summary()
        self.print_description()
        self.print_source_post()
        self.print_source_feed()
        self.print_lang()
        self.print_date()
        self.print_target_data()
        self.print_type_flux()

    def print_id(self):
        if self.id != None:
            print('id : ', self.id, '\n')
    def print_title(self) :
        if self.title != None:
            print('title : ', self.title, '\n')
    def print_summary(self) : 
        if self.summary != None:
            print('summary : ', self.summary, '\n')
    def print_description(self) : 
        if self.description != None:
            print('description : ', self.description, '\n')
    def print_source_post(self) : 
        if self.source_post != None:
            print('source_post : ', self.source_post, '\n')
    def print_source_feed(self) :
        if self.source_feed != None:
            print('source_feed : ', self.source_feed, '\n')
    def print_lang(self) : 
        if self.lang != None:
            print('lang : ', self.lang, '\n')
    def print_date(self) :
        if self.date != None:
            print('date : ', self.date, '\n')
    def print_target_data(self) : 
        if self.target_data != None:
            print('target_data : ', self.target_data, '\n')
    def print_type_flux(self):
        if self.type_flux != None :
            print('type : ', self.type_flux, '\n')
    def print_type_predit(self):
        if self.type_predit != None :
            print('type predit : ', self.type_predit, '\n')

    def integrity_construct(self):
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

    def save(self):
        if self.tool != None :
            self.tool.insertion_item(self)