from src.crawler import Crawler
from src.nettoyeur import Nettoyeur
from src.dataset import Dataset
from src.classifier import Classifier
from src.database_tool import DatabaseTool

# Activation Crawler
#cr = Crawler(2, _save_tool=0)
#cr.crawl_from_file('ressources/mini_flux_rss.txt')
#cr.crawl_from_file('ressources/flux_rss.txt')


# Initialise classifier fr
#print("=============================================== Initialise classifier fr + Validation")
#classifier_fr = Classifier()
#classifier_fr.create_dataset(save_dataset=True)
#classifier_fr.all_classification(save_model=True)


# Recharge classifier fr
print("=============================================== Validation classifier fr")
classifier_fr = Classifier(lang='fr')
classifier_fr.load_all_model()
classifier_fr.load_dataset()
classifier_fr.all_classification()


# Initialise classifier en
#print("=============================================== Initialise classifier en + Validation")
#classifier_en = Classifier(lang='en')
#classifier_en.create_dataset(save_dataset=True)
#classifier_en.all_classification(save_model=True)


# Recharge classifier en
print("=============================================== Validation classifier en")
classifier_en = Classifier(lang='en')
classifier_en.load_all_model()
classifier_en.load_dataset()
classifier_en.all_classification()


# ==== Validation Croisé fr to en
print("=============================================== Validation croisée classifieur fr to en")
classifier_fr_en = Classifier(lang='fr')
classifier_fr_en.load_all_model()
classifier_fr_en.dataset = classifier_en.dataset
classifier_fr_en.all_classification()

# ==== Validation Croisé en to fr
print("=============================================== Validation croisée classifieur en to fr")
classifier_en_fr = Classifier(lang='en')
classifier_en_fr.load_all_model()
classifier_en_fr.dataset = classifier_fr.dataset
classifier_en_fr.all_classification()