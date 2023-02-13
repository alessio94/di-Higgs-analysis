#!/usr/bin/env python2

######################################################################
# This script renders documentation page for tags in the QFramework.
# It takes a list of files as input and searches them for the
# occurence of "//@tag:" and "//@tags:" for one-line comments or
# "/*@tag:" and "/*@tags:" for multi-line comments. A table
# documenting all tags and the corresponding description is created
# as html and tex file.
#
# The script is easily expandable to other keywords than "tag" by
# adding keywords to the list "triggerTypes" a few lines below.
# Currently enabled keywords:
# - tag (tags read from all kinds of TQTaggable objects)
# - observable (mainly factory based Observables and their keywords)
# - env (environment variables)
######################################################################

import argparse
import sys


# the alias is the 'appliation name' that will be dumped to the info tags of any
# sample folder created
alias = "makeQTagDocumentation"

triggerTypes = ["tag", "observable","env"]

triggerSingle = {}
triggerSinglePy = {}
triggerMulti  = {}
for triggerType in triggerTypes:
  triggerSingle[triggerType] = ["//@"+triggerType,"//@"+triggerType+"s","// @"+triggerType,"// @"+triggerType+"s"]
  triggerSinglePy[triggerType] = ["#@"+triggerType,"#@"+triggerType+"s","# @"+triggerType,"# @"+triggerType+"s"]
  triggerMulti[triggerType] =  ["/*@"+triggerType,"/*@"+triggerType+"s","/* @"+triggerType,"/* @"+triggerType+"s"]
triggerMultiEnd = "*/"

classList = {}
tagList = {}
descList = {}
for triggerType in triggerTypes:
  classList[triggerType] = []
  tagList[triggerType] = []
  descList[triggerType] = []


def findTriggerLengthAndType(line, triggerList, patternAfterTrigger):
  triggers = []
  thisTriggerType = ""
  for triggerType in triggerTypes:
    for a in triggerList[triggerType]:
      if line.startswith(a+patternAfterTrigger):
        if thisTriggerType != "" and triggerType != thisTriggerType:
          print "A comment was parsed and passed different triggers ("+thisTriggerType+", "+triggerType+"). Your trigger names seem to be ambiguous. Choose different trigger names."
        thisTriggerType = triggerType
        triggers.append(a)
  triggerLen = max(len(a) for a in triggers)
  return triggerLen, thisTriggerType

