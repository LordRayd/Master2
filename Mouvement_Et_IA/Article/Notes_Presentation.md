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