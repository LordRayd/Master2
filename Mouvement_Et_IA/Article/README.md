# Presentation d'un article
## Choit de l'article
2018_CVPR_Cao_OpenPose.pdf
## Resumé
### Defis
* Chaque image contient un nombre de personnes non definit qui apparaissent a des positions et tailles différentes
* C'est difficile d'associer les *membres* des personnes a causes des interactions, de l'environnement spatial, de l'occlussion
* La complexité de calcul augmente avec le nombre de personne
### Approche existante
* **Top Down** : L'approche de base est de faire une detection (estimation) pour chaque personne détecter dans l'image. Le temps d'éxécution est proportionnel au nombre de personnes. Si la detection de personne echoue (proximité,..) alors on ne peut pas recupérer de données sur l'individu
* **Bottom Up** : Evitee les erreurs de detections de base et n'associe pas le temps d'exécution au nombre de personnes. N'utilise le contexte global des autres part du corps et personnes entrainant impliquant donc un calcul aussi long que le premier a cause de l'inférence global sur le calcul de fin.
### Prémices de la Methode 
Estimation de pose de multiple personne avec performance compétitive sur plusieurs Benchmark.
Une méthode Bottom Up(ascendante) qui utilise l'association des score avec les **Champs d'Affinités de partie** (PAF), un jeu de vecteur de champs 2D qui represente la location et l'orientation des membres sur l'image.
Ils montrent que les représentation ascendante de détection et d'association montrent un contexte global sufisant pour qu'une analyse gourmande obtienne des résultat de **hautes qualités** avec une fraction du cout de calcul.
### Element important du papier
* Ils provent que PAF est crucial pour maximisé l'accuracy alors que les données du corps ne le sont pas. En enlevant les calcul sur le corps et en gardant les membres ils augment la rapidité de 200% et l'accuracy de 7%.
* Ils presentent un jeu de données de 15k pieds humains labellisé. Et ils montrent que l'utilisation de la reconnaissance des pieds avec le modele corps n'entrainent pas un temps de calcul plus long que le model corps tout seul en conservant l'accuracy
* Ils montrent que leur méthode est généralisable en l'utilisant sur des véhicules
* Ils presentent OpenPose: une librairies Open Source : première librairies de détection de pose de multi-person en 2D en temps réels. Ce qui inclus les points clés du corps, des pieds, des mains et du visage
* Ils font aussi une comparaison de leur méthode avec Mask R-CNN et AlphaPose, qui montrent leur avantage. 
### Method
#### Architecture du reseau
* L'architecture prédit de manière itérative :
    * les champs d'affinité d'association de membre à membre
    * les cartes de confiance de detection
* L'architecture de prediction itérative affine la prediction au cours de plusieurs étapes
* L'ancien model avait des couches convolutionel de 7x7
* Le nouveau model remplace le 7x7 par 3 kernel consecutif de 3x3, résuidant le nombre d'opération de 97 à 51. La sortie des 3 kernel est concaténé en suivant l'approche de DenseNet. Le nomrbre de couche noon-linéaire est triplé et le reseau garde les features (particularité) de bas et haut niveaux.

#### Détection et association simultanné
* L'image est analysé par un CNN qui genere une liste de features qui est l'entrée de la première étape. Durant la première étape le résau produit un ensemble de champs d'affinité des membres.
À chaque étape suivante, les prédictions de l'étape précédente et les caractéristiques de l'image d'origine sont concaténées et utilisées pour produire des prédictions affinées.
Après avoir itérations sur le nombre total d'étape PAF, le processus est répété pour la détection des cartes de confiance, en commençant par la prédiction PAF la plus mise à jour.


### OpenPose
#### Présentation
* Le premier systeme de reconnaissance multi-personne en temps reel détectant le corps, les mains, les pieds et le visage.
* Il peut tourner sous tous OS et utilisé le CPU comme le GPU.
* L'utilisateur peut choisir en entrée une image, une vidéo, une webcam ou l'adresse IP d'une camera.
* Il peut choisir d'aficher ou de sauvegarder les resultats et qu'elle resultat il souhaite (liste au point 1), combien de GPU utiliser, si il doit passer des frames etc...
* OpenPose est séparable en 3 blocks :
    * detection corps + pied
    * detection mains
    * detection visage
