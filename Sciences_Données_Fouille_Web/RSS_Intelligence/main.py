from src.crawler import Crawler
from src.nettoyeur import Nettoyeur
from src.dataset import Dataset
from src.classifier import Classifier
from src.database_tool import DatabaseTool


cr = Crawler(2, _save_tool=0)
cr.crawl_from_file('ressources/mini_flux_rss.txt')
#cr.crawl_from_file('ressources/flux_rss.txt')

classifier_fr = Classifier()
classifier_fr.create_dataset(save_dataset=True)
classifier_fr.all_classification(save_model=True)

#classifier_fr.load_all_model()
#classifier_fr.load_dataset()



classifier_en = Classifier(lang='en')
classifier_en.create_dataset(save_dataset=True)
classifier_en.all_classification(save_model=True)

# ==== Prediction d'une classe
bdd = DatabaseTool()
for key in bdd._db.keys() :
    if(bdd._db[key].lang == 'fr') :
        texte_xml = bdd._db[key].target_data
        vrai_classe = bdd._db[key].type_flux
#print(texte_xml)
classifier_fr.predict(texte_xml)
print(vrai_classe)