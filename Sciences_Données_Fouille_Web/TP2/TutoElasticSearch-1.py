
# coding: utf-8

# ## Tutoriel Elastic Search

# In[46]:


#!/usr/bin/python3
#
from elasticsearch import Elasticsearch


# In[47]:


# make sure ES is up and running
import requests
res = requests.get('http://localhost:9200')
print(res.content)


# In[48]:


es = Elasticsearch([{'host': 'localhost', 'port': 9200}])


# In[49]:


# index some stuff
doc1={'title':'Tribune : le cri d’alarme de quinze mille scientifiques sur l’état de la planète', 
         'content':'Tribune. Il y a vingt-cinq ans, en 1992, l’Union of Concerned Scientists et plus de 1 700 scientifiques indépendants, dont la majorité des lauréats de prix Nobel de sciences alors en vie, signaient le « World Scientists’Warning to Humanity ». Ces scientifiques exhortaient l’humanité à freiner la destruction de l’environnement et avertissaient : « Si nous voulons éviter de grandes misères humaines, il est indispensable d’opérer un changement profond dans notre gestion de la Terre et de la vie qu’elle recèle. » Dans leur manifeste, les signataires montraient que les êtres humains se trouvaient sur une trajectoire de collision avec le monde naturel. Ils faisaient part de leur inquiétude sur les dégâts actuels, imminents ou potentiels, causés à la planète Terre, parmi lesquels la diminution de la couche d’ozone, la raréfaction de l’eau douce, le dépérissement de la vie marine, les zones mortes des océans, la déforestation, la destruction de la biodiversité, le changement climatique et la croissance continue de la population humaine. Ils affirmaient qu’il fallait procéder d’urgence à des changements fondamentaux afin d’éviter les conséquences qu’aurait fatalement la poursuite de notre comportement actuel.'}
es.index(index='rssi', doc_type='item-rss', id=0, body=doc1)


# In[50]:


doc2={'title':'En Espagne, le Tage se meurt',
     'content':'Le plus long fleuve de la péninsule ibérique agonise, victime de la sécheresse et des transferts d’eau'}
es.index(index='rssi', doc_type='item-rss', id=1, body=doc2)


# In[51]:


#get back a doc from its id
es.get(index='rssi', doc_type='item-rss', id=0)


# In[53]:


query = input("Enter a search: ")


# In[54]:


result = es.search(index='rssi', doc_type='item-rss', body={"query": {"match": {"title": query.strip()}}})


# In[ ]:


if result.get('hits') is not None and result['hits'].get('hits') is not None:
    print(result['hits']['hits'])
else:
    print({})


# In[55]:


result = es.search(index='rssi', doc_type='item-rss', body={"query": {"match": {"content": query.strip()}}})


# In[56]:


if result.get('hits') is not None and result['hits'].get('hits') is not None:
    print(result['hits']['hits'])
else:
    print({})

