import shelve

class DatabaseTool:
    """
    Outil de sauvegarde des elements Rss dans une base de données shelve

    Attributs:
    _db : La connexion à la base de données
    """
    _db = None

    def __init__(self, database_name='database/database'):
        """
        Initialiseur de l'objet DatabaseTool

        Paramètres:
        database_name : L'endroit ou sera sauvegarder la base de données
        """
        self._db = self.getConnection(database_name=database_name)

    def getConnection(self, database_name='database/database'):
        """
        Ouvre une connection avec la base de données et la retourne

        Paramètres:
        database_name : L'endroit ou sera sauvegarder la base de données
        """
        return shelve.open(database_name, 'c')

    def insertion_items(self, _items) :
        """
        Insère les items données en base de données

        Paramètres:
        _items : La liste des items à sauvegarder
        """
        for i in _items :
            self.insertion_item(i)

    def insertion_item(self, _item):
        """
        Sauvegarde un item en base de données si il n'y est pas deja ou qu'il a changé

        Paramètres:
        _item : L'item a sauvegarder
        """
        if self._db.__contains__(_item.id) == False:
            self._db[_item.id] = _item
        else : 
            if _item.integrity != self._db[_item.id].integrity :
                self._db[_item.id] = _item

    def verification_integrity(self, id_, integrity_):
        """
        Vérifie l'intégrité d'un élément en base

        Paramètres:
        id_ : L'identifiant de l'élément
        integrity_ : La valeur a comparer avec celle en base

        Retour:
        False si l'élément n'est pas en base ou que son champs integrity a changé
        True autrement
        """
        ret = True
        if self._db.__contains__(id_) == False:
            ret = False
        else : 
            if integrity_ != self._db[id_].integrity :
                ret = False
        return ret
    
    def get_all_element_lang(self, lang='fr'):
        """
        Retourne tous les éléments pour une langue donnée
        
        Paramètres:
        lang='fr' : La langue dont il faut récupérer les éléments
        
        Retourne:
        Une liste d'objet Item_RSS
        """
        list_item = []
        for key in self._db.keys() :
            if(self._db[key].lang == lang) :
                list_item.append(self._db[key])
        return list_item