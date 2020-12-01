from elasticsearch import Elasticsearch
import numpy as np

class ElasticTool:

    _es = None
    
    def __init__(self):
        self._es = self.getConnection()
        self.add_all_index()

    def getConnection(self, _host = 'localhost', _port=9200):
        if self._es == None :
            self._es = Elasticsearch([{'host': _host, 'port': _port}])
        return self._es

    def affichage_etat(self):
        if self._es.ping() :
            print('ElasticSearch Tourne')
        else :
            print('ElasticSearch ne tourne pas')

    def add_index(self, name_index):
        if self._es.indices.exists(index=name_index):
            self._es.indices.create(index=name_index, ignore=400)

    def add_all_index(self):
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
        self._es.indices.delete(index=name_index, ignore=[400, 404])

    def delete_all_index(self):
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

    _es = None
    
    def __init__(self):
        self._es = self.getConnection()
        self.add_index()

    def getConnection(self, _host = 'localhost', _port=9200):
        if self._es == None :
            self._es = Elasticsearch([{'host': _host, 'port': _port}])
        return self._es

    def affichage_etat(self):
        if self._es.ping() :
            print('ElasticSearch Tourne')
        else :
            print('ElasticSearch ne tourne pas')

    def add_index(self):
        if self._es.indices.exists(index='item'):
            self._es.indices.create(index='item', ignore=400)

    def delete_index(self):
        self._es.indices.delete(index='item', ignore=[400, 404])

    def insertion_item(self, _item) :
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
            'type_predit' : _item.type_predit,
            'tags' : np.append(_item.description.split(' '), _item.summary.split(' '),_item.title.split(' '), _item.target_data.split(' '))
        }
        return self._es.index(index='item', id=_item.id, body=content_body)

    def search_by_tags(self, tags, size_result=999):
        
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