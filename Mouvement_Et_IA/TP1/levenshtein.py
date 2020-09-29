import numpy as np # maths

def lenven(word1, word2):
    l1 = len(word1)+1
    l2 = len(word2)+1
    M = [[0 for i in range(l1)] for j in range(l2)]
    for i in range(l1):
        M[i][0] = i
        M[0][i] = i
    print(M)

lenven('chien', 'neige')