* La composante principale étant la détection corps et pieds. OpenPose peut aussi utilisé la detection unique du corps entrainé sur les datasets de COCO et MPII.
* Grâce au résultat fournie par la detection du corps, en connaissant la position de certains membres(yeux, nez, oreilles,...) on peut deja estimer une bouding box du visage.
* De la même façon, on peut definir une bouding box des mains avec les postitions des bras.
* La bibliothèque comprend également la détection de pose 3D, en effectuant une triangulation 3D avec un raffinement de Levenberg-Marquardt non linéaire sur les résultats de plusieurs caméras synchronisées.
* L'inférence d'OpenPose dépasse tous les algorithmes à l'état de l'art tout en conservant un résultat de qualité.
* OpenPose peut tourner à 22 FPS sur une NVIDIA Gtx 1080 Ti.
* OpenCv à intégré OpenPose car il était utilisé par les chercheurs et est efficace.
#### Détection des pieds améliorer
Avant OpenPose, les datasets ne décrivait que les chevilles, le genoux, la hanche, l'épaule, le coude, le poignet, le cou, le torse et le dessus de tête.
Cependant, certains algorithmes on besoins de données plus précise au niveau des pieds tels que le gros orteil ou le talon.
Pour répondre au problème, ils ont extraits du datasets de COCO des pieds qu'ils ont labelissé avec la platforme Clickworker.
Au total 14k annotations, en 3D, on été ajoutés avec environ 3 annotations par pied, pour créé un dataset de pied.
Un modele de detection aurait pu être utilisé dans OpenPose cependant il aurait souffert des problemes du *Top Down*, c'est pour cela qu'il est intégré au detecteur de corps.
De plus la fusion des deux permet au detecteur de corps d'être plus précis, en particulier les jambes et chevilles.


### Resultat
Les chercheurs utilisent 3 benchmark de pose multi-personne différent pour évaluer leur algorithme:
* Le dataset MPII qui est composé de 3844 données d'entrainement et 1758 de validation, de positions très articulés, avec 14 partit du corps.
* Le dataset COCO qui impliquent de detecter en meme temps les personnes ainsi que leurs 12 membres du corps et 5 points du visage.
* Le dataset de pied qu'ils ont eu meme créé.
De plus ils ont pu observer que OpenPose permet une bonne qualité de résultat pour un temps de calcul réduit.
#### MPII
Sur l'ensemble du dataset MPI de test, OpenPose sans recherche sans échelle surpasse les méthodes à l'état de l'art d'environ  13% sur mAP.
En utilsant une recherche à 3 échelles augmente le resultat jusque 75.6% mAP.
La méthode d'analyse gourmande atteint une meilleure précision qu'une formule d'optimisation de coupe de graphe basée sur une structure de graphe entièrement connectée.
Ils on egalement entrainer leur modele sur grpahe entièrement connecté et on comparé les résultats en sélectionnant tout les côtés ou juste les bords de l'arbre minimum.
Les 2 méthodes ont eu des résultats similaires montrant l'utilité d'utilisé le moins de bord possible.
#### COCO
En utilisant la moyenne de la precision(AP) sur 10 OKS (similitude de point-clés d'objet) sur le jeu de données de validation de COCO (~ 20k images),
on peut voir que si nous utilisons le détecteur d'objet à l'état de l'art, Single ShotMultiBox Detector (SSD), les performances chutent de 10%. 
Cette comparaison indique que les performances des approches descendantes dépendent fortement du détecteur de personne. 
En revanche, leur méthode ascendante atteint 58,4% AP.

#### Propre jeu de donnée
Ils comparent également leur modele utilisant le corps et les pieds avec celui utilisant que le corps, et il s'apperçoivent que le modele combinés est moins efficace, sur les données de validation COCO et leur propre dataset.
Cependant ils affirment que cela est dû au nombre du jeu de données d'apprentissage qui n'est que de 14k annotations corps et pieds par rapport a 100k pour le corps.
Malgré tout les résultats obtenus sur le dataset "test-dev" de COCO reste très similaire lorsque l'on ne prend pas en compte les annotations des pieds pour le modele combiné qui est légèrement plus performant.

### Généralisation
Afin de démontrer que OpenPose ne se limite a la detection du corps huumain, ils l'ont appliqué à la détection de véhicule et obtienne des résultats plus que correct avec un score de 70% AP.

### Erreur
Le modele presenter n'est cependant pas parfait et il existe plusieurs cas qui ne permette pas une détection correct. 
Par exemple, le pied ou la jambe qui est occulté par le corps, ou encore des faux négatif quand le pied est visible mais le reste du corps ne l'est pas.