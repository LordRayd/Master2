from src.dataset import Dataset
import pickle

#import sklearn classifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import make_classification
from sklearn.neural_network import MLPClassifier
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.svm import SVC
from sklearn.metrics import precision_recall_curve
from sklearn.metrics import average_precision_score

class Classifier :
    """
    Attributs:

    dataset : Le dataset utilisé pour entrainer et tester les modeles
    model_knn : le modele du plus proche voisin
    model_reg_log : le modele de regression logistique
    model_bayesien : le modele du bayesien naif
    model_svm : le modele svm
    model_random_forest : le modele ranndom forest
    model_neural : le modele de reseau de neurone
    """

    def __init__(self, lang='fr'):
        """
        Initialiseur de l'objet Classifier

        Paramètres :
        lang='fr' : Choix de la langue du Classifier
        """

        self.dataset = Dataset(lang=lang)
        self.model_knn = None
        self.model_reg_log = None
        self.model_bayesien = None
        self.model_svm = None
        self.model_random_forest = None
        self.model_neural = None
    
    def create_dataset(self, tool=0, save_dataset=False) :
        """
        Crée un nouveau Dataset

        Paramètres:
        tool=0 : Quel outil va ton utiliser pour la recupération des informations
            0 : DatabaseTool
            1 : ElasticTool
        save_dataset=False : si on doit ou non sauvegarder le dataset dans les fichiers
        """
        self.dataset.create_dataset(tool=tool, save_dataset=save_dataset)

    def load_dataset(self, lang='fr'):
        """
        Recharge un dataset depuis les fichiers

        Paramètres:
        lang : la langue du dataset a chargé
        """
        self.dataset.load_dataset(lang=lang)

    def knn(self, save_model=False):
        """
        Utilisation du classifieur k plus proche voisin
        
        Paramètres
        save_model=False : Sauvegarde le model pour qu'il soit recharger lors d'un futur lancement grace a la methode save_model
        """

        print('==================== Classifier KNN ======================')
        if self.model_knn == None :
            self.model_knn = KNeighborsClassifier(n_neighbors=3)
            self.model_knn.fit(self.dataset.X_train, self.dataset.y_train)

        score = self.model_knn.score(self.dataset.X_test, self.dataset.y_test)
        self.affiche_resultat_score(score)
        if save_model == True :
            self.save_model(self.model_knn, model_name='knn')

    def reg_log(self, save_model=False):
        """
        Utilisation du classifieur regression logistique
        
        Paramètres
        save_model=False : Sauvegarde le model pour qu'il soit recharger lors d'un futur lancement grace a la methode save_model
        """

        print('=========== Classifier Regression Logistique =============')
        if self.model_reg_log == None :
            self.model_reg_log = LogisticRegression(max_iter=1000)
            self.model_reg_log.fit(self.dataset.X_train, self.dataset.y_train)

        score = self.model_reg_log.score(self.dataset.X_test, self.dataset.y_test)
        self.affiche_resultat_score(score)
        if save_model == True :
            self.save_model(self.model_reg_log, model_name='reg_log')

    def bayesien(self, save_model=False):
        """
        Utilisation du classifieur bayesien naif
        
        Paramètres
        save_model=False : Sauvegarde le model pour qu'il soit recharger lors d'un futur lancement grace a la methode save_model
        """

        print('=============== Classifier Bayesion Naif =================')
        # Creation model si inexistant
        if self.model_bayesien == None :
            self.model_bayesien = GaussianNB()
            self.model_bayesien.fit(self.dataset.X_train, self.dataset.y_train)

        score = self.model_bayesien.score(self.dataset.X_test, self.dataset.y_test)
        self.affiche_resultat_score(score)

        if save_model == True :
            self.save_model(self.model_bayesien, model_name='bayesian')

    def svm(self, save_model=False):
        """
        Utilisation du classifieur SVM
        
        Paramètres
        save_model=False : Sauvegarde le model pour qu'il soit recharger lors d'un futur lancement grace a la methode save_model
        """

        print('==================== Classifier SVM ======================')
        if self.model_svm == None :
            self.model_svm = SVC(gamma='auto')
            self.model_svm.fit(self.dataset.X_train, self.dataset.y_train)

        score = self.model_svm.score(self.dataset.X_test, self.dataset.y_test)
        self.affiche_resultat_score(score)
        if save_model == True :
            self.save_model(self.model_svm, model_name='svm')

    def neural(self, save_model=False):
        """
        Utilisation du classifieur reseau de neurone
        
        Paramètres
        save_model=False : Sauvegarde le model pour qu'il soit recharger lors d'un futur lancement grace a la methode save_model
        """

        print('============= Classifier Reseau de Neurone ===============')
        if self.model_neural == None :
            self.model_neural = MLPClassifier(max_iter=1000)
            self.model_neural.fit(self.dataset.X_train, self.dataset.y_train)

        score = self.model_neural.score(self.dataset.X_test, self.dataset.y_test)
        self.affiche_resultat_score(score)
        if save_model == True :
            self.save_model(self.model_neural, model_name='neural')
        
    def random_forest(self, save_model=False):
        """
        Utilisation du classifieur Random Forest
        
        Paramètres:
        save_model=False : Sauvegarde le model pour qu'il soit recharger lors d'un futur lancement grace a la methode save_model
        """

        print('=============== Classifier Random Forest =================')
        if self.model_random_forest == None :
            self.model_random_forest = RandomForestClassifier(max_depth=2, random_state=0)
            self.model_random_forest.fit(self.dataset.X_train, self.dataset.y_train)

        score = self.model_random_forest.score(self.dataset.X_test, self.dataset.y_test)
        self.affiche_resultat_score(score)

        if save_model == True :
            self.save_model(self.model_random_forest, model_name='forest')

    def all_classification(self, save_model=False):
        """
        Execute toute les classifications les unes a la suite des autres

        Paramètres:
        save_model=False : Sauvegarde le model pour qu'il soit recharger lors d'un futur lancement grace a la methode save_model 
        """
        self.knn(save_model=save_model)
        self.reg_log(save_model=save_model)
        self.bayesien(save_model=save_model)
        self.svm(save_model=save_model)
        self.neural(save_model=save_model)
        self.random_forest(save_model=save_model)

    def affiche_resultat_score(self, score):
        """
        Affiche les resultats obtenu par uun model

        Paramètres:
        score : Le score a affiché
        """
        print('Nombre de données d entrainement', self.dataset.X_train.shape[0])
        print('Nombre de données de test', (self.dataset.X_test.shape[0]))
        print('Pourcentage erreur dans ces données : ', (1-score)*100, '%\n')

    def predict(self, texte_xml):
        """
        Predit la classe/label d'un texte xml donné

        Paramètres:
        texte_xml : Le texte dont on souhaite connaitre le label

        Retourne le label
        """
        x = self.dataset.preprocessing_data(texte_xml)
        y = self.model_bayesien.predict(x)
        return self.dataset.encoder_label.inverse_transform(y)[0]

    def save_model(self, model, model_name, lang='fr', default_path_file='model/'):
        """
        Sauvegarde le model donné

        Paramètres:
        model : le modele a sauvegarder
        model_name : le nom du modele a sauvegarder
        lang : la langue du modele
        default_path_file : le chemin par default ou sera stockés le modele
        """
        pickle.dump(model, open(default_path_file + lang + '/' + model_name +'.pkl', 'wb'))

    def load_model(self, model_name, lang='fr', default_path_file='model/'):
        """
        Recharge un model depuis les fichiers
        
        Paramètres:
        model_name : le nom du modele a recharger
        lang : la langue du modele
        default_path_file : le chemin par default ou est stockés le modele
        """
        if model_name == 'knn' :
            self.model_knn = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))
        elif model_name == 'reg_log':
            self.model_reg_log = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))
        elif model_name == 'bayesian' :
            self.model_bayesien = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))
        elif model_name == 'svm' : 
            self.model_svm = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))
        elif model_name == 'neural' :
            self.model_neural = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))
        elif model_name == 'forest' :
            self.model_random_forest = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))
        else:
            print('modele non reconnu')

    def load_all_model(self,lang='fr',default_path_file='model/'):
        """
        Recharge tous les modeles depuis les fichiers
        """
        self.load_model('knn', lang=lang, default_path_file=default_path_file)
        self.load_model('reg_log', lang=lang, default_path_file=default_path_file)
        self.load_model('bayesian', lang=lang, default_path_file=default_path_file)
        self.load_model('svm', lang=lang, default_path_file=default_path_file)
        self.load_model('neural', lang=lang, default_path_file=default_path_file)
        self.load_model('forest', lang=lang, default_path_file=default_path_file)