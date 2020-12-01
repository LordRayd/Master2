import fileinput
import feedparser

from src.item_rss import Item_RSS
from src.database_tool import DatabaseTool
from src.elastic_tool import ElasticTool

class Crawler:
    """
    Class Crawler
    """
    nb_crawl_max = 3

    def __init__(self, nb_already=0, _save_tool=1):
        self.nb_already_done = nb_already
        self.save_tool = _save_tool
        if self.save_tool == 0 : 
            self.tool = DatabaseTool()
        else : 
            self.tool = ElasticTool()

    def crawl(self, _link, _type_flux=['default']):
        if(self.nb_already_done < self.nb_crawl_max):
            d = feedparser.parse('%s' % _link)
            for post in d.entries:
                elem = Item_RSS(post,d.feed, tool=self.tool)
                for l in elem.all_links:
                    c = Crawler(self.nb_already_done + 1, _save_tool=self.save_tool)
                    c.crawl(l['href'], _type_flux=_type_flux)
                elem.save()

    def crawl_from_file(self, filename):
        links = [link.rstrip('\n').split(' ') for link in fileinput.input(files=(filename))]
        for link,*subjects in links:
            self.crawl(link, _type_flux=subjects)