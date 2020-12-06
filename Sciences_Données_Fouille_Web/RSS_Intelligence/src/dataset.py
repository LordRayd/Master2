from src.database_tool import DatabaseTool
from src.elastic_tool import ElasticTool
from src.nettoyeur import Nettoyeur

import pickle

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.naive_bayes import GaussianNB

class Dataset:
    """
    Gère les datasets que l'on va utilisé

    Attributs:
    nettoyeur : L'objet Nettoyeur
    lang : La lang du Dataset que l'on souhaite créé
    data : L'ensemble des données de la langue choisit
    target : L'ensemble des type de flux correspondant aux données récupérées
    X_train : Le jeu de données d'entrainement
    y_train : Les labels du jeu de données d'entrainement
    X_test : Le jeu de données de validation
    y_test : Les labbels du jeu de doonnées de validation
    vector : L'objet CountVectorizer qui servira pour la construction des X
    encoder_label : L'objet LabelEncoder qui servira pour la construction des Y
    """

    def __init__(self, lang='fr', tool=0):
        """
        Initialisateur de la classe Dataset

        Paramètres:
        lang='fr' : la langue dont on souhaite construire un jeu de données
        tool=0 : Quels outils va ton utiliser pour la recupération des informations
            0 : DatabaseTool
            1 : ElasticTool
        """
        self.nettoyeur = Nettoyeur(tool=tool)
        self.lang = lang
        self.data = []
        self.target = []
        self.vector = None
        self.encoder_label = None
    
    def create_dataset(self, tool=0, save_dataset=False):
        """
        Creation d'un jeu de données sur un langage

        Paramètres:
        tool=0 : Quels outils va ton utiliser pour la recupération des informations
            0 : DatabaseTool
            1 : ElasticTool
        save_dataset=False : si on doit ou non sauvegarder le dataset dans les fichiers
        """
        if tool == 0 :
            database = DatabaseTool()
            list_elem = database.get_all_element_lang(lang=self.lang)
            for elem in list_elem :
                self.data.append(self.nettoyeur.get_clean_string(elem.target_data, lang=self.lang))
                self.target.append(elem.type_flux)
            self.X_train, self.X_test, self.y_train, self.y_test = train_test_split(self.data, self.target, test_size=0.15)

            # Transforme Les lebels
            self.encoder_label = LabelEncoder()
            self.y_train = self.encoder_label.fit_transform(self.y_train)
            self.y_test = self.encoder_label.fit_transform(self.y_test)

            # create a count vectorizer object 
            self.vector = CountVectorizer(analyzer='word', token_pattern=r'\w{1,}')
            self.vector.fit(self.data)

            # transform the training and validation data using count vectorizer object
            self.X_train =  self.vector.transform(self.X_train).toarray()
            self.X_test =  self.vector.transform(self.X_test).toarray()

        if save_dataset == True :
            self.save_vector()
            self.save_data()
            self.save_label_encoder()

    def load_dataset(self,lang='fr'):
        """
        Recharge un dataset depuis les fichiers

        Paramètres:
        lang : la langue du dataset a chargé
        """
        self.load_vector(lang=lang)
        self.load_label_encoder(lang=lang)
        self.load_data(lang=lang)

    def preprocessing_data(self, texte_xml):
        """
        Applique le nottoyeur a un texte xml que l'on souhaite utilisé pour la prédiction

        Paramètres:
        texte_xml : le texte xml a transformer

        Retourne l'élément qui sera données a predict de Classifier
        """
        tab_text = [self.nettoyeur.get_clean_string(texte_xml)]
        return self.vector.transform(tab_text).toarray()

    def save_data(self):
        """
        Sauvegarde les données d entrainement et de test
        """
        pickle.dump(self.X_train, open("dataset/"+ self.lang +"/xtrain.pkl","wb"))
        pickle.dump(self.X_test, open("dataset/"+ self.lang +"/xtest.pkl","wb"))
        pickle.dump(self.y_train, open("dataset/"+ self.lang +"/ytrain.pkl","wb"))
        pickle.dump(self.y_test, open("dataset/"+ self.lang +"/ytest.pkl","wb"))

    def load_data(self, lang='fr'):
        """
        Recharge les données d entrainement et de test depuis les fichiers
        """
        self.X_train = pickle.load(open("dataset/"+ lang +"/xtrain.pkl", "rb"))
        self.X_test = pickle.load(open("dataset/"+ lang +"/xtest.pkl", "rb"))
        self.y_train = pickle.load(open("dataset/"+ lang +"/ytrain.pkl", "rb"))
        self.y_test = pickle.load(open("dataset/"+ lang +"/ytest.pkl", "rb"))

    def save_vector(self):
        """
        Sauvegarde le vecteur de transformation des textes
        """
        pickle.dump(self.vector.vocabulary_,open("dataset/"+ self.lang +"/vect.pkl","wb"))

    def load_vector(self, lang='fr'):
        """
        Recharge le vecteur de transformation des textes depuis les fichiers
        """
        self.vector = CountVectorizer(decode_error="replace",vocabulary=pickle.load(open("dataset/"+ lang +"/vect.pkl", "rb")))

    def save_label_encoder(self):
        """
        Sauvegarde le transformer de label
        """
        pickle.dump(self.encoder_label.classes_, open("dataset/"+ self.lang +"/label.pkl","wb"))
    
    def load_label_encoder(self, lang='fr'):
        """
        Recharge le transformer de label depuis les fichiers
        """
        self.encoder_label = LabelEncoder()
        self.encoder_label.classes_ = pickle.load(open("dataset/"+ lang +"/label.pkl", "rb"))