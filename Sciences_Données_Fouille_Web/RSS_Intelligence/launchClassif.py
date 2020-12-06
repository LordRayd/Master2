from src.classifier import Dataset
from src.crawler import Crawler

cr = Crawler(2, _save_tool=0)
# ================= Necessite un fichier flux_rss.txt format sur une ligne:  lien type
cr.crawl_from_file('ressources/flux_rss.txt')

dataset = Dataset.create_dataset('fr')
print(len(dataset))