import shelve

class DatabaseTool:

    _db = None

    def __init__(self):
        self._db = self.getConnection()

    def getConnection(self, database_name='database/database'):
        return shelve.open(database_name, 'c')

    def insertion_items(self, _items) :
        for i in _items :
            self.insertion_item(i)

    def insertion_item(self, _item): 
        if self._db.__contains__(_item.id) == False:
            self._db[_item.id] = _item
        else : 
            if _item.integrity != self._db[_item.id].integrity :
                self._db[_item.id] = _item

    def verification_integrity(self, id_, integrity_):
        ret = True
        if self._db.__contains__(id_) == False:
            ret = False
        else : 
            if integrity_ != self._db[id_].integrity :
                ret = False
        return ret