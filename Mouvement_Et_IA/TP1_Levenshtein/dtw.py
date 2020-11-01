import numpy as np # maths

# Fonction fileToList écrite par Romann Yvinec avec son accord d'utilisation
def fileToList(file):
    f=open(file)
    text=f.read()
    filteredList=filter(lambda a : a!= "",text.split(" "))
    return list(map(float,filteredList))

def afficheMat(M):
    print('\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in M]))
    print('\n')

signal1 = np.linspace(4,20,6)
signal2 = np.linspace(4,20,8)

def distance_eucli(s1, s2):
    l1 = len(s1)
    l2 = len(s2)
    M = [[0 for i in range(l2)] for j in range(l1)]
    for i in range(l1):
        for j in range(l2):
            M[i][j] = np.sqrt((s1[i] - s2[j]) ** 2)
    return M

def mat_dist(s1, s2):
    l1 = len(s1)
    l2 = len(s2)
    D = [[0 for i in range(l2)] for j in range(l1)]
    for i in range(l1):
        for j in range(l2):
            D[i][j] = np.sqrt((s1[i] - s2[j]) ** 2)
            val = 0
            if i > 0 :
                if j > 0 :
                    val = min(D[i-1][j-1] , min(D[i-1][j] , D[i][j-1]))
                else :
                    val = D[i-1][j]
            elif j > 0 :
                    val = D[i][j-1]
            D[i][j] = D[i][j] + val
    return D

def choix_chemin(M):
    j = len(M[0])-1
    i = len(M)-1
    while i > 0 or j > 0 :
        M[i][j] = 'X'
        if(M[i-1][j] < M[i-1][j-1]):
            if(M[i-1][j] < M[i][j-1]):
                i = i-1
            else :
                j = j-1
        else:
            if(M[i-1][j-1] < M[i][j-1]):
                i = i-1
                j = j-1
            else:
                j=j-1
    M[0][0] = 'X'
    return M

def calc_sum(s1,s2) :
    M = distance_eucli(s1,s2)
    res = []
    if len(M[0])-1 == len(M)-1 :
        val = 0
        for i in range(len(M)-1):
            val = val + M[i][i]
            res.append(val)
    return np.asarray(res), val

def dtw():
    print('================= Dynamique Time Warping ========================')
    #print(signal1)
    #print(signal2)
    fc11 = fileToList('./DATA_DTW/FC1_1.txt')
    fc12 = fileToList('./DATA_DTW/FC1_2.txt')
    res = mat_dist(fc11, fc12)
    print(res)
    print(calc_sum(fc11,fc12))
    M_choix = choix_chemin(res)
    print(np.matrix(M_choix))

dtw()