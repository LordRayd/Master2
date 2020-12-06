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
        save_model=False : Sauvegarde le model pour qu'il soit recharger lors d'un futur lancement grrace a la methode save_model
        """

        print('=============== Classifier Bayesion Naif =================')
        # Creation model si inexistant
        if self.model_bayesien == None :
            self.model_bayesien = GaussianNB()
            self.model_bayesien.fit(self.dataset.X_train, self.dataset.y_train)

        score = self.model_bayesien.score(self.dataset.X_test, self.dataset.y_test)
        print('Nombre de données d entrainement', self.dataset.X_train.shape[0])
        print('Nombre de données de test', (self.dataset.X_test.shape[0]))
        print('Pourcentage erreur dans ses données : ', (1-score)*100, '%')

        if save_model == True :
            self.save_model(self.model_bayesien, model_name='bayesian')
    
    def random_forest(self):
        X, y = make_classification(n_samples=1000, n_features=4, n_informative=2, n_redundant=0,  random_state=0, shuffle=False)
        clf = RandomForestClassifier(max_depth=2, random_state=0)
        clf.fit(X, y)
        print(clf.predict([[0, 0, 0, 0]]))

    def neural(self):
        self.model_neural = MLPClassifier(solver='lbfgs', alpha=1e-5, hidden_layer_sizes=(5, 2), random_state=1)
        self.model_neural.fit(self.dataset.X_train, self.dataset.y_train)
        print(self.model_neural.predict(self.dataset.X_test))

    def save_model(self, model, model_name, lang='fr', default_path_file='model/'):
        if model_name == 'bayesian' :
            pickle.dump(model, open(default_path_file + lang + '/' + model_name +'.pkl', 'wb'))

    def load_model(self, model_name, lang='fr', default_path_file='model/'):
        
        if model_name == 'bayesian' :
            self.model_bayesien = pickle.load(open(default_path_file + lang + '/' + model_name +'.pkl', 'rb'))