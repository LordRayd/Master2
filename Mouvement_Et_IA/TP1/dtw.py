import numpy as np # maths

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

#def elastic(s1,s2):
#comprend pas
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




#res = distance_eucli(signal1,signal2)

res = mat_dist(signal1, signal2)
print(signal1)
print(signal2)
print(res)