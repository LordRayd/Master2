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

class Classifier :
    """
    Attributs :

    dataset : 
    model_bayesien : 
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
        self.model_bayesien = None
        self.model_random_forest = None
        self.model_neural = None

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
        print('Nombre de données d entrainement', self.dataset.X_train.shape[0])
        print('Nombre de données de test', (self.dataset.X_test.shape[0]))
        print('Pourcentage erreur dans ces données : ', (1-score)*100, '%')

        if save_model == True :
            self.save_model(self.model_bayesien, model_name='bayesian')
    
    def random_forest(self):
        clf = RandomForestClassifier(max_depth=2, random_state=0)
        clf.fit(X, y)
        print(clf.predict([[0, 0, 0, 0]]))

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
        print('Nombre de données d entrainement', self.dataset.X_train.shape[0])
        print('Nombre de données de test', (self.dataset.X_test.shape[0]))
        print('Pourcentage erreur dans ces données : ', (1-score)*100, '%')

        if save_model == True :
            self.save_model(self.model_bayesien, model_name='neural')
        



    def save_model(self, model, model_name, lang='fr', default_path_file='model/'):
        pickle.dump(model, open(default_path_file + lang + '/' + model_name +'.pkl', 'wb'))


    def load_model(self, model_name, lang='fr', default_path_file='model/'):
        if model_name == 'bayesian' :
            self.model_bayesien = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))
        elif model_name == 'neural' :
            self.model_neural = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))