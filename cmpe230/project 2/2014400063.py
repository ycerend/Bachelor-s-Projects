import os
import sys
from collections import deque
import re



class cFiles(object): #it generates an object for every c file with its path, name and included files
    path=""
    name=""
    includeFiles=[]
    def __init__(self,path,name,includeFiles): #initializes the c file object
        self.path=path
        self.name=name
        self.includeFiles=includeFiles

def make_cFiles(path,name,includeFiles): #method for generating a c file object
    cFile = cFiles(path,name,includeFiles)
    return cFile

class hFiles(object): #it generates an object for every h file with its path and name
    path= ""
    name= ""
    def __init__(self,path,name):#initializes the h file object
        self.path = path
        self.name = name

def make_hFiles(path,name):#method for generating a h file object
    hFile=hFiles(path,name)
    return hFile

def generateMakeFile(cFilesAll, hFilesAll): #generates the makefile file
    headers=[]                              #keeps all the headers of .h files that will be used during generating makefile
    with open(os.path.join(cwd,'makefile'),'w') as out:  #generates the makefile file and writes in it
        toBeWritten="program: "                 #toBeWritten keeps the lines for the makefile, this line writes the execution of .o files
        for cFiles in cFilesAll:                   #this for loop writes execution of .o files of .c files to toBeWritten 
            toBeWritten += cFiles.name[0:-2] +".o "
        toBeWritten += "\n"
        out.write(toBeWritten)                      #this line writes the toBeWritten string to makefile file
        toBeWritten="\tcc "
        for cFiles in cFilesAll:                    #this for loop writes the second line of execution of the .c programs
            toBeWritten += cFiles.name[0:-2] +".o "
        toBeWritten +="-o program \n\n"
        out.write(toBeWritten)  #this line writes the toBeWritten string to makefile file
        for cFiles in cFilesAll:                #this for loop writes the decleration of every .o executable files that will be generated from .c files
            toBeWritten= cFiles.name[0:-2] +".o: "+ cFiles.path + "/" +cFiles.name  #this line writes the first line of the decleration of every .c file
            includeFiles = cFiles.includeFiles    #includeFiles keeps the .h files that is included in .c file
            for included in includeFiles:           #it writes the included files to the decleration of .c files
                for realfiles in hFilesAll:
                    name= realfiles.name            #it finds the path of the .h file from hFilesAll(it is an array that keeps the all h files with its name and its path)
                    if included == name:            
                        headers.append(realfiles.path)      #it appends the path of header file to headers array
                        toBeWritten += " "+realfiles.path+"/" +realfiles.name
                       
            toBeWritten+="\n"
            out.write(toBeWritten) #this line writes the toBeWritten string to makefile file
            toBeWritten = "\tcc -c "
            headers= list(set(headers))  #it removes the duplicates
            for included in headers:  #it generates the second line of the decleration of .o files
                toBeWritten += " -I"+included
            toBeWritten += " "+cFiles.path + "/" +cFiles.name +"\n\n"
            out.write(toBeWritten)  #this line writes the toBeWritten string to makefile file
            headers=[]  #initializing header file for each .c file
        



cwd =os.path.abspath(sys.argv[1])  #it takes the full path of taken 
qlist = deque( [cwd]) #takes the path from the cwd
cFilesAll=[]  #keeps all the c file objects that keeps every c files' name,path and included files
hFilesAll=[]  #keeps all the h file objects that keeps every h files' name and path
includeFiles=[]  #keeps the every c files' included files locally when directories are traversing
cFilesNames=[]  #keeps all the c files' names
hFilesNames=[]  #keeps all the h files' names
notIncludedError =False #keeps the error information
totalIncludedFiles=[]  #keeps the total included files that included in all c files
while qlist:  #traverses the directory
  currentdir =  qlist.popleft() 
  dircontents = os.listdir(currentdir)
  for name in dircontents:
    currentitem = currentdir + "/" + name
    if os.path.isdir(currentitem):
        qlist.append(currentitem)
    else:
        if currentitem.endswith('.c'): #if the current file is a c file it goes in there
                cFilesNames.append(name)  #adds the name of the c file to the c file names array
                file = open(currentitem, "r") #opens the c file to reach included files
                for line in file: #search the #included lines
                    if "#include" in line: #if there a included line exists goes in there
                        if re.findall(r'"([^"]*)"', line): #checks if there is .h file included
                            includeFiles.append(re.findall(r'"([^"]*)"', line)) #adds the found included file to the includeFiles
                            totalIncludedFiles.append(re.findall(r'"([^"]*)"', line))#adds the found included file to the total includeFiles
                realIncluded=[item for sublist in includeFiles for item in sublist]  #removes the duplicates
                oneCFile = make_cFiles(currentdir, name, realIncluded) #generating c file object with its path, name and included files
                cFilesAll.append(oneCFile) #adds the c file object to the array
                includeFiles = [] #initializes the includeFiles for the next c file
        elif currentitem.endswith('.h'):
            hFilesNames.append(name) #adds the name of the h file to the h file names array
            oneHFile= make_hFiles(currentdir,name)#generating h file object with its path and name 
            hFilesAll.append(oneHFile)#adds the h file object to the array

flat_list=[item for sublist in totalIncludedFiles for item in sublist]  #removes the duplicates in total included files
flat_list=list(set(flat_list)) #making a list from unlisted total included files



for cfiles in cFilesAll:  #checks the errors whether the included file is not found
    included=cfiles.includeFiles
    for includedHfiles in included:
        if includedHfiles not in hFilesNames:
            notIncludedError=True  #if there is an file included in c file and not found it raises an error
            print("included" + includedHfiles+ " in " + cfiles.name +" is not found! Error!")

for hfiles in hFilesAll: #checks the unused .h files
    hFileName= hfiles.name
    if hFileName not in flat_list: 
        print(hFileName +" is not used in any .c file! Warning!") #if there is a warning situation it raises a warning 


if not notIncludedError: #if there is no error it generates the makefile
    generateMakeFile(cFilesAll,hFilesAll)

