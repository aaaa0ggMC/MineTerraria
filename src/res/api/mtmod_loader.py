'''
Import Suggestions:
    from mtmod_loader import *
Using Docs:
Loading dods has many steps
While in version 1.0,it has only 1 step:
    Initializing
    
Initializing:
    To init,you need to have a function OnInit()
'''
# We suggest you not to change mlc directly
class ModLoaderConfig:
    #Strings that shows on the menu
    menuStrings = []
    # Very Warning:Do not change this value,or mod loading proc will fail!
    # now loading proc!DO NOT CHANGE IT!!THIS IS VERY IMPORTANT DATA
    nowProc = 0
    # the mod's log
    modLog = ''
    #the statuses
    menuLock = False
'''
The Manifest file's attributes
You can easily get them
'''
class ManifestFileSets:
    # Mod Name
    ModName = ""
    # This Mod's Version
    ModVersion = ""
    # The Mod's Creator
    ModAuthor = ""
    # The Mod's Description
    ModDescription = ""
    # The Only Package ID,Make sure this id is the only one!!!!
    ModPackage = ""
    
mlc = ModLoaderConfig()
mf = ManifestFileSets()
apiVersion = "api-1.0"

MenuLocked = 2
NowIsNotIniting = 1
Suc = 0
Fail = -1

'''
Lock the menu,other mods cannot change menuString if they follows the usual steps
'''
def lockMenu():
    mlc.menuLock = True

'''
Set the strings shows upon the menu
'''
def setMenuString(menuStr):
    if(mlc.menuLock):
        return MenuLocked
    if(mlc.nowProc == 0):
        mlc.menuStrings = menuStr
        return Suc
    else:
        return NowIsNotIniting
def sms(menuStr):
    return setMenuString(menuStr)

'''
Append the strings that shows on the menu
'''
def appendMenuString(nstr):
    mlc.menuStrings.append(str(nstr))
def ams(nstr):
    appendMenuString(nstr)

'''
Get the strings that shows on the menu
'''
def getMenuString():
    return mlc.menuStrings
def gms():
    return mlc.menuStrings
'''
Get the loading proc now
'''
def getLoadingProcess():
    return mlc.nowProc
def glp():
    return mlc.nowProc
'''
Write the program log
'''
def writeModLog(strlg):
    mlc.modLog += str(strlg)
def wml(strlg):
    writeModLog(strlg)
