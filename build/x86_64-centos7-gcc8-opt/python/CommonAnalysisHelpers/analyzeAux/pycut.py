import QFramework
import ROOT
import os

class PyCut:
    def __init__(self,name,cut= "1", weight = "", options={}):
        """
        Create a new cut with the specified name, cut expression, and/or weight. 
        A dict of options can be specified in which keys must be strings, values
        must be boolean, numeric (int or float), or strings. Options that will
        have an effect are the same as when creating a TQCut from a TQFolder.
        Note that for a cut to take effect it must be added to an existing cut!
        (the very first base cut is provided by the framework)
        """
        #note for internal use: name can also be a TQFolder if the Cut is created based on an existing folder
        #the graph structure should always be encoded in TQFolders, the PyCut class is merely a convenient wrapper 
        #for users
        if isinstance(name, QFramework.TQFolder):
            self.folder = name
            return None #ctor must return None as by python standard
            
        if not isinstance(name,basestring):
            raise TypeError("PyCut name must be a string, got '{:s}'",str(type(name)))
        self.folder = QFramework.TQFolder(name)
        for key,val in options.items():
            self.folder.setTag(key,val)
        #set these last
        self.folder.setTagString(".cutExpression",cut)
        self.folder.setTagString(".weightExpression",weight)
        
    def __iadd__(self,other):
        #addition with re-asignment (foo += bar)
        self.folder.addFolder(other.folder)
        return other
    def __add__(self,other):
        self.folder.addFolder(other.folder)
        return other
        
    def __eq__(self,other): # method implementing comparison via '==' operator
        if isinstance(other,type(self)):
          return self.folder == other.folder
        return False
        
    def is_(self,other): # method implementing comparison via 'is' operator
        if isinstance(other,type(self)):
          return self.folder is other.folder
        return False
        
    def get_root(self):
        return PyCut(self.folder.getRoot())
    
    def printTree(self):
        r = self.folder.getRoot()
        r.printContents("rdt")
    
    def find(self,key):
        if self.folder.GetName() == key:
            return self
        resFolder = self.folder.getFolder(os.path.join("*",key))
        if resFolder:
            return PyCut(resFolder)
        raise KeyError("No cut with name '{:s}' found".format(key))
        return None
        
    def __getitem__(self,key):
        root = self.get_root()
        return root.find(key)
    def __setitem__(self,key,val):
        pass
