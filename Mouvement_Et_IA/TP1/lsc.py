import numpy as np # maths

def afficheMat(M):
    print('\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in M]))
    print('\n')

def construction_matrice(word1,word2):
    l1 = len(word1)+1
    l2 = len(word2)+1
    M = [[0 for i in range(l1)] for j in range(l2)]
    afficheMat(M)
    for i in range(1,l2):
        for j in range(1, l1):
            if(word1[j-1] == word2[i-1]):
                val = M[i-1][j-1] + 1
            else :
                val = max(M[i][j-1], M[i-1][j])
            M[i][j] = val
    return M, M[l2-1][l1-1]

def lsc(word1, word2):
    M,val = construction_matrice(word1,word2)
    afficheMat(M)
    print('la valeur de LCS est : ',val)

lsc('chien', 'neige')