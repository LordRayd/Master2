from elasticsearch import Elasticsearch
from src.item_rss import Item_RSS
import numpy as np

class ElasticTool:
    """
    Attributs:
    _es : La connexion a la base ElasticSearch
    """
    _es = None
    
    def __init__(self):
        """
        Initialiseur de la class ElasticTool. Fait la connexion avec ElasticSearch et ajoute tous les index necessaires.
        """
        self._es = self.getConnection()
        self.add_all_index()

    def getConnection(self, _host = 'localhost', _port=9200):
        """
        Etablit une connexion avec ElasticSearch

        Paramètres:
        _host='localhost' : l'adresse ou se trouve ElasticSearch
        _port=9200 : Le port sur lequel on accede à ElasticSearch

        Retourne la connexion
        """
        if self._es == None :
            self._es = Elasticsearch([{'host': _host, 'port': _port}])
        return self._es

    def affichage_etat(self):
        """
        Affiche l'état d'ElasticSearch a savoir si il est lancé ou non
        """
        if self._es.ping() :
            print('ElasticSearch Tourne')
        else :
            print('ElasticSearch ne tourne pas')

    def add_index(self, name_index):
        """
        Ajoute un index à ElasticSearch
        
        Paramètres:
        name_index : Le nom de l'index
        """
        if self._es.indices.exists(index=name_index):
            self._es.indices.create(index=name_index, ignore=400)

    def add_all_index(self):
        """
        Ajoute tous les index nécessaires au fonctionnement des modules
        """
        self.add_index("item_rss")
        self.add_index("title")
        self.add_index("summary")
        self.add_index("description")
        self.add_index("links")
        self.add_index("source_post")
        self.add_index("lang")
        self.add_index("date")
        self.add_index("target_data")
        self.add_index('integrity')
        self.add_index('type_flux')
        self.add_index('type_predit')

    def delete_index(self, name_index):
        """
        Supprime un index d'elasticSearch

        Paramètres:
        name_index : Le nom de l'index a supprimé
        """
        self._es.indices.delete(index=name_index, ignore=[400, 404])

    def delete_all_index(self):
        """
        Supprime tous les index nécessaire au fonctionnement des modules
        """
        self.delete_index("item_rss")
        self.delete_index("title")
        self.delete_index("summary")
        self.delete_index("description")
        self.delete_index("links")
        self.delete_index("source_post")
        self.delete_index("lang")
        self.delete_index("date")
        self.delete_index("target_data")
        self.delete_index('integrity')
        self.delete_index('type_flux')
        self.delete_index('type_predit')

    def insertion_all_items(self, _items):
        """
        Insère tous les éléments dans ElasticSearch

        Paramètres:
        _items : La list des Item_RSS à ajouter
        """
        for i in _items :
            self.insertion_item(i)

    def insertion_item(self, _item):
        if self.verification_integrity(_item.id, _item.integrity) == False :
            id_title = self.save_title(_item)
            id_summary = self.save_summary(_item)
            id_description = self.save_description(_item)
            id_all_links = self.save_all_links(_item)
            id_source_post = self.save_source_post(_item)
            id_lang = self.save_lang(_item)
            id_date = self.save_date(_item)
            id_target_data = self.save_target_data(_item)
            id_integrity = self.save_integrity(_item)
            id_type_flux = self.save_type_flux(_item)
            id_type_predit = self.save_type_predit(_item)
            self.save_item(_item.id, id_title, id_summary, id_description, id_all_links, id_source_post, id_lang, id_date, id_target_data, id_integrity, id_type_flux, id_type_predit)

    def save_item(self, _id, _title, _summary, _description, _all_links, _source_post, _lang, _date, _target_data, _integrity, _type_flux, _type_predit ):
        content_body = {}
        if _title != False :
            content_body['id_title'] = _title
        if _summary != False :
            content_body['id_summary'] = _summary
        if _description != False :
            content_body['id_description'] = _description
        if _all_links != False :
            content_body['id_all_links'] = _all_links
        if _source_post != False :
            content_body['id_source_post'] = _source_post
        if _lang != False :
            content_body['id_lang'] = _lang
        if _date != False :
            content_body['id_date'] = _date
        if _target_data != False :
            content_body['id_target_data'] = _target_data
        if _integrity != False :
            content_body['id_integrity'] = _integrity
        if _type_flux != False :
            content_body['type_flux'] = _type_flux
        if _type_predit != False :
            content_body['type_predit'] = _type_predit
        self._es.index(index='item_rss', id=_id, body=content_body)

    def save_title(self, _item) : 
        if(_item.title != None) : 
            content_body = {
                'value' : _item.title,
                'tags' : _item.title.split(' '),
                'id_item' : _item.id
            }
            return self._es.index(index="title", body=content_body)['_id']
        return False

    def save_summary(self, _item):
        if _item.summary != None :
            content_body = {
                'value' : _item.summary,
                'tags' : _item.summary.split(' '),
                'id_item' : _item.id
            }
            return self._es.index(index="summary", body=content_body)['_id']
        return False

    def save_description(self, _item):
        if _item.description != None :
            content_body = {
                'value' : _item.description,
                'tags' : _item.description.split(' '),
                'id_item' : _item.id
            }
            return self._es.index(index="description", body=content_body)['_id']
        return False

    def save_link(self, _link, _id):
        content_body = {
            'value' : _link,
            'id_item' : _id
        }
        return self._es.index(index="links", body=content_body)['_id']

    def save_all_links(self,_item):
        id_tab_links = []
        if(_item.all_links != None):
            for l in _item.all_links:
                id_tab_links.append(self.save_link(l, _item.id))
        return id_tab_links if len(id_tab_links)>0 else False

    def save_source_post(self, _item):
        if _item.source_post != None :
            content_body = {
                'value' : _item.source_post,
                'id_item' : _item.id
            }
            return self._es.index(index="source_post", body=content_body)['_id']
        return False

    def save_lang(self, _item):
        if _item.source_post != None :
            content_body = {
                'value' : _item.lang,
                'id_item' : _item.id
            }
            return self._es.index(index="lang", body=content_body)['_id']
        return False

    def save_date(self, _item):
        if _item.date != None :
            content_body = {
                'value' : _item.date,
                'id_item' : _item.id
            }
            return self._es.index(index="date", body=content_body)['_id']
        return False

    def save_target_data(self, _item):
        if _item.target_data != None :
            content_body = {
                'value' : _item.target_data,
                'tags' : _item.target_data.split(' '),
                'id_item' : _item.id
            }
            return self._es.index(index="target_data", body=content_body)['_id']
        return False

    def save_integrity(self, _item):
        if _item.integrity != None :
            content_body = {
                'value' : _item.integrity,
                'id_item' : _item.id
            }
            return self._es.index(index="integrity", body=content_body)['_id']
        return False
        
    def save_type_flux(self, _item):
        if _item.type_flux != None :
            type_ = {}
            for i in range(len(_item.type_flux)):
                type_[str(i)] = _item.type_flux[i]
            content_body = {
                'value' : type_,
                'id_item' : _item.id
            }
            return self._es.index(index="type_flux", body=content_body)['_id']
        return False

    def save_type_predit(self, _item):
        if _item.type_flux != None :
            content_body = {
                'value' : _item.type_predit,
                'id_item' : _item.id
            }
            return self._es.index(index="type_predit", body=content_body)['_id']
        return False

    def search_by_tags(self, index_name, tags, size_result=999):
        
        str_query = ''
        maxi = len(tags)
        for i in range(maxi):
            str_query += 'tags:' + '*'+tags[i]+'*'
            if i < maxi-1:
                str_query += ' AND '
                
        query_body ={
            "query": {
                "query_string": {
                    "query" : str_query
                }
            }
        }
        return self._es.search(index=index_name, body=query_body, size=size_result)['hits']['hits']

    def verification_integrity(self, id_, integrity_):
        res = self._es.get(index="integrity", id=id_, ignore=[400,404])
        return False

    def get_all_from_index(self, index_name, size_result=999):
        query_body = {
            'size' : 100,
            'query': {
                'match_all' : {}
            }
        }
        return self._es.search(index=index_name, body=query_body, size=size_result)['hits']['hits']




