# RSS Intelligence
## Installation
### Modules 
Intaller les modules aveec pip install :
* langdetect
* elasticsearch
* BeautifulSoup
* scikit-learn
* stop-words ()
* feedparser (https://pypi.org/project/feedparser/)
* snowballstemmer (https://pypi.org/project/snowballstemmer/)
* matplotlib
* gensim (https://pypi.org/project/gensim/)
### Projet
Dezipper RSS_Intelligence
## Lancement
### Pré-requis
* Avoir les fichiers dans le dossier dataset
* Avoir un fichier de flux rss intitulé *mini_flux_rss.txt* dans ressources
* Avoir un dossier database
* Avoir les modeles dans le dossier model
### Commande
Pour lancer il suffit de faire *python main.py*

## Benchmark résultats
| **Méthode** | **language** | **F1** | **accuracy** | **auc(macro)** |
|---|---|---|---|---|
| NeuralNetwork | fr | 0.508 | 0.683 | 0.852 |
| NeuralNetwork | en | 0.117 | 0.162 | 0.648 |
## Fonctionnement
### Explications
Le Projet de création d'une recherche intelligente s'appuie sur les modules :
* Crawler : s'occupera de crawler dans un flux ou un fichier
* DatabaseTool : Un outil de sauvegarde en base de données shelve
* ElasticTool : Un outil de sauvegarde en base de données elasticsearch
* Item_RSS : Une representation d un item rss
* Classifier : Classifie les textes en different type ['ART_CULTURE', 'ECONOMIE', 'POLITIQUE', 'SANTE_MEDECINE', 'SCIENCE', 'SPORT']
* Dataset : Crée un dataset pour le classifier a partir d un outil de sauvegarde
* Nettoyeur : Permet de notteyer les textes avant de les utilisés
* Benchmark : Réalise le benchmark demandé
* IHM : interface d'utilisation de RSS Intelligence

### Architecture
main.py  
├───benchmark  
│&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───benchmark_en.xml  
│&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└───benchmark_fr.xml   
├───database  
├───dataset  
│&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───en  
│&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└───fr  
├───doc    
├───model  
│&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───en  
│&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└───fr  
├───ressources    
│&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───flux_rss.txt  
│&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└───mini_flux_rss.txt  
└───src  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───benchmark.py  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───classifier.py  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───crawler.py  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───database_tool.py  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───dataset.py  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───elastic_tool.py  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───enrichment.py  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├───item_rss.py  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└───nettoyeur.py