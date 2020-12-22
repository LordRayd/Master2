from src.crawler import Crawler
from src.nettoyeur import Nettoyeur
from src.dataset import Dataset
from src.classifier import Classifier
from src.database_tool import DatabaseTool
from src.elastic_tool import ElasticToolv0
from src.benchmark import result
from src.enrichment import Enrichment

class IHM :
    """
    Interface de RSS Intelligence 
    """

    def choix():
        """
        Permet le premier choix d actions a executer
        """
        print("Vous pouvez à tous moments quitter le programme en entrant la lettre 'q' minuscule")
        while(1) : 
            val = input("crawl | classif | benchmark | search | enrich : ")
            if val == 'crawl' or val == 'cr':
                ret = IHM.crawl()
                if ret == False:
                    break
            elif val == 'classif' or val == 'cl':
                ret = IHM.classify()
                if ret == False:
                    break
            elif val == 'benchmark' or val == 'b':
                ret = IHM.benchmark()
                if ret == False:
                    break
            elif val == 'search' or val == 's':
                ret = IHM.search()
                if ret == False:
                    break
            elif val == 'enrich' or val == 'e':
                ret = IHM.enrich()
                if ret == False:
                    break
            elif val == 'q' :
                break

    def enrich():
        """
        Permet l utilisation du module Enrichment
        """
        enri = Enrichment()
        
        load_bool = input('Charger un model ou en créer un (l/i) : ')
        if load_bool == 'q':
            return False
        elif load_bool == 'i':
            enri.enrich(save=True)
        nb_mot = 'init'
        while not nb_mot.isdigit():
            nb_mot = input('Nombre de mot similaire a afficher : ')
            if nb_mot == 'q':
                return False
        
        enri.enrich(load=True)
        texte = input('Recherche par similarité : ')
        words = enri.most_similar(texte, n_words_similar=int(nb_mot))
        for word in words:
            print(word)

    def search():
        """
        Permet la recherche dans elasticsearch par mot similaire
        """
        elastic = ElasticToolv0()
        texte = input("texte a rechercher : ")

        n_similar = 3
        enrich = Enrichment()
        ret_words = enrich.most_similar(texte, n_words_similar=n_similar)
        max_words = len(ret_words)
        str_query = ''
        for i in range(max_words):
            str_query += '('
            for j in range(n_similar):
                str_query += 'description:' + ret_words[i][j][0]
                if j < n_similar-1:
                    str_query += ' OR '

            str_query += ')'
            if i < max_words-1:
                str_query += ' AND '


        list_elem = elastic.search(str_query)
        print('Il y a ', len(list_elem), ' element(s) qui correspond(e/s)')
        if len(list_elem) > 0 :
            val = input('Afficher toutes les descriptions des items ? (y/n) : ')
            while val != 'y' and val != 'n' and val != 'q':
                val = input('Afficher toutes les descriptions des items ? (y/n) : ')
            if val == 'y':
                for item in list_elem:
                    print(item.description)
            elif val == 'n':
                return True
            elif val =='q':
                return False
        
    def crawl():
        """
        Permet de choisir comment crawler
        """
        val = input("file | url : ")
        if val == 'file' :
            return IHM.crawl_file()
        elif val == 'url':
            return IHM.crawl_url()
        elif val == 'q' :
            return False
    
    def crawl_file():
        """
        Permet de crawler depuis un fichier
        """
        filepath = input("Crawler depuis quelle fichier : ")
        if filepath == 'q':
            return False
        nombre_max = input("Combien de fois doit on crawler a travers les flux : ")
        if nombre_max == 'q':
            return False
        cr = Crawler(nb_max=int(nombre_max), _save_tool=1)
        cr.crawl_from_file(filepath)
        
    def crawl_url():
        """
        Permet de crawler depuis une url ou un fichier xml
        """
        filepath = input("Crawler depuis quelle url : ")
        if filepath == 'q':
            return False
        nombre_max = input("Combien de fois doit on crawler a travers les flux : ")
        if nombre_max == 'q':
            return False
        cr = Crawler(nb_max=int(nombre_max), _save_tool=1)
        cr.crawl(filepath)

    def classify():
        """
        Permet de choisir quoi faire avec le module classifier
        """
        lang = input('Langue à utilisé (fr | en) : ')
        if lang == 'q':
            return False
        val = input('init | validation | pedict : ')
        if val == 'init' or val == 'i':
            return IHM.classify_init(lang)
        elif val == 'validation' or val == 'v':
            return IHM.classify_validation(lang)
        elif val == 'predict' or val == 'p':
            return IHM.classify_predict(lang)

    def classify_init(lang):
        """
        Permet l'initialisation et la classification de tous les elements de la langue données
        """
        classifier = Classifier(lang=lang)
        classifier.create_dataset(tool=1, save_dataset=True)
        classifier.all_classification(save_model=True)

    def classify_validation(lang):
        """
        Permet d'executer la validation de tous les elements de la langue données
        """
        classifier = Classifier(lang=lang)
        classifier.load_all(lang=lang)
        classifier.all_classification()

    def classify_predict(lang):
        """
        Permet de predire la classe du texte que l'on va taper dans la langue données précedement
        """
        classifier = Classifier(lang=lang)
        classifier.load_all(lang=lang)
        texte = input('Phrase à déterminer : ')
        if texte == 'q':
            return False
        y_pred, proba, classes = classifier.predict(texte)
        print(y_pred)

    def benchmark():
        """
        Permet l execution du benchmark
        """
        filepath = input('nom du fichier sur lequel appliquer le benchmark : ')
        if filepath == 'q':
            return False
        
        lang = input('Langue à utilisé (fr | en) : ')
        while(lang != 'fr' and lang != 'en'): 
            if lang == 'q':
                return False
            lang = input('Langue à utilisé (fr | en) : ')
        result(filepath, lang=lang)

IHM.choix()