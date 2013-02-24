def numOnes(A):
    count = 0;
    for i in (0, 4):
        if(A[i] == 1):
            count+=1;
    return count;

def isRed(L):
    if((abs(L[0] - L[2]) < 2000) and (abs(L[2] - L[3]) < 2000) and (abs(L[1] - L[3]) < 2000)):
        if((L[1]*5) < L[0]):
            return True;

def isBrown(L):
    if(numOnes == 3 and L[1] > 15000):
        return True;

def isYellow(L):
    if L[0] < 10000 and L[1] < 10000 and L[2] < 10000 and L[3] < 10000 and  (L[0] > 2) and (L[1] > 2) and (L[2] > 2) and (L[3] > 2): #yellow has smallest values
       if abs(L[0] - L[2]) < L[0]/3 and abs(L[3] - L[2]) < L[2]/3 and abs(L[0] - L[3]) < L[3]/3: # w, r, g all fairly close (not nescessary since no other meets above if statement condition
            return 1;
        
#def isYellow(L):
#   if(L[0] < 10000 and L[1] < 10000 and L[2] < 10000 and L[3] < 10000):
#        if((abs(L[0] - L[2]) < (L[2]/7)) and (abs(L[2] - L[3]) < (L[3]/7)) and (abs(L[1] - L[3]) < (L[1]/7))):
#            return True;

def isBlue(L):
    if(L[1] > L[0] and L[1] > L[2] and L[1] > L[3]):
        if(((L[1] - L[0]) > abs(L[0] - L[2])) and ((L[1] - L[0]) > abs(L[0] - L[3])) and ((L[1] - L[0]) > abs(L[2] - L[3]))):
            return True;

def isPurple(L):
    if((L[0] > L[1]) and (L[2] > L[1]) and (L[3] > L[1]) and ((L[0]*4) > (L[1]*5))):
        return True;

def isGreen(L):
    if((abs(L[0] - L[1]) < 3000) and (abs(L[1] - L[2]) < 3000) and (abs(L[2] - L[3]) < 3000) and (abs(L[3] - L[1]) < 3000)):
       if(L[0] > 15000 and L[1] > 15000 and L[2] > 15000 and L[3] > 15000):
           return True;
       
           

print ("hello") 

c = [int(input("White = ")), int(input("Blue = ")),  int(input("Red = ")), int(input("Green = "))];

if(isRed(c)):
    print ("Red PASS");

if(isBrown(c)):
    print ("Brown PASS");

if(isYellow(c)):
    print ("Yellow PASS");

if(isBlue(c)):
    print ("Blue PASS");

if(isPurple(c) == 1):
    print ("Purple PASS");

if(isGreen(c) == 1):
    print ("green PASS");




