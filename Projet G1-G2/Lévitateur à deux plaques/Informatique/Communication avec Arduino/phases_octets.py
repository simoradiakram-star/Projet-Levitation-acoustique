import math as m

# Constantes
N_TRANSDUCTEUR = 80
N_DIVS = 10
N_PORTS = 8
N_T = N_TRANSDUCTEUR // N_PORTS

# Initialisation des phases
Phi = [0.0 for i in range(N_TRANSDUCTEUR)]

Phi[3]=m.pi

# Création de la matrice binaire oct_T
oct_T = [[0 for i in range(N_DIVS)] for j in range(N_TRANSDUCTEUR)]

for i in range(N_TRANSDUCTEUR):
    for j in range(N_DIVS):
        angle = 2 * m.pi * j / N_DIVS + Phi[i]
        value = m.sin(angle)
        oct_T[i][j] = 0 if value <= 0 else 1

# Création de la structure oct_P
oct_P = [[[] for i in range(N_PORTS)] for j in range(N_DIVS)]

for j in range(N_DIVS):
    for i in range(N_PORTS):
        for k in range(N_T):
            oct_P[j][i].append(oct_T[i * N_T + k][j])

# Construction du buffer (octets à envoyer)
buffer = [0 for i in range(N_DIVS * N_PORTS)]

for i in range(N_DIVS):
    for j in range(N_PORTS):
        value = 0
        for k in range(8):  # on regroupe 8 bits dans un octet
            value = (value << 1) | (oct_P[i][j][k] & 1)
        buffer[i * N_PORTS + j] = value

i=1
for val in buffer :
    print(format(val,'08b'),end=' ')
    if (i%N_PORTS ==0) : print ()
    i+=1
