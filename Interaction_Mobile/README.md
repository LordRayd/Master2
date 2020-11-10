# Interaction Mobile

## Enseignant
Frank Poirier
## Mot de passe
Ouverture Pdf : IM
## Objectifs
Ce cours est centré sur la conception de l’interaction pour les systèmes mobiles (smartphones, tablettes, objets connectés) avec une vision prospective sur les évolutions futures.

Il développe les points suivants :

* Conception d'interaction avec les dispositifs mobiles et objets connectés
* Méthodes de l'intelligence artificielle et de l'apprentissage automatique pour le traitement automatique du langage naturel
* Conception d'assistants virtuels textuel ou vocal
* Conception d'interaction persuasive

## Projet Chatbot
2 cas d'utilisation :
* Prise de rendez-vous
* Chosir une bouteille chez un caviste

## Resume Cours
### Domaines TALN:
    Reconnaissance Automatique de la Parole (ASR)
    Synthèse de la Parole à partir du Texte (TTS)
    Speech To Speech translation (STS)
    Modélisation du Langage (LM)
    Compréhension du langage (LU)
    Dialogue Homme-Machine (MMD ==> HMD)
    Traduction (MT)
    Génération de texte
    
### Modèles reconnaissance parole par époque:
    1960-1970 : Dynamic Time Warping (DTW)
    1970-1980 : Hidden Markov Models (HMM)
    1986 : Multi-layer perceptron (MLP)
    1987-1995 : Réseaux de neurones (NN) de type MLP
    1995-2009 : Gaussian Mixture Models (GMMs)  
    1990-... : Recurrent networks
    2002-... : Deep Neural Networks (DNN)
    
### Fondement théorique:
    théorie des automates (Alan Turing, 1937)
    modélisation du neurone (Warren McCulloch et Walter Pitts, 1943)
    théorie de l’information (Claude Shannon, 1948)
    théorie des langages formels (Noam Chomsky, 1956)
    processus de Markov caché (ca 1960)
    algorithme forward
    algorithme de Viterbi (Andrew Viterbi, 1967)
    approche symbolique (IA) ca 1960
    approche bayesienne (ML) ca 1980
### ANN (Artificial Neural Network):
    (1ere époque)
    1843 : Du Bois-Reymond, mesure de l'activité électrique du cerveau
    1943 : Mc Culloch & Pitts, modélisation du neurone par un automate à seuil à 2 états = neurone formel
    1949 : Hebb, "The Organization of Behavior" ➜ première règle d'apprentissage
    1957 : Rosenblatt, "The Principles of Neurodynamics" ➜ modèle du Perceptron (reconnaissance de formes simples H / T)
    1958 : Turing, "Can Machines Think ?"
    1960 : Widrow & Hoff, ADALINE (ADAptative LINear Element)
    1969 : Minsky & Papert, Perceptrons - An Introduction to Computational Geometry ➜limitation : séparation linéaire

    (2eme époque)
    Néo-connexionnisme : paradigme de l’expert ➜ paradigme de l’enfant
    1981 : Teuvo Kohonen, "Self-Organization and Associative Memories" ➜ auto-organisation de cartes topologiques
    1982 : John Hopfield, "Neural Networks and Physical Systems with Emergent Collective Computational Abilities»
    1986 : David Rumelhart et Yann Le Cun : back-propagation algorithm
    1987 : Stephen Grossberg, "The Adaptative Brain", inspiration neurophysiologique
    1988 : Teuvo Kohonen, Learning Vector Quantization

    (3eme époque)
    1995 : Vladimir Vapnik, SVM
    1998 : Yann Le Cun, stochastic gradient descent algorithm
    2000 : Igor Aizenberg, mot *deep* pour les ANN
    2006 : Geoffrey Hinton, «I knew how the brain works» ➜  apprentissage non-supervisé  +  deep belief nets 
    2012 : Large Scale Visual Recognition Challenge(LSVRC) sur base de données Imagenet

    3 grands type de réseau:
        - feedforward
        - feedback / récurrent (RNN: Recurrent Neural Network, utilisation de donnée à t-1 pour calculer t, exemple: LSTM)
        - auto-organisation
    CTC (Connectionist Temporal Classification) ==> récurrent

### Interview Yann Le Cun:
    diff entre IA et IBiologique: un humain a un modèle du monde préexistant
    Il faudrait apprendre au machine de comprendre le monde en ne faisant que l'observer

### Interview Stanislas Dehaene:
    Les informations qui rentrent dans le cerveau est globale, pas limité à un module, contrairement à l'ensemble des applis d'une machine
    Le cerveau est conscient de ce qu'il ne sait pas
    Le cerveau est conscient de lui même, de ses limites

### TALN: Traitement Automatique du Langage Naturel
    => pluri disciplinaire (IA, informatique théorique, logique, linguistique, neuro-sciences, stats, ...
    motivations: modéliser le language (coté scientifique et théorique), obtenir des applications concrètes
    1962: première conf du MIT par Bar-Hillel
    depuis 1954: gros investissement sur le domaine
    ALPAC (Automatic Language Processing Advisory Council)
    Zellig Harris: publication travaux linguistique 1951-1954
    N. Chomsky: publie travaux sur syntaxe langue naturelle (hypothèses cognitions) 1957 
    Ross Quillian préconise utilisation de réseaux sémantique pour représenter sens des mots/phrases (représenter relations entre concepts)
    Tenny Winograd 1972 SHRDLU (logiciel dialogue avec micro monde)
    La signification dépend très étroitement d’informations qui font partie des connaissances des interlocuteurs
    ATN: Augmented Transition Network
    Une partie importante des travaux actuels cherchent à créer des mécanismes d'acquisition auto des connaissances de langage (logique par rapport à ce qui a été dit avant)
    Les difficultés que l’on rencontre en TALN ressortent soit de l’ambiguïté, soit de d’implicite

### Étapes de traitement (théorique, dans la réalité ces "étapes" communiques entre elles dans les deux sens):
* Segmentation du texte (en fonction de la langue, en fonction des points, espace, attention aux sigles, ...)
* Lexical (mots) (quel verbe, mot, quelle personne, singulier ?, pluriel ?, détecter des patternes dans les mots, stemme, suffixe flexionnel/dérivationnel)
* Syntaxique (relations entres mots) (grammaire, éliminer les séquences grammaticalement invalide, désigner le mots dominant, arbre syntaxique)
* Sémantique (représentation du sens) (réprésentation logique formelle avec des prédicats) 
* Pragmatique (associer ce qui a été énoncé dans le contexte courant, voir au dela de l'énoncé, limite par rapport au lore en fait ...)

applications: traitement documentaire, production de document, interfaces naturelles

1956: naissance IA (John Mc Carthy, MarvinMinsky, Allan Newell, Herbert Simon)