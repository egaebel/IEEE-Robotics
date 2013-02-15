

noOnes = True;
#WILL HAVE CONDITION IF OVER CERTAIN NUMBER OF 1.0'S WILL RETAKE PULSE (2 1.0's)
def isRed(L):
    if(noOnes):
        if L[0] > 15000 and L[2] > 15000 and L[3] > 15000:
            if(L[1]*5 < L[0] and L[1]*5 < L[2] and L[1]*5 < L[3]):
                return 1;
    elif L[0] == 1 and L[1] > 1 and L[2] > 1 and L[3] > 1:  #just white 1 case
        if L[2] > 25000 and L[3] > 25000 and abs(L[2] - L[3]) < L[2]/4:
            if L[2] > L[1]*5 and L[3] > L[1]*5:
                return 1;
    elif L[0] == 1 and L[2] == 1 and L[3] == 1 and L[1] < 10000: #case of 3 1's and low blue
        return 1;
        
def isBrown(L):
    if(L[3] > 80000 or L[2] > 80000): #only color with constistant higher than 90k r/g values
        if(L[1] > 35000):
            return 1;
    elif L[0] == 1 and L[2] == 1 and L[3] == 1: #Special case of 3 1's and blue (becasue really dark color)
        if L[1] > 35000:
            return 1;

def isYellow(L):
    if L[0] < 10000 and L[1] < 10000 and L[2] < 10000 and L[3] < 10000 and noOnes: #yellow has smallest values
       if abs(L[0] - L[2]) < L[0]/3 and abs(L[3] - L[2]) < L[2]/3 and abs(L[0] - L[3]) < L[3]/3: # w, r, g all fairly close (not nescessary since no other meets above if statement condition
            return 1;
        
def isBlue(L):
    if noOnes and L[1] > L[0] and L[1] > L[2] and L[1] > L[3]: #Only one where blue consistently greatest when no all_others_ones NEED NO ONES TO DIFFERENTIATE FROM BROWN CASE w/ 3 1's and high blue
        return 1;

def isPurple(L): #OR IF ONE OF THEM IS 1.00
    if L[0] > 15000 and L[1] > 15000 and L[2] > 15000 and L[3] > 15000: #to differ frm yellow
        if noOnes and abs(L[0] - L[2]) < L[2]/12 and abs(L[0] - L[3]) < L[0]/12 and abs(L[2] - L[3]) < L[3]/12: #w, r, g very close in value
            if L[1] < L[0] and L[1] < L[2] and L[1] < L[3]: #Assure blue pulse smaller
                if L[1]*2 > L[0] and L[1]*2 > L[2] and L[1]*2 > L[3]: #but not as relative small as red color detect
                    return 1;
#def isGreen(*pulse):

print ("hello") 

c = [int(input("White = ")), int(input("Blue = ")),  int(input("Red = ")), int(input("Green = "))];

if(isRed(c) == 1):
    print ("Red PASS");

if(isBrown(c) == 1):
    print ("Brown PASS");

if(isYellow(c) == 1):
    print ("Yellow PASS");

if(isBlue(c) == 1):
    print ("Blue PASS");

if(isPurple(c) == 1):
    print ("Purple PASS");

