# TP 4 
## Théorie
on crée un tableau 4d ou l'on store (X,3,3,103) :
on prend 1 pixel et le pixel a gauche et à droite et dans le fond et on le stocke et ça pour tous les pixels

Dans le result ça fait un tableau de 18 et des poussière tableau 3d de 3 * 3 * 103 elements


## Meilleur Algo
### 1 - Overall Accuracy = 99.99, Kappa = 1.000, epoch=20
```
Layer (type)                 Output Shape              Param #   
=================================================================
conv2d_3 (Conv2D)            (None, 32, 32, 32)        4032      
_________________________________________________________________
max_pooling2d_3 (MaxPooling2 (None, 16, 16, 32)        0         
_________________________________________________________________
conv2d_4 (Conv2D)            (None, 16, 16, 64)        18496     
_________________________________________________________________
max_pooling2d_4 (MaxPooling2 (None, 8, 8, 64)          0         
_________________________________________________________________
conv2d_5 (Conv2D)            (None, 8, 8, 128)         73856     
_________________________________________________________________
max_pooling2d_5 (MaxPooling2 (None, 4, 4, 128)         0         
_________________________________________________________________
flatten_1 (Flatten)          (None, 2048)              0         
_________________________________________________________________
dense_2 (Dense)              (None, 128)               262272    
_________________________________________________________________
dense_3 (Dense)              (None, 10)                1290      
=================================================================
Total params: 359,946
Trainable params: 359,946
Non-trainable params: 0
```

### 2 - Overall Accuracy = 99.99, Kappa = 1.000, patch de 32*32, epoch=156
Batchs de 24 car perte d'infos avec des patchs trop grands<br>
PCA afin de garder 17 spectres
```
Layer (type)                 Output Shape              Param #   
=================================================================
conv2d_79 (Conv2D)           (None, 17, 17, 128)       139392    
_________________________________________________________________
conv2d_80 (Conv2D)           (None, 12, 12, 256)       1179904   
_________________________________________________________________
conv2d_81 (Conv2D)           (None, 9, 9, 512)         2097664   
_________________________________________________________________
max_pooling2d_28 (MaxPooling (None, 4, 4, 512)         0         
_________________________________________________________________
flatten_27 (Flatten)         (None, 8192)              0         
_________________________________________________________________
dense_70 (Dense)             (None, 1024)              8389632   
_________________________________________________________________
dense_71 (Dense)             (None, 512)               524800    
_________________________________________________________________
dense_72 (Dense)             (None, 9)                 4617      
=================================================================
Total params: 12,336,009
Trainable params: 12,336,009
Non-trainable params: 0
```