def main(args):
  pathList = args.path
  errorFound = False

  for path in pathList:
    fname = TQFolder.getPathTail(TString(path)).Data()
    print ("parsing file '{:s}'".format(fname))

    commentFoundInCurrentFile = {}
    for triggerType in triggerTypes:
      commentFoundInCurrentFile[triggerType] = False
    with open(path) as f:
      content = f.readlines()
      isPython = path.endswith(".py")
      lineNumber = 0
      while len(content) > 0 :
        line = content.pop(0)
        lineNumber = lineNumber+1
        
        #first look for starting comments
        posSingle = len(line)+1
        posMulti  = len(line)+1
        if isPython:
          posSingle = min([x for x in [TQStringUtils.findFree(TString(line),TString(a+":"),TString("\"\"''")) for triggerType in triggerTypes for a in triggerSinglePy[triggerType]] if x >= 0] + [len(line)+1])
        else:
          posSingle = min([x for x in [TQStringUtils.findFree(TString(line),TString(a+":"),TString("\"\"''")) for triggerType in triggerTypes for a in triggerSingle[triggerType]] if x >= 0] + [len(line)+1])
          posMulti  = min([x for x in [TQStringUtils.findFree(TString(line),TString(a+":"),TString("\"\"''")) for triggerType in triggerTypes for a in triggerMulti[triggerType]]  if x >= 0] + [len(line)+1])

        if not (posSingle == (len(line)+1) and posMulti == (len(line)+1)):

          if (posSingle<posMulti):
            #we have a single line comment
            hasContinue = False
            line = line[posSingle:]

            triggerLen, triggerType = findTriggerLengthAndType(line, triggerSinglePy if isPython else triggerSingle, ":")
            line = line[triggerLen+1:]

            if line.endswith("\\\n"):
              hasContinue = True
              line = line[:-2]

            while hasContinue:
              tmpLine = content.pop(0)
              lineNumber = lineNumber+1
              hasContinue = False
              cPos = kNPOS
              if isPython:
                cPos = TQStringUtils.findFree(TString(line),TString("#"),TString("\"\"''"))
              else:
                cPos = TQStringUtils.findFree(TString(line),TString("//"),TString("\"\"''"))
              if cPos == kNPOS :
                print "Error: no comment found in line {:d} following a line continuation sequence ('\\\\n') on the previous line!"
                continue
              if tmpLine.endswith("\\\n"):
                hasContinue = True
                tmpLine = tmpLine[:-2]
              tmpLine = tmpLine[cPos+2:]
              tmpLine = tmpLine.strip()
              if tmpLine == '':
                tmpLine ="\n"
              else:
                tmpLine = " "+tmpLine
              line += tmpLine
            parseLine(fname, line, triggerType)
            commentFoundInCurrentFile[triggerType] = True

          else: 
            #we have a multi line comment
            line = line[posMulti:]
            triggerLen, triggerType = findTriggerLengthAndType(line, triggerMulti, ":")
            line = line[triggerLen+1:]

            hasContinue = True
            endPos = line.find(triggerMultiEnd)
            if endPos >= 0:
              hasContinue = False
              line = line[:endPos]

            while (hasContinue and len(content) > 0) :
              tmpLine = content.pop(0)
              lineNumber = lineNumber+1
              endPos = tmpLine.find(triggerMultiEnd)
              if endPos >= 0:
                hasContinue = False
                tmpLine = tmpLine[:endPos]
              tmpLine = tmpLine.strip()
              if tmpLine == '':
                tmpLine ="\n"
              else:
                tmpLine = " "+tmpLine
              line += tmpLine
            parseLine(fname, line, triggerType)
            commentFoundInCurrentFile[triggerType] = True

        #then look for continuing comments (without a ":" after the trigger)
        if isPython:
          posSingle = min([x for x in [TQStringUtils.findFree(TString(line),TString(a),TString("\"\"''")) for triggerType in triggerTypes for a in triggerSinglePy[triggerType]] if x >= 0] + [len(line)+1])
          posMulti  = len(line)+1 #set to max (= "not found"), as this variant is not supported for python scripts
        else:
          posSingle = min([x for x in [TQStringUtils.findFree(TString(line),TString(a),TString("\"\"''")) for triggerType in triggerTypes for a in triggerSingle[triggerType]] if x >= 0] + [len(line)+1])
          posMulti  = min([x for x in [TQStringUtils.findFree(TString(line),TString(a),TString("\"\"''")) for triggerType in triggerTypes for a in triggerMulti[triggerType]]  if x >= 0] + [len(line)+1])

        
        if not (posSingle == (len(line)+1) and posMulti == (len(line)+1)):
          
          if (posSingle<posMulti):
            #we have a single line comment
            hasContinue = False
            line = line[posSingle:]
            triggerLen, triggerType = findTriggerLengthAndType(line, triggerSinglePy if isPython else triggerSingle, "")
            if not commentFoundInCurrentFile[triggerType]:
              print "WARNING: You are trying to append something to a previous comment, but there is no previous comment. Did you forget a \":\" after your trigger keyword? Skipping this comment for now."
              errorFound = True
              continue
            line = line[triggerLen:]
            
            if line.endswith("\\\n"):
              hasContinue = True
              line = line[:-2]
            
            while hasContinue:
              tmpLine = content.pop(0)
              lineNumber = lineNumber+1
              hasContinue = False
              cPos = TQStringUtils.findFree(TString(line),TString("//"),TString("\"\"''"))
              if cPos == kNPOS :
                print "Error: no comment found in line {:d} following a line continuation sequence ('\\\\n') on the previous line!"
                continue
              if tmpLine.endswith("\\\n"):
                hasContinue = True
                tmpLine = tmpLine[:-2]
              tmpLine = tmpLine[cPos+2:]
              tmpLine = tmpLine.strip()
              if tmpLine == '':
                tmpLine ="\n"
              else:
                tmpLine = " "+tmpLine
              line += tmpLine
            line.lstrip()
            descList[triggerType][len(descList[triggerType])-1] += " "+line
            
          else: 
            #we have a multi line tag comment
            line = line[posMulti:]
            triggerLen, triggerType = findTriggerLengthAndType(line, triggerMulti, "")
            if not commentFoundInCurrentFile[triggerType]:
              print "WARNING: You are trying to append something to a previous comment, but there is no previous comment. Did you forget a \":\" after your trigger keyword? Skipping this comment for now."
              errorFound = True
              continue
            line = line[triggerLen+1:]
            hasContinue = True
            endPos = line.find(triggerMultiEnd)
            if endPos >= 0:
              hasContinue = False
              line = line[:endPos]
              
            while (hasContinue and len(content) > 0) :
              tmpLine = content.pop(0)
              lineNumber = lineNumber+1
              endPos = tmpLine.find(triggerMultiEnd)
              if endPos >= 0:
                hasContinue = False
                tmpLine = tmpLine[:endPos]
              tmpLine = tmpLine.strip()
              if tmpLine == '':
                tmpLine ="\n"
              else:
                tmpLine = " "+tmpLine
              line += tmpLine
            line.lstrip()
            descList[triggerType][len(descList[triggerType])-1] += " "+line

  for triggerType in triggerTypes:
    table = TQTable("documentationTable")
    table.expand(len(classList[triggerType])+1,2)
    #table.setEntry(0,0,"Class/File")
    table.setEntry(0,0,"Tags")
    table.setEntry(0,1,"Description")
    table.setTagString("preamble.latex","\\usepackage{amsmath, amssymb, marvosym}\n\
    \\usepackage[utf8]{inputenc}\n\
    \\usepackage{MnSymbol}\n\
    \\usepackage[a4paper,margin=1cm,bmargin=1.5cm,tmargin=1.5cm]{geometry}\n\
    \\usepackage[none]{hyphenat}\n\
    \\usepackage{array}\n\
    \\def\\arraystretch{1.5}\
    ")
    table.setTagString("env","longtable")
    table.setTagBool("allowlinewrap",True)
    table.setTagString("style.padding.vertical", "4px");
    table.setTagString("tableStyle.html","border-collapse: collapse ;table-layout:fixed; width:100%;")
    #table.setColAlign(0,"@{} p{0.25\\textwidth}")
    table.setColAlign(0,"@{} >{\\hspace{0pt}}p{0.30\\textwidth}")
    table.setColAlign(1,">{\\hspace{0pt}}p{0.675\\textwidth} @{}")
    latexPrefix = ""
    latexSuffix = ""

    lastClass = ""
    j = 1 #skip the first line completely, it's nothing but the head line which is set above.
    for (i,f) in enumerate(classList[triggerType]):
      #print "Class/File: {:s}".format(f)
      #print "Tag(s) : {:s}".format(tagList[i])
      #print "Description: {:s}".format(descList[i])
      if not f == lastClass:
        table.setHline(j,2)
        table.setEntry(j,1,f)
        table.setEntry(j,1,"{\\bfseries "+makeLatexCompatible(f)+"}","latex")
        table.setHline(j+1,1)
        lastClass = f
        j = j+1
      #table.setEntry(j,0,f)
      #table.setEntry(j,0,makeLatexCompatible(f),"latex")
      #table.setProperty(j,0,"prefixText.latex",latexPrefix.format(1))
      #table.setProperty(j,0,"suffixText.latex",latexSuffix)
      table.setEntry(j,0,tagList[triggerType][i].replace(",",", ").replace(",  ",", "))
      table.setEntry(j,0,makeLatexCompatible(tagList[triggerType][i]),"latex")
      table.setProperty(j,0,"prefixText.latex",latexPrefix.format(1))
      table.setProperty(j,0,"suffixText.latex",latexSuffix)
      table.setEntry(j,1,descList[triggerType][i].replace(",",", ").replace(",  ",", "))
      table.setEntry(j,1,makeLatexCompatible(descList[triggerType][i]),"latex")
      table.setProperty(j,1,"prefixText.latex",latexPrefix.format(1))
      table.setProperty(j,1,"suffixText.latex",latexSuffix)
      j = j+1
    table.writeHTML("documentation_"+triggerType+".html")
    table.writeLaTeX("documentation_"+triggerType+".tex","standalone=true,layout=something")#,layout=standalone")
  return errorFound
        
        
        