#================================== Version avec tout dans 1 index ===============================
class ElasticToolv0 :

    """
    Outil de sauvegarde dans une base ElasticSearch avec tout dans 1 index
    Attributs:
    _es : La connexion a la base ElasticSearch
    """
    _es = None
    
    def __init__(self):
        """
        Initialiseur de la class ElasticTool. Fait la connexion avec ElasticSearch et ajoute tous les index necessaires.
        """
        self._es = self.getConnection()
        self.add_index()

    def getConnection(self, _host = 'localhost', _port=9200):
        """
        Etablit une connexion avec ElasticSearch

        Paramètres:
        _host='localhost' : l'adresse ou se trouve ElasticSearch
        _port=9200 : Le port sur lequel on accede à ElasticSearch

        Retourne la connexion
        """
        if self._es == None :
            self._es = Elasticsearch([{'host': _host, 'port': _port}])
        return self._es

    def affichage_etat(self):
        """
        Affiche l'état d'ElasticSearch a savoir si il est lancé ou non
        """
        if self._es.ping() :
            print('ElasticSearch Tourne')
        else :
            print('ElasticSearch ne tourne pas')

    def add_index(self):
        """
        Ajoute l index nécessaire au fonctionnement des modules
        """
        if self._es.indices.exists(index='item'):
            self._es.indices.create(index='item', ignore=400)

    def delete_index(self):
        """
        Supprime l index nécessaire au fonctionnement des modules
        """
        self._es.indices.delete(index='item', ignore=[400, 404])

    def insertion_item(self, _item) :
        """
        Insère l item Rss dans ElasticSearch

        Paramètres:
        _items : L objet Item_RSS à ajouter
        """
        content_body = {
            'title' : _item.title,
            'summary' : _item.summary,
            'description' : _item.description,
            'all_links' : _item.all_links,
            'source_post' : _item.source_post,
            'source_feed' : _item.source_feed,
            'lang' : _item.lang,
            'date' : _item.date,
            'target_data' :  _item.target_data,
            'type_flux' : _item.type_flux,
            'type_predit' : _item.type_predit #,
            #'tags' : np.append(_item.description.split(' '), _item.summary.split(' '),_item.title.split(' '), _item.target_data.split(' '))
        }
        return self._es.index(index='item', id=_item.id, body=content_body)

    def search_by_tags(self, tags, size_result=999):
        """
        Plus utilisé mais a la base il servait a faire une recherche par mot dans le texte de la page html
        pour utilisé il faudrait décommenter la ligne dans insertion_item prevut a cet effet et la modifier un peu
        
        Paramètres:
        tags : Un tableau ddes differents tags a chercher
        size_result : Le nombre d'item max que l'on peut renvoyer
        """
        str_query = ''
        maxi = len(tags)
        for i in range(maxi):
            str_query += 'tags:' + '*'+tags[i]+'*'
            if i < maxi-1:
                str_query += ' AND '
                
        query_body ={
            "query": {
                "query_string": {
                    "query" : str_query
                }
            }
        }
        return self._es.search(index='item', body=query_body, size=size_result)['hits']['hits']

    def get_all_element_lang(self, lang='fr', size_result=9999):
        """
        Retourne tous les éléments d'une langue donné 

        Paramètres:
        lang : La langue dont on doit trouver les éléments
        size_result : Le nombre d'item max que l'on peut renvoyer
        """
        str_query = 'lang:' + lang
                
        query_body ={
            "query": {
                "query_string": {
                    "query" : str_query
                }
            }
        }
        results = self._es.search(index='item', body=query_body, size=size_result)['hits']['hits']
        list_item = []
        for result in results:
            data = result['_source']
            item = Item_RSS()
            item.create_from_tool(id_ = result['_id'], title_ = data['title'], summary_ = data['summary'], description_ = data['description'], all_links_ = data['all_links'], source_post_ = data['source_post'], source_feed_ = data['source_feed'], lang_ = data['lang'], date_ = data['date'], target_data_ = data['target_data'], type_flux_ = data['type_flux'])
            list_item.append(item)
        return list_item
        #return self._es.search(index='item', body=query_body, size=size_result)['hits']['hits']
    
    def search(self, str_query, size_result=999):
        """
        Retourne tous les éléments d'une langue donné 

        Paramètres:
        str_query : La requete à éxécuter sur ElasticSearch
        size_result : Le nombre d'item max que l'on peut renvoyer

        Retourne une liste d'item rss
        """
        query_body = {
            "query": {
                "query_string": {
                    "query": str_query
                }
            }
        }
        results = self._es.search(index='item', body=query_body, size=size_result)['hits']['hits']
        list_item = []
        for result in results:
            data = result['_source']
            item = Item_RSS()
            item.create_from_tool(id_ = result['_id'], title_ = data['title'], summary_ = data['summary'], description_ = data['description'], all_links_ = data['all_links'], source_post_ = data['source_post'], source_feed_ = data['source_feed'], lang_ = data['lang'], date_ = data['date'], target_data_ = data['target_data'], type_flux_ = data['type_flux'])
            list_item.append(item)
        return list_item