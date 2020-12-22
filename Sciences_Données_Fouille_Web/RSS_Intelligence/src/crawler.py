import fileinput
import feedparser

from src.item_rss import Item_RSS
from src.database_tool import DatabaseTool
from src.elastic_tool import ElasticTool
from src.elastic_tool import ElasticToolv0

class Crawler:
    """
    Attributs :
    
    nb_crawl_max=3 : Le nombre maximal de crawl possible
    nb_already_done : Le nombre de crawl deja effectués
    save_tool=1 : Quels outils va ton utiliser pour la sauvegarde des informations
        0 : DatabaseTool
        1 : ElasticTool
    clean=1 : Si on a besoin de clean la page web avant de la sauvegarder la valeur est a 1 autrement 0
    tool : L'outil qui sera utilisé pour la sauvegarde
    """

    def __init__(self, nb_already=0, _save_tool=1, nb_max=3):
        """
        Initialiseur de l'objet Classifier

        Paramètres :
        _save_tool=1 : Le numéro de l'outil que l'ont utilisera pour la sauvegarde
            0 : DatabaseTool
            1 : ElasticTool
        nb_already=0 :  Le nombre de crawl deja effectués
        """
        self.nb_already_done = nb_already
        self.save_tool = _save_tool
        self.nb_crawl_max = nb_max
        if self.save_tool == 0 : 
            self.tool = DatabaseTool()
            self.clean = 0
        else : 
            self.tool = ElasticToolv0()
            self.clean = 1

    def crawl(self, _link, _type_flux='default'):
        """
        Crawl dans le lien donnée

        Paramètres :
        _link : le lien dans lequel il faut crawler
        _type_flux : Le type de flux qui sera mis par default au élément recupéré
        """
        if(self.nb_already_done < self.nb_crawl_max):
            try : 
                d = feedparser.parse('%s' % _link)
                for post in d.entries:
                    elem = Item_RSS()
                    elem.create_from_feed(post,d.feed, tool=self.tool, type_flux=_type_flux, clean_source=self.clean)
                    for l in elem.all_links:
                        c = Crawler(self.nb_already_done + 1, _save_tool=self.save_tool)
                        c.crawl(l['href'], _type_flux=_type_flux)
                    elem.save()
            except UnicodeEncodeError as e:
                self.target_data = None
                
    def crawl_from_file(self, filename):
        """
        Crawl depuis un fichier dont la formation est sur une ligne :
        lien TYPE_DE_FLUX

        filename : Le chemin vers le fichier a lire
        """
        links = [link.rstrip('\n').split(' ') for link in fileinput.input(files=(filename))]
        
        print('================== Commence à Crawler ====================\n')
        for link, subject in links:
            self.crawl(link, _type_flux=subject)
        print('==================== Fini de Crawler =====================\n')