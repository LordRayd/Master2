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

class Classifier :
    """
    Attributs :

    dataset : 
    model_knn :
    model_reg_log :
    model_bayesien : 
    model_svm : 
    model_random_forest :
    model_neural : 
    """

    def __init__(self, lang='fr'):
        """
        Initialiseur de l'objet Classifier

        Paramètres :
        lang='fr' : Choix de la langue du Classifier
        """

        self.dataset = Dataset(lang=lang)
        self.dataset.create_dataset()
        self.model_knn = None
        self.model_reg_log = None
        self.model_bayesien = None
        self.model_svm = None
        self.model_random_forest = None
        self.model_neural = None
    
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
        
        Paramètres
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
        self.knn(save_model=save_model)
        self.reg_log(save_model=save_model)
        self.bayesien(save_model=save_model)
        self.svm(save_model=save_model)
        self.neural(save_model=save_model)
        self.random_forest(save_model=save_model)

    def affiche_resultat_score(self, score):
        print('Nombre de données d entrainement', self.dataset.X_train.shape[0])
        print('Nombre de données de test', (self.dataset.X_test.shape[0]))
        print('Pourcentage erreur dans ces données : ', (1-score)*100, '%\n')

    def save_model(self, model, model_name, lang='fr', default_path_file='model/'):
        pickle.dump(model, open(default_path_file + lang + '/' + model_name +'.pkl', 'wb'))

    def load_model(self, model_name, lang='fr', default_path_file='model/'):
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
        elif model_name == 'forest'
            self.model_random_forest = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))
        else:
            print('modele non reconnu')