from xml.dom import minidom
import json
from src.classifier import Classifier

def readXMLFile(fname):
    """
    Read the benchmark file and return a dict 
    """
    # convert an XML file (fname) into a list of dicts
    data = []
    en_db = minidom.parse(fname)
    items = en_db.getElementsByTagName('item')
    for i in range(len(items)):
        d={}
        title=items[i].getElementsByTagName('title')[0].childNodes[0].data
        desc=items[i].getElementsByTagName('description')[0].childNodes[0].data
        txt=items[i].getElementsByTagName('text')[0].childNodes[0].data
        d['title']=title
        d['description']=desc
        d['text']=txt
        data.append(d)
    return data

def result(filename, lang ='fr'):
    """
    Do the classification at each element of the benchmark file and save all pred + proba in a file
    contains in benchmark/
    """
    data = readXMLFile(filename)
    classifier = Classifier(lang=lang)
    classifier.load_all(lang=lang)
    res = dict()
    predicted=[]
    probs=[]
    for elem in data:
        if elem['text'].strip() :
            pred, proba, classes = classifier.predict(elem['text'])
        elif elem['description'].strip() :
            pred, proba, classes = classifier.predict(elem['description'])
        else : 
            pred, proba, classes = classifier.predict(elem['title'])
        predicted.append(pred)
        probs.append(proba[0].tolist())
        
    #predicted=['ART_CULTURE', 'ECONOMIE', 'POLITIQUE', 'SANTE_MEDECINE', 'SCIENCE', 'SPORT'] ## retrive from classifier
    #probs=[[1,0,0,0,0,0],[0,1,0,0,0,0],[0,0,1,0,0,0],[0,0,0,1,0,0],[0,0,0,0,1,0],[0,0,0,0,0,1]]## retrive from classifier predict_proba

    res['pred']=list(predicted)     # list of predicted labels
    res['probs']=list(probs)        # list of list of 6 probas/scores (1 proba/score per class) listed in the following order :
                                    # ['ART_CULTURE' 'ECONOMIE' 'POLITIQUE' 'SANTE_MEDECINE' 'SCIENCE' 'SPORT']
    res['names']=['LEBERRE']        # list of team member name(s )
    res['method']="NeuralNetwork"        # methode name
    res['lang']=lang                # benchmark language (en of fr)
    file = open("benchmark/LEBERRE_NeuralNetwork_"+lang+".res","w")
    file.write(json.dumps(res))
    file.close()

#result('benchmark/benchmark_fr.xml')
#result('benchmark/benchmark_en.xml', lang='en')