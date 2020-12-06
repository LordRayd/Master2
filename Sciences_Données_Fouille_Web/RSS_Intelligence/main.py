from src.crawler import Crawler
from src.nettoyeur import Nettoyeur
from src.dataset import Dataset
from src.classifier import Classifier


cr = Crawler(2, _save_tool=0)
cr.crawl_from_file('ressources/mini_flux_rss.txt')

#dataset = Dataset()
#dataset.create_dataset()
#print(len(dataset.X_train))
#print(len(dataset.X_test))
#print('data fr : ', dataset.data)
#print('target fr : ', dataset.target)

classifier = Classifier()
classifier.all_classification(save_model=True)