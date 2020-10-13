from datetime import datetime
from elasticsearch import Elasticsearch

# by default we connect to localhost:9200
es = Elasticsearch()

# create an index in elasticsearch, ignore status code 400 (index already exists)
es.indices.create(index='my-index', ignore=400)
{'acknowledged': True, 'shards_acknowledged': True, 'index': 'my-index'}

# datetimes will be serialized
es.index(index="my-index", id=42, body={"any": "data", "timestamp": datetime.now()})
{'_index': 'my-index',
 '_type': '_doc',
 '_id': '42',
 '_version': 1,
 'result': 'created',
 '_shards': {'total': 2, 'successful': 1, 'failed': 0},
 '_seq_no': 0,
 '_primary_term': 1}

# but not deserialized
es.get(index="my-index", id=42)['_source']
{'any': 'data', 'timestamp': '2019-05-17T17:28:10.329598'}
print(es.info())

# Creation des Index dans elasticsearch
es.indices.create(index="item_rss", ignore=400)
es.indices.create(index="title", ignore=400)
es.indices.create(index="summary", ignore=400)
es.indices.create(index="description", ignore=400)
es.indices.create(index="all_links", ignore=400)
es.indices.create(index="source_post", ignore=400)
es.indices.create(index="lang", ignore=400)
es.indices.create(index="date", ignore=400)
es.indices.create(index="target_data", ignore=400)


# Insertion dans chacun des index qui menent a l'item
es.index(index="item_rss", id=XXX, body={'item' : objectItem_Rss})
es.index(index="title", id=nom_title, body={'id_item' : id_de_lobject})
es.index(index="title", id=summary, body={'id_item' : id_de_lobject})
es.index(index="title", id=description, body={'id_item' : id_de_lobject})
es.index(index="title", id=all_links, body={'id_item' : id_de_lobject})
es.index(index="title", id=source_post, body={'id_item' : id_de_lobject})
es.index(index="title", id=lang, body={'id_item' : id_de_lobject})
es.index(index="title", id=date, body={'id_item' : id_de_lobject})
es.index(index="title", id=target_data, body={'id_item' : id_de_lobject})
