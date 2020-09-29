import numpy as np # maths

def afficheMat(M):
    print('\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in M]))
    print('\n')

def construction_matrice(word1,word2):
    l1 = len(word1)+1
    l2 = len(word2)+1
    M = [[0 for i in range(l1)] for j in range(l2)]
    for i in range(l2):
        M[i][0] = i
    for i in range(l1):
        M[0][i] = i
    afficheMat(M)
    for i in range(1,l2):
        print('index i : ', i)
        for j in range(1, l1):
            print('index j : ', j)
            val = min( min(M[i-1][j-1]+1, M[i][j-1]+1), M[i-1][j]+1)
            if(word1[j-1] == word2[i-1]):
                val = min(val, M[i-1][j-1])
            M[i][j] = val
    return M, M[l2-1][l1-1]

def choix_chemin(M,word1,word2):
    i = len(word2)
    j = len(word1)
    while i>0 or j>0 :
        M[i][j] = 0
        if(word1[j-1] == word2[i-1]):
            i = i-1
            j = j-1
        else :
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
    return M

def lenvenshtein(word1, word2):
    M, val = construction_matrice(word1,word2)
    print('la valeur de levenshtein est : ',val)
    afficheMat(M)
    M = choix_chemin(M,word1,word2)
    afficheMat(M)

lenvenshtein('chien', 'neiger')