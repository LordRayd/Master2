import numpy as np # maths

def afficheMat(M):
    print('\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in M]))
    print('\n')

def construction_matrice(word1,word2):
    l1 = len(word1)+1
    l2 = len(word2)+1
    M = [[0 for i in range(l1)] for j in range(l2)]
    for i in range(1,l2):
        for j in range(1, l1):
            if(word1[j-1] == word2[i-1]):
                val = M[i-1][j-1] + 1
            else :
                val = max(M[i][j-1], M[i-1][j])
            M[i][j] = val
    return M, M[l2-1][l1-1]

def choix_chemin(M,word1,word2):
    i = len(word2)
    j = len(word1)
    while i>0 or j>0 :
        M[i][j] = '   X'
        if(i==0):
            j = j-1
        elif(j==0):
            i = i - 1
        elif(M[i-1][j] >= M[i][j-1]):
            i = i-1
        else :
            j = j-1
    return M

def lsc(word1, word2):
    M,val = construction_matrice(word1,word2)
    afficheMat(M)
    print('la plus longue sous-séquence commune est : ',val)
    M2 = choix_chemin(M,word1,word2)
    afficheMat(M2)
lsc('neige', 'chien')
lsc('CATGT', 'ACGCTG')