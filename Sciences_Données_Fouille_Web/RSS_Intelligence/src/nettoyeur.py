from string import punctuation
from snowballstemmer import stemmer
from stop_words import get_stop_words

from bs4 import BeautifulSoup
import lxml
from lxml.html.clean import Cleaner
import io
import os

class Nettoyeur :

    def __init__(self, tool=0, optional_ponctiation_carac=''):
        self.punctuation = punctuation + '«».'
        self.punctuation += optional_ponctiation_carac
        self.tool = tool

    def supported_lang(self, lang='fr') :
        if lang == 'fr':
            return 'french'
        elif lang == 'en':
            return 'english'
        else :
            raise Exception(f'Le language n est pas supporté {lang}')

    def enleve_balise(self, texte):
        soup = BeautifulSoup(texte, features="html.parser")
        cleaner = Cleaner()
        cleaner.javascript = True # Enleve les balises script et leur contenu
        cleaner.style = True      # Enleve les balises style et leur contenu
        # ====== PLUS DE BALISE SCRIPT & STYLE ==========
        target_data = lxml.html.tostring(cleaner.clean_html(lxml.html.document_fromstring(str(soup.prettify())))) 
        s = BeautifulSoup(target_data, features='lxml')
        return os.linesep.join([s.lstrip() for s in s.getText().splitlines() if s and s.strip() != ''])

    def enleve_espace_xml(self, texte):
        s = BeautifulSoup(texte, features='lxml')
        return os.linesep.join([s.lstrip() for s in s.getText().splitlines() if s and s.strip() != ''])

    def enleve_espace(self, texte):
        return texte.replace('\t', ' ').replace('\n', '').strip()

    def enleve_chiffre(self, texte):
        return ''.join([i for i in texte if not i.isdigit()])

    def enleve_ponctuation(self, texte):
        return str(texte).translate(str.maketrans('', '', self.punctuation))

    def enleve_stop_words(self, texte, lang='fr'):
        stopwords = get_stop_words(self.supported_lang(lang))
        return ' '.join([word for word in texte.split() if word.lower() not in stopwords])

    def nettoyer(self, texte, lang='fr'):
        if self.tool == 1 :
            return self.enleve_stop_words(self.enleve_ponctuation(self.enleve_chiffre(self.enleve_espace_xml(texte))),lang)
        else :
            return self.enleve_stop_words(self.enleve_ponctuation(self.enleve_chiffre(self.enleve_balise(texte))), lang)

    def get_clean_array(self, texte, lang='fr'):
        stem = stemmer(self.supported_lang(lang))
        ret = []
        array_texte_cleaned = self.nettoyer(texte,lang=lang).split(' ')
        for elem in array_texte_cleaned :
            ret.append(stem.stemWord(elem.lower()))
        return ret

    def get_clean_string(self, texte, lang='fr'):
        texte_array = self.get_clean_array(texte, lang=lang)
        return ' '.join(texte_array)

if __name__ == '__main__':
    nettoyeur = Nettoyeur()
    print(nettoyeur.get_clean_array("tést. a    € 1 2 3\n 1 2 3 test"))