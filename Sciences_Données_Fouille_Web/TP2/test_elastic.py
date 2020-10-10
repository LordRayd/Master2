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