def parseLine (fname, line, triggerType):
  classList[triggerType].append(fname)
  line = line.lstrip()
  if line.startswith("["):
    #we have a list of tags the following comments apply to
    closePos = line.find("]")
    if (closePos<0):
      print "Error in file '{:s}': Treating entire line as list of tags".format(fname)
      print "List of tags is not closed properly in line {:d}:".format(lineNumber)
      print "'{:s}'".format(line)
      closePos = len(line)
    tagList[triggerType].append(line[1:closePos])
    line = line[min([closePos+1,len(line)]):len(line)]
    line = line.strip(" \t\n:")
  else:
    #only a single tag
    closePos = TQStringUtils.findFirstOf(line," \t\n")
    tagList[triggerType].append(line[:closePos].rstrip(":"))
    line = line[closePos:]
    line = line.strip(" \t\n:")
  descList[triggerType].append(line)
                    
def makeLatexCompatible(string):
  string = string.replace("\\","\\textbackslash ")
  string = string.replace("\\textbackslash  ","\\textbackslash\\ ")
  string = string.replace("_","\\textunderscore ")
  string = string.replace("\\textunderscore  ","\\textunderscore\ ")
  string = string.replace("$","\\$")
  string = string.replace("> ","\\textgreater\\ ")
  string = string.replace("< ","\\textless\\ ")
  string = string.replace(">","\\textgreater ")
  string = string.replace("<","\\textless ")
  string = string.replace("{","\\{")
  string = string.replace("}","\\}")
  string = string.replace("[","\\[")
  string = string.replace("]","\\]")
  string = string.replace("%","\\%")
  string = string.replace("&","\\&")
  string = string.replace(",",",\\hspace{0pt}") # allow line breaking even though 'no hyphenation' is set
  string = string.replace(".",".\\hspace{0pt}") 
  string = string.replace("^","\\textasciicircum ")
  string = string.replace("#","\\#")
  return string
  
  
  
  
      
      
      
      
      
      
      
      
      
      
      
      
    
  
  
  
  
  

if __name__ == "__main__":
  # parse the CLI arguments
  parser = argparse.ArgumentParser(description='Generates a documentation of tags used in the QFramework.')
  parser.add_argument('path', metavar='PATH', type=str, 
            default="$(TQPATH)/Root/*",
            nargs = "*",
            help='path to the framework files to be read')
  args = parser.parse_args()
  #get rid of command line arguments since ROOT is very greedy here (and tends to choke from it!)
  sys.argv = []
  
  from ROOT import *
  from QFramework import TQStringUtils
  TQLibrary.getQLibrary().setApplicationName(alias)
  errorFound = main(args)
  sys.exit(errorFound)

