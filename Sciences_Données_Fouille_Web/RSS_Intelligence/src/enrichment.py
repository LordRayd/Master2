from src.nettoyeur import Nettoyeur
from src.elastic_tool import ElasticToolv0
from src.database_tool import DatabaseTool

from gensim.models import Word2Vec
from gensim.utils import tokenize

class Enrichment:

    def __init__(self, tool=1):
        """
        Initialise l objet Enrichment

        Paramètres:
        tool : L outil de sauvegarde utilisé 
            0 : DatabaseTool
            1 : ElasticToolv0
        """
        self.nettoyeur = Nettoyeur()
        if tool == 1:
            self.tool = ElasticToolv0()
        elif tool==0:
            self.tool = DatabaseTool()
        self.path = "model/word2vec"

    def clean(self, texte, lang='fr'):
        """
        Utilise le nettyeur pour supprimer les elements genants du texte

        Paramètres:
        texte : Le texte à nettoyer
        lang : La langue a utilisé lors du nettoyage
        """
        return self.nettoyeur.enleve_stop_words(self.nettoyeur.enleve_chiffre(texte),lang=lang)

    def enrich(self, load=False, save=False, lang=['fr', 'en']):
        """
        Initialise le model Word2Vec, l'entraine et le sauvegarde
        
        Paramètres:
        load : si True charge le model depuis les fichiers locaux
        save : si True sauvegarde le model dans le dossier model
        lang : un array des langs a supportés
        """
        words = []

        for language in lang:
            for item in self.tool.get_all_element_lang(lang=language):
                words.append(list(tokenize(self.clean(item.description), deacc=True, lower=True)))
        
        if load == False :
            self.word2vec = Word2Vec(words, min_count=2)
        else :
            self.loadVect()
        self.word2vec.train(words, total_examples=self.word2vec.corpus_count, epochs=self.word2vec.epochs)

        if save == True:
            Word2Vec.save(self.word2vec, self.path)

    def loadVect(self) :
        """
        Recharge le model Word2Vec depuis les fichiers locaux
        """
        self.word2vec = Word2Vec.load(self.path)

    def most_similar(self, words, n_words_similar=3):
        """
        Trouve les mots similaires au texte rentré
        
        Paramètres:
        words : une phrase des mots dont on cherche les mots similaires 
        n_words_similar : Le nombre de mot le plus similaires a retourner
        
        Retourne Un liste des mots similaires"""
        ret = []
        self.loadVect()
        for word in words.split():
            if word in self.word2vec.wv.vocab:
                ret.append(self.word2vec.most_similar(positive=[word], topn=n_words_similar))
        return ret