import numpy as np # maths

def afficheMat(M):
    print('\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in M]))
    print('\n')

def lsc(word1, word2):
    l1 = len(word1)+1
    l2 = len(word2)+1
    M = [[0 for i in range(l1)] for j in range(l2)]
    for i in range(1,l1):
        for j in range(1, l2):
            val = min( M[i][j-1]+1, M[i-1][j]+1)
            if(word1[i-1] == word2[j-1]):
                val = M[i][j] = M[i-1][j-1] + 1
            M[i][j] = val
    afficheMat(M)

lsc('neige', 'chien')