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

## Fonctionnement
Le module Classifier s'appuie sur les modules :
* Crawler
* DatabaseTool
* Item_RSS
Et il comprend les modules :
* Classifier
* Dataset
* Nettoyeur
Une evolution devrait pouvoir permettre au module d'utiliser ElasticSearch. Tout est pret il ne reste que a modifier legèrement ElasticTool