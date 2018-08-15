import random as rd
def LecturaVoltaje(name,x):
    if name is 'bateria':
        return 32+x
    return rd.randrange(67,87,3)

def LecturaCorriente(X):
    if X is 3:
        return 3
    if X is 30:
        return 30
    if X is 24:
        return 24
    return X-2
    

def SecuenciaDeMuestreo(Sol,Data,Bat,T):
#Lectura de voltaje retorna un valor de voltaje la idea es tener tres cordenadas en cada 
# secuencia de muestreo
## una generacion de n Valores de muestre caracteristicos 
    Data={}
    Sol={}
    Bat={}
    for i in range(T):
        if T%4 is 0:
            Bat[i]=LecturaVoltaje("bateria",2)
        Sol[i]=LecturaVoltaje("Solar",1)
        if Sol[i] < 40:
            Data[i]=LecturaCorriente(3)
        if Bat[i] < 40:
            if Sol[i] > 70:
                Data[i]=LecturaCorriente(30)
            if Sol[i] > 60:
                Data[i]=LecturaCorriente(24)
        if Bat[i] is 48:
            Data[i]=LecturaCorriente(3)
        if Bat[i]>48:
            Data[i]=LecturaCorriente(30-i)

    return Sol,Data, Bat

def GeneradoCurva(C):
    return C

def Activador(C,D):
    return C, D

S={}
D={}
B={}
S,D,B=SecuenciaDeMuestreo(S,D,B,30)
for i in range(30):

    print('Solar',S[i],'corriente',D[i],'bateria',B[i])
    #print('\n')
    
    #print('\n')
    