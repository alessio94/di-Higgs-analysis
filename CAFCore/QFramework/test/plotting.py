#!/usr/bin/env python

import argparse
import datetime
import fileinput
import hashlib
import os
import re
import shutil
import subprocess
import sys
import tempfile
import unittest

import ROOT
ROOT.gROOT.SetBatch(True)
ROOT.PyConfig.IgnoreCommandLineOptions = True

import QFramework
from QFramework import TQSampleFolder
from QFramework import TQPythonTest
from QFramework import TQStringUtils
from QFramework import TQFolder

start = datetime.datetime.now()
protocol = "https"
compare = True
plotterClassDefault = "TQHWWPlotter"
plotterClass = plotterClassDefault
verboseDefault = False
verbose = verboseDefault
referenceDefault = ""
reference = referenceDefault
artifactsFolder = ""
token = ""
hashNumbering = True
referToOldPlotsIfIdentical = True
maxOutputLinesByDiff = 0
alreadyWrittenByDiff = 0
select = []
git_dir = ""

class plottingTest(TQPythonTest):

  def init(self):
    self.samples = TQSampleFolder("samples")
    self.samples.setTagInteger("style.default.histLineColor",ROOT.kBlack)

    bkg1 = self.samples.getSampleFolder("bkg/all/bkg1+")
    bkg2 = self.samples.getSampleFolder("bkg/all/bkg2+")
    bkg3 = self.samples.getSampleFolder("bkg/all/bkg3+")
    bkg4 = self.samples.getSampleFolder("bkg/all/bkg4+")
    sig1 = self.samples.getSampleFolder("sig/all/sig1+")
    sig2 = self.samples.getSampleFolder("sig/all/sig2+")

    backgrounds = [bkg1,bkg2,bkg3,bkg4]
    signals = [sig1,sig2]
    addUnderOverflow = [bkg1, sig2]

    shapes = {
      bkg1:"1",
      bkg2:"x",
      bkg3:"exp(-x)",
      bkg4:"exp(-0.5*((x-10)/10)**2)",
      sig1:"exp(-0.5*((x-10)/2)**2)",
      sig2:"exp(-0.5*((x-15)/5)**2)"
    }

    labels = {
      bkg1:"Bkg 1",
      bkg2:"Bkg 2",
      bkg3:"Bkg 3",
      bkg4:"Bkg 4",
      sig1:"Sig 1",
      sig2:"Sig 2",
    }

    scales = {
      bkg1:5,
      bkg2:2,
      bkg3:1,
      bkg4:0.5,
      sig1:1,
      sig2:0.3
    }

    colors = {
      bkg1:ROOT.kBlue,
      bkg2:ROOT.kGreen,
      bkg3:ROOT.kCyan,
      bkg4:ROOT.kOrange,
      sig1:ROOT.kRed,
      sig2:ROOT.kYellow
    }

    data = None

    for p in signals:
      p.setTagBool(".isSignal", True)
    for p in backgrounds:
      p.setTagBool(".isBackground", True)

    for p in signals+backgrounds:
      f = p.getFolder(".histograms/MyCut+")
      name = p.GetName()
      hist = ROOT.TH1F("MyVar","MyVar",20,0,20)
      hist.SetDirectory(0)
      hist.GetXaxis().SetTitle("MyVar [GeV]")
      tf = ROOT.TF1(name,shapes[p])
      hist.FillRandom(name)
      if p in addUnderOverflow:
        nBinsX = hist.GetNbinsX()
        hist.SetBinContent(0, 0.5*hist.GetBinContent(1))
        hist.SetBinContent(nBinsX + 1, 0.5*hist.GetBinContent(nBinsX))
      hist.Scale(scales[p])
      if not data:
        data = hist.Clone()
        data.SetDirectory(0)
      else:
        data.Add(hist)
      p.setTagString("style.default.title",labels[p])
      p.setTagInteger("style.default.histFillColor",colors[p])
      f.addObject(hist)

    self.bkg = [p.getPath().Data() for p in backgrounds]
    self.sig = [p.getPath().Data() for p in signals]
    self.data = ["data/all"]

    QFramework.TQHistogramUtils.rerollPoisson(data, 4357)
    datafolder = self.samples.getSampleFolder(self.data[0]+"+")
    datafolder.getFolder(".histograms/MyCut+").addObject(data)
    datafolder.setTagString("style.default.title","Data")
    datafolder.setTagInteger("style.default.histMarkerStyle",20)


  def getEmptyTag(self):
    tag = {}
    tagTestList = []
    testWithList = []
    tag["values"] = tagTestList
    tag["testWith"] = testWithList
    tag["testOnce"] = True
    return tag

  def getEmptyTagTestItem(self):
    tagTestItem = {}
    tagTestItem["value"] = ""
    tagTestItem["conditions"] = {}
    tagTestItem["conditionsNOT"] = {}
    return tagTestItem

  def createTagTestItem(self, value):
    item = self.getEmptyTagTestItem()
    item["value"] = value
    return item


  def replaceTimeStamp(self):
    """
    This function loops over all files in self.cFiles (".C" files)
    and removes the default root timestamp. This way, files
    created at different times look the same.
    """

    replacements = {}
    for cFile in self.cFiles:
      with open(cFile) as f:
        for lineNo, line in enumerate(f):
          if re.search("^\s*//", line) != None and "by ROOT version" in line:

            # The current line probably contains a timestamp.
            # Try to replace the timestamp by a generic printout
            # using with a regular expression.
            newLine = re.sub(r'\(([A-Z][a-z]{2}\s){2}[\s\d]\d\s\d\d:\d\d:\d\d\s\d{4}\)', '(### ### ## ##:##:## ####)', line)
            if newLine == line:
              self.printMessage("Time stamp in "+ cFile +" could not be replaced. It should have the format \"(Thu Jun 21 14:28:10 2018)\". Does the following line contain the timestamp?")
              self.printMessage(line, False)
            else:
              # Need to replace current line. Store the line
              # number and the replacement string.
              replacements[cFile] = lineNo, newLine
            break

    # If a file needs to be rewritten, do it here.
    for cFile in replacements:
      with open(cFile) as f:
        with open(cFile+"_tmp", 'w') as f_tmp:
          for lineNo, line in enumerate(f):
            if replacements[cFile][0] == lineNo:
              f_tmp.write(replacements[cFile][1])
            else:
              f_tmp.write(line)
      os.rename(cFile+"_tmp", cFile)

  def setUp(self):
    """
    Create temporary test directory.
    """
    # create temporary directory
    super(plottingTest, self).setUp()

    # referenceCommit is set to (decreasing precedence):
    # 1. variable "reference" taken from CLI
    # 2. environment variable "referenceCommit_$branchName"
    #    (all dashes must be replaced by underscores)
    # 3. variable referenceDefault (which is an empty string)
    # If no referenceCommit is defined, the latest master parent is
    # taken as reference.
    self.referenceCommit = referenceDefault
    if 'CI_COMMIT_REF_NAME' in os.environ:
      branchName = os.environ['CI_COMMIT_REF_NAME']
      branchName = branchName.replace("-", "_")
      refCommitEnvVar = "referenceCommit_"+branchName
      if refCommitEnvVar in os.environ:
        self.referenceCommit = os.environ[refCommitEnvVar]
        self.printMessage("  Environment variable defines a reference commit for this branch. "+refCommitEnvVar + ": " + self.referenceCommit)
    if reference != "":
      self.referenceCommit = reference
      self.printMessage("  Command-line options overwrite previous setting of reference commit: " + reference)
    out, err = self.executeGitCommand("rev-parse " + self.referenceCommit)
    self.referenceCommit = out.strip()
    self.referenceJobName = '${CI_JOB_NAME}'

    self.cFiles = []
    self.pdfFiles = []
    self.gitlabAddressUser = 'https://gitlab.cern.ch/atlas-caf/CAFCore/'
    self.gitlabAddress = protocol+'://gitlab.cern.ch/api/v4/projects/atlas-caf%2FCAFCore/'
    if token == "":
      self.headerToken = '--header "Private-Token: ${ATLASCAF_TOKEN}"'
    else:
      self.headerToken = '--header "private-Token: ' + token + '"'
    self.dotCFilesFolder = "dotCFiles"
    self.dotPDFFilesFolder = "pdfFiles"
    if os.path.exists(self.dotCFilesFolder):
      shutil.rmtree(self.dotCFilesFolder)
    os.makedirs(self.dotCFilesFolder)
    if os.path.exists(self.dotPDFFilesFolder):
      shutil.rmtree(self.dotPDFFilesFolder)
    os.makedirs(self.dotPDFFilesFolder)
    open("fullDiff.txt", 'w').close()
    if os.path.isfile("messageForGitlab.txt"):
      os.remove("messageForGitlab.txt")

    self.artifactsFolderBase = "artifacts"
    self.artifactsFolderBase2 = "plotting-artifacts"
    self.referenceHashIndicator = "Full reference commit hash : "

  def tearDown(self):
    if hasattr(self, "artifactsZipPath") and os.path.exists(self.artifactsZipPath):
      os.remove(self.artifactsZipPath)
    # delete temporary directory
    super(plottingTest, self).tearDown()


  def addDefaultTags(self, testCombination, tags):
    # go through conditions and add them until there are no more to add
    addedCondition = True
    while addedCondition:
      addedCondition = False
      for tagName in list(testCombination):
        if tagName not in tags:
          self.printMessage("You applied a condition for the tag \""+ tagName+"\", but there is no such tag defined.")
          self.printMessage("Exiting.", False)
          import sys
          sys.exit(-1)
        for tagItem in tags[tagName]["values"]:
          if tagItem["value"] == testCombination[tagName]:
            for conditionTagName in tagItem["conditions"]:
              if not conditionTagName in testCombination:
                testCombination[conditionTagName] = tagItem["conditions"][conditionTagName]
                addedCondition = True
              else:
                if testCombination[conditionTagName] != tagItem["conditions"][conditionTagName]:
                  # This is just a warning for debugging. This tag combination is not supposed to be created.
                  # print("WARNING: The following combination could not be generated, because the conditions are inconsistent.")
                  # print("         "+ str(testCombination)+ " and "+ conditionTagName+ " = "+ str(tagItem["conditions"][conditionTagName])+ " required by " + tagName)
                  return False

    # add default tags and try to avoid conditionsNOT tags. not the best way possible.
    for candidateName in tags:
      candidateTag = tags[candidateName]
      if candidateName not in testCombination:
        # this is the tag we're trying to add (tag value tag["values"][i]["value"]; see which value is allowed
        canAddThisTag = False
        i = 0
        while i < len(candidateTag["values"]) and not canAddThisTag:
          candidateTagItem = candidateTag["values"][i]
          candidateValue = candidateTagItem["value"]
          canAddThisTag = True
          # check if this value is in conflict with any of the previous values
          # compare values of testCombination to conditionsNOT values, while conditionsNOTName must match combinationTagName
          for conditionNOTName in candidateTagItem["conditionsNOT"]:
            conditionNOTList = candidateTagItem["conditionsNOT"][conditionNOTName]
            for conditionNOT in conditionNOTList:
              if conditionNOTName in testCombination:
                if testCombination[conditionNOTName] == candidateValue:
                  canAddThisTag = False



          # go through all entries of testCombination
          # loop over tags[testCombinationName]["values"] and find the tagItem with the value of testCombinationValue
          # check if tagItem[conditionsNOT] has an entry for candidateName
          # loop over entries of tagItem[conditionsNOT][candidateName]
          # if entry agrees with candidateValue, cancel
          for testCombinationName in testCombination:
            testCombinationValue = testCombination[testCombinationName]
            tagItem = ""
            for tagItem2 in tags[testCombinationName]["values"]:
              if tagItem2["value"] == testCombinationValue:
                tagItem = tagItem2
            if tagItem == "":
              # The following is a valid warning, but it can make sense to define a condition that does not match a possible value.
              # For now, allow to define condition that has not been defined in value list:
              tagItem = tagItem2
              # print("Logical error in code. A value has been assigned to be tested which is not in the list of possible values.")
            if candidateName in tagItem["conditionsNOT"]:
              for tagItemValue in tagItem["conditionsNOT"][candidateName]:
                if tagItemValue == candidateValue:
                  canAddThisTag = False

          if canAddThisTag:
            testCombination[candidateName] = candidateValue
          i = i+1
        if not canAddThisTag:
          return False
    return True


  def addTagsToTestWith(self, testCombinations, testCombination, tagsToTestWith, tagsTestedWith, tags):

    # print("     ","addTagsToTestWith(",testCombinations, ",", testCombination,",", tagsToTestWith,", tags)")
    # print("     ","addTagsToTestWith( testCombinations, ", testCombination,",", tagsToTestWith,", tags)")

    if len(tagsToTestWith) == 0:
      success = self.addDefaultTags(testCombination, tags)
      # print("        appending", testCombination, "to", testCombinations)
      if testCombination not in testCombinations and success:
        testCombinations.append(testCombination)
      # print("        gives    ", testCombinations)
      return

    tagName = tagsToTestWith[0]
    tag = tags[tagName]
    if not tagName in tagsTestedWith:
      tagsTestedWith.append(tagName)
    del tagsToTestWith[0]

    if len(tag["testWith"]) > 0:
      for tagToTestWith in tag["testWith"]:
        if tagToTestWith in tagsToTestWith or tagToTestWith in tagsTestedWith:
          continue
        tagsToTestWith.append(tagToTestWith)
    for tagLoopName in tags:
      tagLoop = tags[tagLoopName]
      if tagName in tagLoop["testWith"]:
        if tagLoopName in tagsToTestWith or tagLoopName in tagsTestedWith:
          continue
        tagsToTestWith.append(tagLoopName)

    for tagTest in tag["values"]:
      testCombination[tagName] = tagTest["value"]
      self.addTagsToTestWith(testCombinations, dict(testCombination), tagsToTestWith[:], tagsTestedWith, tags)

    return

  def removeUnwantedCombinations(self, combinations, tags):
    for tagName in tags:
      tag = tags[tagName]
      for tagItem in tag["values"]:
        # check if there are any conditions for given tag value
        if len(tagItem["conditions"]) > 0 or len(tagItem["conditionsNOT"]) > 0:

          entriesToRemove_index = []
          for i, combination in enumerate(combinations):

            # print("checking combination", i, combination)
            if combination == {}:
              continue

            #loop over entries in combination and remove all that fit the conditions
            removeEntry = False
            if combination[tagName] == tagItem["value"]:
              for conditionTagName in tagItem["conditions"]:

                # print("  if", conditionTagName, ":", tagItem["conditions"][conditionTagName], "!=", combination[conditionTagName], "--> remove")
                if tagItem["conditions"][conditionTagName] != combination[conditionTagName]:
                  removeEntry = True
                  break
              if not removeEntry:
                for conditionNOTTagName in tagItem["conditionsNOT"]:
#                  print("  if", conditionNOTTagName, ":", tagItem["conditionsNOT"][conditionNOTTagName], "==", combination[conditionNOTTagName], "--> remove")
                  for conditionNOTValue in tagItem["conditionsNOT"][conditionNOTTagName]:
                    if conditionNOTValue == combination[conditionNOTTagName]:
                      removeEntry = True
                      break
            if removeEntry:
              entriesToRemove_index.append(i)
          for entryToRemove_index in reversed(entriesToRemove_index):
            del combinations[entryToRemove_index]

    return

  def createTestTags(self, tags):
    # remove all double occurences of testing
    for tagName in tags:
      tag = tags[tagName]
      for testWith in tag["testWith"]:
        if testWith not in tags:
          self.printMessage("You want to test the tag \""+ tagName+"\" together with the tag \""+ testWith + "\", but the latter one does not exist.", False)
          tagIsMissing = True
          self.assertFalse(tagIsMissing)
        if tag in tags[testWith]["testWith"]:
          tag["testWith"].remove(testWith)

    combinations = []
    tagsTestedWith = []

    # add combination without any tags to test default values
    combinations.append({})

    combination_default = {}
    # add most basic combination: all defaults
    if not self.addDefaultTags(combination_default, tags):
      self.printMessage("Could not add default tag combination (only zeroth entries of tagItem). Exiting.", False)
      return combinations, False
    combinations.append(combination_default)

    for tagName in tags:
      # print("    Adding tag",tagName)
      if tagName in tagsTestedWith:
        continue
      tag = tags[tagName]
      if len(tag["testWith"]) == 0 and tag["testOnce"]:
        # Add all permutations of tag, while keeping all other tags fixed
        for tagItem in tag["values"]:
          combination = {}
          combination[tagName] = tagItem["value"]
          success = self.addDefaultTags(combination, tags)
          if combination not in combinations and success:
            combinations.append(combination)
      else:
        # Test permutations of this tag while also varying tag["testWith"]
        tagsToTestWith = tag["testWith"]
        tagsToTestWith.append(tagName)
        self.addTagsToTestWith(combinations, {}, tagsToTestWith, tagsTestedWith, tags)

      # print("    combinations:")
      # for i, combination in enumerate(combinations):
      #   if i < 10:
      #     print("        "+str(i)+" "+ str(combination))
      #   else:
      #     print("       "+str(i)+" "+ str(combination))

      # print("=======================")

    self.removeUnwantedCombinations(combinations, tags)

    # print("summary:")
    # for i, combination in enumerate(combinations):
    #   if i < 10:
    #     print("        "+str(i)+" "+ str(combination))
    #   else:
    #     print("       "+str(i)+" "+ str(combination))

    return combinations, True


  def combinationToString(self, combination):
    combinationString = ""
    first = True
    for combinationTagName in sorted(combination.keys()):
      if not first:
        combinationString += ","
      first = False
      combinationTagValue = combination[combinationTagName]
      combinationString += combinationTagName+"="+combinationTagValue
    return combinationString

  def combinationsToList(self, combinations):
    combinationList = []
    for combination in combinations:
      combinationString = self.combinationToString(combination)
      combinationList.append(combinationString)
    return combinationList

  def executeGitCommand(self, command, printErrorMessage = True):
    if git_dir == "":
      newCommand = "git " + command
    else:
      newCommand = "git --git-dir=" + git_dir + " " + command
    return self.executeCommand(newCommand, printErrorMessage)

  def executeCommand(self, command, printErrorMessage = True):
    p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, executable='/bin/bash')
    out, err = p.communicate()
    out = out.decode("ascii")
    err = err.decode("ascii")
    if err != "" and printErrorMessage:
      print("ERROR MESSAGE:"+ err)
    return out, err

  def plotCombinations(self, combinations, summaryFile):
    from ROOT import TString

    try:
      Plotter = getattr(QFramework,plotterClass)
    except AttributeError:
      raise RuntimeError("no plotter '"+plotterClass+"' available in QFramework!")

    with open(summaryFile,"w") as f:
      for i, combination in enumerate(combinations):
        identifier = self.combinationToString(combination)
        hashValue = hashlib.md5(identifier.encode()).hexdigest()

        if len(select) > 0 and not hashValue in select:
          continue

        filename = "myplotHash_"+hashValue
        if not hashNumbering:
          filename = "myplotHash_"+str(i).zfill(3)
        f.write(filename+",\t"+identifier+"\n")
        filenameDotC = self.dotCFilesFolder+"/"+filename+".C"
        filenameDotPDF = self.dotPDFFilesFolder+"/"+filename+".pdf"

        plotter = Plotter(self.samples)
        plotter.setTagString("labels.0",hashValue)
        plotter.setTagBool("verbose",verbose)
        for combinationTagName in combination:
          combinationTagValue = combination[combinationTagName]
          if combinationTagValue != "***NO VALUE***":
            plotter.setTagString(combinationTagName, combinationTagValue)

        for b in self.bkg:
          bName = "Graph_" + TQFolder.getPathTail(TString(b)).Data()
          plotter.addBackground(b, ".name=bkg_" + bName)
        for s in self.sig:
          sName = "Graph_" + TQFolder.getPathTail(TString(s)).Data()
          plotter.addSignal(s,  ".name=sig_" + sName)
        for d in self.data:
          dName = "Graph_" + TQFolder.getPathTail(TString(d)).Data()
          plotter.addData(d, ".name=data_" + dName)
        plotter.plotAndSaveAs("MyCut/MyVar",filenameDotC, "sortObjects=true")
        plotter.plotAndSaveAs("MyCut/MyVar",filenameDotPDF)
        self.cFiles.append(filenameDotC)
        self.pdfFiles.append(filenameDotPDF)

  def getMasterCommitHash(self):
    """
    This function returns the hash of the master commit, from which
    the current commit branched off.
    """

    command = 'merge-base origin/master HEAD'
    masterHash, err = self.executeGitCommand(command)
    return masterHash.strip()

  def getParentCommitHash(self, commitHash):
    # The option --first-parent ignores all commits merged into the
    # current branch (i.e. it lists only commits from the current
    # branch.
    command = 'rev-list --first-parent "' + commitHash + '" |head -n 2 | tail -n 1'
    parentHash, err = self.executeGitCommand(command)
    return parentHash.strip()

  def parseJobName(self, jobName):
    if jobName.startswith('$'):
      command = "echo " + jobName
      out, err = self.executeCommand(command, False)
      if out == "\n" or err != "":
        if err != "":
          print("err: "+ err)
        print("    I assume that you are looking for a job with the name \"test\".")
        return "test"
      return out.strip()
    return jobName

  def searchForArtifacts(self, maxNoIterations, onMaster):
    if self.referenceCommit != "":
      self.printMessage("  Searching for jobs and artifacts of commit "+ self.getShortGitHash(self.referenceCommit) + ".")
      referenceHash = self.referenceCommit
      maxNoIterations = 1
    else:
      referenceHash = self.getMasterCommitHash()
      if onMaster:
        referenceHash = self.getParentCommitHash(referenceHash)
      self.printMessage("  Searching for jobs and artifacts of last master parent ("+self.getShortGitHash(referenceHash)+").")

    artifactsFound = False
    i = 0
    jobNumber_return = None
    artifactsPlotFolder = None
    artifactsPlotFolderPDF = None
    while not artifactsFound and referenceHash != "" and i < maxNoIterations:
      referToHash = ""
      jobNumbers = self.getJobNumbersOfCommit(referenceHash, self.referenceJobName)
      for jobNumber in jobNumbers:
        jobNumber_return  = jobNumber
        if not artifactsFound:
          artifactsFound, artifactsFolder, referToHash = self.downloadArtifacts(jobNumber, self.artifactsFolderBase)
          if artifactsFound or referToHash != "":
            self.printMessage("  Found job " + jobNumber + " for commit " + self.getShortGitHash(referenceHash) +".")
          if referToHash != "":
            self.printMessage("    The commit has identical plots to " + self.getShortGitHash(referToHash) + ". Looking up this commit instead.")
            referenceHash = referToHash
            break
          artifactsPlotFolder = artifactsFolder + "/referencePlots"
          artifactsPlotFolderPDF = artifactsFolder + "/referencePlotsPDF"
          if artifactsFound:
            self.printMessage('  Artifacts are downloaded to directory "' + artifactsFolder+'".')
      if referToHash != "":
        continue
      if not artifactsFound:
        i = i + 1
        self.printMessage("    Could not find artifacts in commit " + self.getShortGitHash(referenceHash) + ". Trying " + str(maxNoIterations-i) + " older commits.")
        referenceHash = self.getParentCommitHash(referenceHash)
    referenceInfo = {}
    referenceInfo["hash"] = referenceHash
    referenceInfo["jobNumber"] = jobNumber_return
    referenceInfo["jobName"] = self.referenceJobName
    return artifactsFound, artifactsPlotFolder, artifactsPlotFolderPDF, referenceInfo

  def analyzeCurlOutAndErr(self, out, err, errString):
    canAccessGitlab = True
    if err != "" and not err.startswith("  % Total    % Received"):
      print("    Unexpected error message while " + errString+":")
      print("      "+ err)
      canAccessGitlab = False
    if out == '{"message":"401 Unauthorized"}':
      self.printMessage("    Cannot access gitlab website. What's your method of authentication?")
      canAccessGitlab = False
    elif out.startswith('{"error":'):
      print("    Cannot access gitlab website. See message:")
      print("      "+ out)
      canAccessGitlab = False
    elif out.startswith('{"message":'):
      print("    There might be an unknown error message: " + out)
    self.assertTrue(canAccessGitlab)

  def getJobNumbersOfCommit(self, commitHash, jobName=""):
    """
    Returns the job numbers of a commit.
    """
    address = '"'+self.gitlabAddress + 'repository/commits/'
    name = '"'
    parsedJobName = self.parseJobName(jobName)
    if parsedJobName != "":
      name = '?name='+parsedJobName+'"'
    command = 'curl -L ' + self.headerToken + ' ' + address + commitHash + '/statuses' + name
    out, err = self.executeCommand(command, False)
    self.analyzeCurlOutAndErr(out, err, "downloading job numbers")
    out = out.replace("true", "True")
    out = out.replace("false", "False")
    out = out.replace("null", "None")
    out = out.strip()
    outList = eval(out)
    listOfIDs = []
    for entry in outList:
      listOfIDs.append(str(entry["id"]))
    return listOfIDs

  def downloadArtifacts(self, jobNumber, folderBase):
    folder = folderBase+"_"+jobNumber
    artifactsZipPath = folder + '/artifacts.zip'
    self.artifactsZipPath = artifactsZipPath
    if not os.path.exists(folder):
      os.makedirs(folder)
    elif os.path.exists(artifactsZipPath):
      os.remove(artifactsZipPath)
    address = '"' + self.gitlabAddress + 'jobs/' + jobNumber + '/artifacts"'

    command = 'curl -L -o '+ artifactsZipPath +' ' + self.headerToken + ' ' + address
    out, err = self.executeCommand(command, False)
    self.analyzeCurlOutAndErr(out, err, "downloading artifacts")
    command = 'unzip -o ' + artifactsZipPath + ' -d ' + folder
    out, err = self.executeCommand(command, False)
    if err != "":
      self.printMessage("      Could not unpack zip folder downloaded for job " + str(jobNumber)+".")
      if err.startswith("  End-of-central-directory signature not found.  Either this file"):
        self.printMessage("      Is it possible that this job ("+ jobNumber + ") does not have artifacts?")
      # print('The full error message by "unzip" follows:')
      # print(err)
      return False, "", ""

    fullFolder = folder + "/" + self.artifactsFolderBase2
    # check if there are plots or if the README.txt file refers to another commit
    readmePath = fullFolder + "/README.txt"
    if os.path.exists(readmePath):
      # only remove zip path if README exists, because this case is not caught by tearDown
      if os.path.exists(self.artifactsZipPath):
        os.remove(self.artifactsZipPath)
      with open(readmePath, "r") as f:
        for line in f.readlines():
          if line.startswith(self.referenceHashIndicator):
            referToHash = line[len(self.referenceHashIndicator):].strip()
            # not really necessary
            # shutil.rmtree(folder)
            return False, "", referToHash

    return True, fullFolder, ""

  def writeDiffOutput(self, filename, out, maxNoLines):
    global alreadyWrittenByDiff
    if alreadyWrittenByDiff + maxNoLines < maxOutputLinesByDiff:
      print('    Difference found in file ' + filename + '. See the output of "diff" (new plots left, reference right):')
      numberOfLines = out.count('\n')
      if numberOfLines > maxNoLines:
        print('    (printing only first '+str(maxNoLines)+' lines)')
        for line in out.split('\n')[:maxNoLines-1]:
          print(line)
        alreadyWrittenByDiff += maxNoLines
      else:
        print(out)
        alreadyWrittenByDiff += numberOfLines
    return

  def writeDiffLog(self, out, command):
    with open("fullDiff.txt", "a+") as f:
      f.write(command+'\n')
      f.write(out)
      f.write('\n')
      f.write('==================================================\n')
      f.write('\n')
    return

  def compareDotPDFFiles(self, dotPDFFolder, artifactsPlotFolder,otherHash):
    dotPDFFiles = self.pdfFiles
    if not os.path.exists(artifactsPlotFolder):
      self.printMessage("  Artifacts folder with plots does not exist (" + artifactsPlotFolder +").")
      return False, []
    dotPDFFilesArtifacts = [f for f in os.listdir(artifactsPlotFolder) if f.endswith(".pdf")]

    filesInBoth = [f.split("/")[-1] for f in dotPDFFiles if f.split("/")[-1] in dotPDFFilesArtifacts]

    if len(filesInBoth) > 0:
      with open("summary.tex","wt") as outfile:
        outfile.write("\\documentclass{beamer}\n")
        outfile.write("\\title{ " + self.getCurrentCommit() + " vs. " + otherHash + " }\n")
        outfile.write("\\begin{document}\n")
        outfile.write("\\maketitle\n")
        import re
        rex = re.compile("myplotHash_([0-9abcdef]*).pdf")
        for file in filesInBoth:
          match = rex.match(file)
          file1 = dotPDFFolder + "/" + file
          file2 = artifactsPlotFolder + "/" + file
          outfile.write("\\section{"+match.group(1)+"}\n")
          outfile.write("\\begin{frame}{"+self.getCurrentCommit()+"}\n")
          outfile.write("\\includegraphics[width=\\textwidth,height=\\textheight,keepaspectratio]{"+file1+"}\n")
          outfile.write("\\end{frame}\n")
          outfile.write("\\begin{frame}{"+otherHash+"}\n")
          outfile.write("\\includegraphics[width=\\textwidth,height=\\textheight,keepaspectratio]{"+file2+"}\n")
          outfile.write("\\end{frame}\n")
        outfile.write("\\end{document}\n")

  def compareDotCFiles(self, dotCFolder, artifactsPlotFolder):
    dotCFiles = self.cFiles
    if not os.path.exists(artifactsPlotFolder):
      self.printMessage("  Artifacts folder with plots does not exist (" + artifactsPlotFolder +").")
      return False, []
    dotCFilesArtifacts = [f for f in os.listdir(artifactsPlotFolder) if f.endswith(".C")]

    filesInBoth = [f.split("/")[-1] for f in dotCFiles if f.split("/")[-1] in dotCFilesArtifacts]
    additionalFilesArtifacts = [f for f in dotCFilesArtifacts if f not in filesInBoth]
    additionalFiles = [f.split("/")[-1] for f in dotCFiles if f.split("/")[-1] not in filesInBoth]
    filesWithDifferences = []

    allFilesEqual = True

    if len(filesInBoth) > 0:
      for file in filesInBoth:
        file1 = dotCFolder + "/" + file
        file2 = artifactsPlotFolder + "/" + file
        command = "diff " + file1 +" "+ file2
        out, err = self.executeCommand(command)
        if out != "":
          allFilesEqual = False
          filesWithDifferences.append(file)
          self.writeDiffOutput(file, out, 50)
          self.writeDiffLog(out, command)
      if allFilesEqual:
        self.printMessage('    No differences found.')
    else:
      self.printMessage('    No files to compare. See summary below.')

    self.printMessage('  Summary after comparison of ' + str(len(filesInBoth)) + ' plot files:')
    if not allFilesEqual:
      self.printMessage('  There were differences found in ' + str(len(filesWithDifferences)) + ' files:', False)
      self.printMessage('    '+ str(filesWithDifferences), False)
    if len(additionalFiles) != 0:
      self.printMessage('  The current commit has ' + str(len(additionalFiles)) + ' files that were not found in artifacts:', False)
      self.printMessage('    '+ str(additionalFiles), False)
    if len(additionalFilesArtifacts) != 0:
      self.printMessage('  The artifacts have ' + str(len(additionalFilesArtifacts)) + ' files that were not found in current commit:', False)
      self.printMessage('    '+ str(additionalFilesArtifacts))
    if allFilesEqual and len(additionalFiles) == 0 and len(additionalFilesArtifacts) == 0:
      self.printMessage('  All files are equal. Everything looks great!')
    return allFilesEqual, filesWithDifferences

  def fuzzyMatchDotCFiles(self, dotCFolder, artifactsPlotFolder, filesWithDifferences):
    '''
    For each entry in filesWithDifference, a number of modifications
    is performed so that it matches respective artifact better. These
    modifications are:
    1) Remove the line that contains the root version
    2) Remove name identifier of histograms (not used currently)
    3) Bring all lines in alphabetical order (not used currently)

    The latter two modifications are not in use currently. The
    motivation for introducing 2) was that root appends "random"
    numbers to the histogram names. This is suppressed by using the
    substring "Graph_" in the histogram name. The motivation for 3)
    was that TCanvas::getListOfPrimitives() returns objects in an
    arbitrary order, which changes the histogram definitions in the
    .C file. The current workaround is to order the list of primitives
    in the TQPlotter.
    '''

    from ROOT import TString
    removeRootVersion = True
    removeHistoIdentifier = False
    sortAlphabetically = False

    self.printMessage('  Trying to fuzzy-match these files. This is meant to catch different root versions.')
    global maxOutputLinesByDiff
    maxOutputLinesByDiff += maxOutputLinesByDiff / 5.
    fuzzyFolder = dotCFolder + "_fuzzy"
    fuzzyFolderArtifacts = artifactsPlotFolder + "_fuzzy"
    if os.path.exists(fuzzyFolder):
      shutil.rmtree(fuzzyFolder)
    os.makedirs(fuzzyFolder)
    if os.path.exists(fuzzyFolderArtifacts):
      shutil.rmtree(fuzzyFolderArtifacts)
    os.makedirs(fuzzyFolderArtifacts)

    allFilesEqual = True
    newFilesWithDifferences = []
    for file in filesWithDifferences:
      for paths in [[artifactsPlotFolder, fuzzyFolderArtifacts], [dotCFolder, fuzzyFolder]]:
        with open(paths[0] + "/" + file, "r") as f:
          with open(paths[1] + "/" + file, "w") as ff:
            newFile = []
            for line in f:
              newLine = TString(line)

              # This condition removes the line with the root version
              if removeRootVersion and newLine.Contains("(### ### ## ##:##:## ####)"):
                continue

              # This section removes all histogram variable names
              if removeHistoIdentifier:
                dummy = TString("")
                if newLine.Contains("->"):
                  TQStringUtils.readUpToText(newLine, dummy, "->")
                elif (newLine.Contains(' = new TH1F')):
                  TQStringUtils.readUpToText(newLine, dummy, ' = new TH1F')
                  pos = TQStringUtils.findFirstOf(newLine, '"', 13)
                  newLine = TString(newLine.Data()[pos:])
              newFile.append(newLine.Data())

            # This section brings all lines in alphabetical order
            if sortAlphabetically:
              newFile.sort()

            for line in newFile:
              ff.write(line)
      file1 = fuzzyFolder + "/" + file
      file2 = fuzzyFolderArtifacts + "/" + file
      command = "diff " + file1 +" "+ file2
      out, err = self.executeCommand(command)
      if out != "":
        allFilesEqual = False
        self.writeDiffOutput(file, out, 20)
        self.writeDiffLog(out, command)
        newFilesWithDifferences.append(file)

    if allFilesEqual:
      self.printMessage('  Fuzzy-matching was successful.')
    else:
      self.printMessage('  Fuzzy-matching was not successful for ' + str(len(newFilesWithDifferences)) + ' files:')
      self.printMessage('    '+ str(newFilesWithDifferences), False)

    return allFilesEqual, newFilesWithDifferences

  def writeReadmeFile(self, referenceInfo):
    '''
    Plots are only uploaded to gitlab if they are different from the
    reference plots. This function writes a readme file referring to
    the commit, which contains the reference plots.
    '''

    referenceHashShort = self.getShortGitHash(referenceInfo["hash"])
    jobLink = self.gitlabAddressUser + '-/jobs/' + referenceInfo["jobNumber"]
    downloadLink = jobLink + '/artifacts/download'

    with open("messageForGitlab.txt", 'w') as f:
      f.write("The plots produced in this commit are equal to the reference commit " + referenceHashShort + ". To avoid duplicating data, please refer to this commit.\n\n")
      f.write(self.referenceHashIndicator + referenceInfo["hash"] + "\n")
      f.write("Link to the job            : " + jobLink + "\n")
      f.write("Artifacts download link    : " + downloadLink + "\n")
    return

  def getLongGitHash(self, shortHash):
    out, err = self.executeGitCommand("rev-parse --verify " + shortHash)
    out = out.strip("\n")
    if not err.strip("\n ") == "":
      print(err)
    return out

  def getShortGitHash(self, longHash):
    out, err = self.executeGitCommand("rev-parse --short " + longHash)
    out = out.strip("\n")
    return out

  def getCurrentCommit(self):
    out, err = self.executeGitCommand("rev-parse HEAD")
    return out.strip()

  def isCommitOnOriginMasterHistory(self, commitHash):
    command = 'rev-list --first-parent origin/master | grep "' + commitHash + '"'
    out, err = self.executeGitCommand(command)
    if not err.strip("\n ") == "":
      print("Error message from git:")
      print(err)
    out = out.strip("\n ")
    numberLines = out.count("\n")
    if not numberLines == 0:
      print("Unexpected output from " + commitHash)
      print(out)
      return False
    if out == self.getLongGitHash(commitHash):
      return True
    else:
      return False

  def isCurrentBranchOriginMaster(self):
    out, err = self.executeGitCommand("rev-parse HEAD")
    out1, err1 = self.executeGitCommand("rev-parse origin/master")
    if out.strip() == out1.strip():
      return True
    return False

  def getCurrentBranchName(self):
    # works fine locally, but not in CI, when commits are checked out (instead of branches)
    out, err = self.executeGitCommand("rev-parse --abbrev-ref HEAD")
    return out.strip()

  def printMessage(self, string, printTime = True, suppressMessage = False):
    if suppressMessage:
      return
    timeDelta = (str(datetime.datetime.now() - start)).split(".")[0]
    if printTime:
      print(timeDelta + string)
    else:
      print(" " * len(timeDelta) + string)

  def defineTestTags(self):
    """
    Syntax of tags:
      tags["tagName"]          : Called tag. Contains all information for the tag "tagName".
      tag["testWith"]          : Specifies list of tagName. The current tag will be tested with all combinations of the specified tags.
      tag["testOnce"]          : If True, all values of this tag will be tested with the default values of all other tags.
      tag["values"]            : List of tagItem. A tagItem contains all information related to a value of a tag.
      tagItem["value"]         : One possible value of the tag "tagName"
      tagItem["conditions"]    : Contains a dictionary of the form {tagName: tagValue}. This value of tag requires tagValue of tagName at the same time.
      tagItem["conditionsNOT"] : Contains dictionary of the form {tagName: [tagValue1, tagValue2]}. This value of tag excludes tagValue1 and tagValue2 of tagName at the same time.
    All conditions are combined using AND.
    """
    tags = {}

    # subplots
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('ratio'))
    tag["values"][1]["conditions"]['style.showRatio'] = "true"
    tag["values"].append(self.createTagTestItem('dmb'))
    tag["values"][2]["conditions"]['style.showDminusB'] = "true"
    tag["values"].append(self.createTagTestItem('cutopt'))
    tag["values"][3]["conditions"]['style.showOptScan'] = "true"
    tags['style.subPlot'] = tag
    # style.showRatio
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['style.showRatio'] = tag
    # style.showDminusB
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['style.showDminusB'] = tag
    # style.showOptScan
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['style.showOptScan'] = tag

    # style.DminusB.errors
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('totalStack'))
    tag["values"].append(self.createTagTestItem('totalBackground'))
    tag["values"][1]["conditions"]['style.subPlot'] = 'dmb'
    tag["values"][2]["conditions"]['style.subPlot'] = 'dmb'
    tags['style.DminusB.errors'] = tag
    tags['style.dmb.errors'] = tag

    # style.stackSignal
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.stackSignal'] = tag

    # style.nLegendCols
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('2'))
    tag["values"].append(self.createTagTestItem('1'))
    tags['style.nLegendCols'] = tag

    # labels.atlas
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('Internal'))
    tags['labels.atlas'] = tag

    # labels.1
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('#sqrt{s}= 13 TeV, 36.1 fb^{-1}'))
    tags['labels.1'] = tag

    # style.main.totalStackError.fillStyle
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('3245'))
    tag["values"].append(self.createTagTestItem('3244'))
    tags['style.main.totalStackError.fillStyle'] = tag

    # geometry.main.yAxis.titleOffset
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('1.'))
    tag["values"].append(self.createTagTestItem('1.2'))
    tags['geometry.main.yAxis.titleOffset'] = tag

    # style.min
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0'))
    tag["values"].append(self.createTagTestItem('-1000'))
    tag["values"].append(self.createTagTestItem('1000'))
    tags['style.min'] = tag

    # style.autoStack
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.autoStack'] = tag

    # style.autoStackSignal
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.autoStackSignal'] = tag

    # labels.legendTotalBkg
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('Uncertainty'))
    tag["values"].append(self.createTagTestItem('Test'))
    tags['labels.legendTotalBkg'] = tag

    # legend.showTotalBkgErrorType
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['legend.showTotalBkgErrorType'] = tag

    # labels.drawInfo
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['labels.drawInfo'] = tag

    # style.main.totalStack.lineWidth
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0'))
    tags['style.main.totalStack.lineWidth'] = tag

    # style.main.totalStack.lineColor
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('kWhite'))
    tag["values"].append(self.createTagTestItem('kRed'))
    tags['style.main.totalStack.lineColor'] = tag

    # style.sub.margin
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('1.2'))
    tag["values"].append(self.createTagTestItem('1.5'))
    tag["testWith"] = ['geometry.sub.height', 'geometry.canvas.height', 'geometry.canvas.width']
    tags['style.sub.margin'] = tag

    # geometry.sub.height
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0.4'))
    tag["values"].append(self.createTagTestItem('0.6'))
    tags['geometry.sub.height'] = tag

    # legend.errorDisplayType
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('f'))
    tag["values"].append(self.createTagTestItem('p'))
    tag["values"].append(self.createTagTestItem('l'))
    tag["values"].append(self.createTagTestItem('lf'))
    tags['legend.errorDisplayType'] = tag

    # test this one tag for all axes. Test all the other ones only for main.xAxis.
    # style.main.xAxis.showTitle
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.main.xAxis.showTitle'] = tag

    # style.main.yAxis.showTitle
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.main.yAxis.showTitle'] = tag

    # style.sub.xAxis.showTitle
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"][1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tag["values"][2]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['style.sub.xAxis.showTitle'] = tag

    # style.sub.yAxis.showTitle
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"][1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tag["values"][2]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['style.sub.yAxis.showTitle'] = tag

    # style.main.xAxis.showLabels
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"][1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['style.main.xAxis.showLabels'] = tag

    # style.main.xAxis.showTicks
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.main.xAxis.showTicks'] = tag

    # style.main.xAxis.nDiv
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('000510'))
    tags['style.main.xAxis.nDiv'] = tag

    # geometry.main.xAxis.textSize
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0.05'))
    tag["values"].append(self.createTagTestItem('0.1'))
    tags['geometry.main.xAxis.textSize'] = tag

    # geometry.main.xAxis.titleSize
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0.05'))
    tag["values"].append(self.createTagTestItem('0.1'))
    tags['geometry.main.xAxis.titleSize'] = tag

    # geometry.main.xAxis.labelSize
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0.05'))
    tag["values"].append(self.createTagTestItem('0.1'))
    tags['geometry.main.xAxis.labelSize'] = tag

    # geometry.main.xAxis.titleOffset
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('1.0'))
    tag["values"].append(self.createTagTestItem('1.3'))
    tags['geometry.main.xAxis.titleOffset'] = tag

    # geometry.main.xAxis.labelOffset
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0.00025'))
    tag["values"].append(self.createTagTestItem('0.00015'))
    tags['geometry.main.xAxis.labelOffset'] = tag

    # geometry.main.xAxis.tickLength
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0.03'))
    tag["values"].append(self.createTagTestItem('0.02'))
    tags['geometry.main.xAxis.tickLength'] = tag

    # style.main.data.lineColor
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('kRed'))
    tags['style.main.data.lineColor'] = tag

    # style.main.data.lineStyle
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('2'))
    tags['style.main.data.lineStyle'] = tag

    # style.main.data.markerColor
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('kBlue'))
    tags['style.main.data.markerColor'] = tag

    # style.main.data.markerSize
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('2'))
    tags['style.main.data.markerSize'] = tag

    # style.main.data.markerStyle
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('3'))
    tags['style.main.data.markerStyle'] = tag

    # geometry.main.scaling
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('1.0'))
    tag["values"].append(self.createTagTestItem('0.8'))
    tags['geometry.main.scaling'] = tag

    # geometry.legend.xMin
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0.55'))
    tag["testWith"] = ["geometry.legend.xMax"]
    tags['geometry.legend.xMin'] = tag

    # geometry.legend.xMax
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0.8'))
    tags['geometry.legend.xMax'] = tag

    # geometry.legend.yMin
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0.65'))
    tags['geometry.legend.yMin'] = tag

    # geometry.legend.yMax
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0.9'))
    tags['geometry.legend.yMax'] = tag

    # geometry.sub.margins.left
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0.2'))
    tag["values"][-1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['geometry.sub.margins.left'] = tag

    # geometry.sub.margins.right
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0.1'))
    tag["values"][-1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['geometry.sub.margins.right'] = tag

    # geometry.sub.margins.top
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0'))
    tag["values"][-1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['geometry.sub.margins.top'] = tag

    # geometry.sub.margins.bottom
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0.2'))
    tag["values"][-1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['geometry.sub.margins.bottom'] = tag

    # style.main.tickx
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.main.tickx'] = tag

    # style.main.ticky
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"][-1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['style.main.ticky'] = tag

    # style.tickx
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.tickx'] = tag

    # style.ticky
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.ticky'] = tag

    # style.main.borderSize
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0.2'))
    tag["values"].append(self.createTagTestItem('0.01'))
    tags['style.main.borderSize'] = tag

    # style.main.borderMode
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('1'))
    tag["values"][-1]["conditions"]['style.main.borderSize'] = '0.2'
    tags['style.main.borderMode'] = tag

    # geometry.canvas.width
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('800'))
    tag["values"].append(self.createTagTestItem('600'))
    tags['geometry.canvas.width'] = tag

    # geometry.canvas.height
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('600'))
    tag["values"].append(self.createTagTestItem('800'))
    tags['geometry.canvas.height'] = tag

    # style.logScale
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"][-1]["conditions"]["style.min"] = '1'
    tag["testWith"] = ['style.logScaleX']
    tags['style.logScale'] = tag

    # style.sub.logScale
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"][-1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['style.sub.logScale'] = tag

    # style.logScaleX
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['style.logScaleX'] = tag

    # style.showSub
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("***NO VALUE***"))
    tag["values"].append(self.createTagTestItem("false"))
    tag["values"].append(self.createTagTestItem("true"))
    tags['style.showSub'] = tag

    # labels.axes.mainX
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("***NO VALUE***"))
    tag["values"].append(self.createTagTestItem("label X"))
    tags['lables.axes.mainX'] = tag

    # labels.axes.mainY
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("***NO VALUE***"))
    tag["values"].append(self.createTagTestItem("label Y"))
    tags['lables.axes.mainY'] = tag

    # cuts.0
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("***NO VALUE***"))
    tag["values"].append(self.createTagTestItem('5'))
    tag["testWith"] = ['cuts.1', 'style.cutLineStyle', 'style.cutLineWidth', 'style.cutLineColor']
    tags['cuts.0'] = tag

    # cuts.1
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("***NO VALUE***"))
    tag["values"].append(self.createTagTestItem('15'))
    tag["values"][-1]["conditions"]['cuts.0'] = '5'
    tags['cuts.1'] = tag

    # style.cutLineStyle
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("7"))
    tag["values"].append(self.createTagTestItem('9'))
    tag["values"][-1]["conditions"]['cuts.0'] = '5'
    tags['style.cutLineStyle'] = tag

    # style.cutLineWidth
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("2"))
    tag["values"].append(self.createTagTestItem('5'))
    tag["values"][-1]["conditions"]['cuts.0'] = '5'
    tags['style.cutLineWidth'] = tag

    # style.cutLineColor
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("kRed"))
    tag["values"].append(self.createTagTestItem('kBlue'))
    tag["values"][-1]["conditions"]['cuts.0'] = '5'
    tags['style.cutLineColor'] = tag


    # style.titleOffset
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("1"))
    tag["values"].append(self.createTagTestItem('1.2'))
    tags['style.titleOffset'] = tag

    # style.textSize
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("0.05"))
    tag["values"].append(self.createTagTestItem('0.07'))
    tag["testWith"] = ['geometry.main.xAxis.textSize']
    tags['style.textSize'] = tag

    # geometry.legendPadRatio
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("0.25"))
    tag["values"].append(self.createTagTestItem('0.35'))
    tags['geometry.legendPadRatio'] = tag

    # style.topLegend
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem("false"))
    tag["values"].append(self.createTagTestItem('true'))
    tags['style.topLegend'] = tag

    # style.tickLength
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0.03'))
    tag["values"].append(self.createTagTestItem('0.02'))
    tags['style.tickLength'] = tag

    # style.sub.yAxis.nDiv
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('505'))
    tag["values"].append(self.createTagTestItem('510'))
    tag["values"][-1]["conditionsNOT"]["style.subPlot"] = '***NO VALUE***'
    tags['style.sub.yAxis.nDiv'] = tag

    # style.ratio.nYdiv
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('505'))
    tags['style.ratio.nYdiv'] = tag

    # errors.totalBkgSys
    # cannot test systematics easily here
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    # tag["values"].append(self.createTagTestItem(''))
    tags['errors.totalBkgSys'] = tag

    # style.showUnderflow
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['style.showUnderflow'] = tag

    # style.showOverflow
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tag["testWith"] = ['style.showUnderflow']
    tags['style.showOverflow'] = tag

    # input.channel

    # style.showEventYields
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['style.showEventYields'] = tag

    # style.showEventYields.useUnderOverflow
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"][-1]["conditions"]['style.showEventYields'] = 'true'
    tags['style.showEventYields.useUnderOverflow'] = tag

    # geometry.legend.height
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('0.4'))
    tags['geometry.legend.height'] = tag

    # style.legend.fillColor
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('kBlue'))
    tag["values"][-1]["conditions"]['style.legend.fillStyle'] = '1001'
    tags['style.legend.fillColor'] = tag

    # style.legend.fillStyle
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('1001'))
    tag["values"][-1]["conditions"]['style.legend.fillColor'] = 'kBlue'
    tags['style.legend.fillStyle'] = tag

    # style.legend.textSize
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('0.032'))
    tag["values"].append(self.createTagTestItem('0.04'))
    tag["values"][0]["conditions"]['style.legend.textSizeFixed'] = 'false'
    tag["values"][1]["conditions"]['style.legend.textSizeFixed'] = 'false'
    tag["values"].append(self.createTagTestItem('0.05'))
    tag["values"].append(self.createTagTestItem('0.04'))
    tag["values"][2]["conditions"]['style.legend.textSizeFixed'] = 'true'
    tag["values"][3]["conditions"]['style.legend.textSizeFixed'] = 'true'

    # style.legend.textSizeFixed
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tags['style.legend.textSizeFixed'] = tag

    # normalize
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['normalize'] = tag

    # normalizeWithoutOverUnderflow
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tag["testWith"] = ['normalize']
    tag["values"][1]["conditions"]['normalize'] = 'true'
    tags['normalizeWithoutOverUnderflow'] = tag

    # normalizeToTotalBkg
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['normalizeToTotalBkg'] = tag

    # errors.drawStatMC
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['errors.drawStatMC'] = tag

    # style.showTotalBkg
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.showTotalBkg'] = tag

    # style.drawData
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.drawData'] = tag

    # style.data.asymErrors
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tags['style.data.asymErrors'] = tag

    # style.showLegend
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.showLegend'] = tag

    # optScan.FOMmode
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('s/sqrt(s+b)'))
    tag["values"][-1]["conditions"]['style.showOptScan'] = 'true'
    tag["testWith"] = ['optScan.FOMbbb']
    tags['optScan.FOMmode'] = tag

    # optScan.FOMbbb
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('false'))
    tag["values"].append(self.createTagTestItem('true'))
    tag["values"][1]["conditions"]['style.showOptScan'] = 'true'
    tag["values"][2]["conditions"]['style.showOptScan'] = 'true'
    tags['optScan.FOMbbb'] = tag

    # style.showCustom
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('false'))
    tags['style.showCustom'] = tag

    # style.ratioFormula
    tag = self.getEmptyTag()
    tag["values"].append(self.createTagTestItem('***NO VALUE***'))
    tag["values"].append(self.createTagTestItem('data/bkg'))
    tag["values"].append(self.createTagTestItem('data/(sig+bkg)'))
    tag["values"].append(self.createTagTestItem('sig/(sqrt(bkg))'))
    tag["values"][1]["conditions"]['style.showCustom'] = 'true'
    tag["values"][2]["conditions"]['style.showCustom'] = 'true'
    tag["values"][3]["conditions"]['style.showCustom'] = 'true'
    tags['style.ratioFormula'] = tag

    return tags

  def test_simplePlot(self):
    """
    This function tests the TQPlotter class. It generates plots based
    on the settings in self.defineTestTags(). After many combinations
    of tags are created (admittedly, in some rare cases the algorithm
    does not find all possible combinations), plots are printed in .C
    format. Next, the timestamp is removed to make plots comparable.

    If the current branch is master, the test case ends and the newly
    created plots are uploaded as artifacts for later comparisons.
    If the current branch is not master, then the newly created plots
    are compared to the artifacts of the most recent master commit,
    which is an ancestor of the current commit (i.e. the master
    commit, from which the current feature branch branched off).

    """
    self.init()

    tags = self.defineTestTags()
    self.printMessage("  Creating tag combinations for testing.")
    combinations, canBuildTestTags = self.createTestTags(tags)
    self.printMessage("  Created "+ str(len(combinations)) + " combinations of tags for plotting.")
    self.assertTrue(canBuildTestTags)

    self.printMessage('  Plotting them now in directory "'+self.dotCFilesFolder+'".')
    self.plotCombinations(combinations, "listOfHashes.txt")
    self.replaceTimeStamp()

    if not compare:
      self.printMessage("  No comparison with artifacts requested. This option should not be used in the CI.")
      self.printMessage("  Exiting.")
      return

    onMaster = False
    if self.isCommitOnOriginMasterHistory(self.getCurrentCommit()) and self.referenceCommit == "":
      # If no reference commit is specified, then the master branch is taken as
      # default for comparisons. Since the user is on the master branch, this
      # will be defined as new default and not compared to previous commits.
      self.printMessage("  You are currently on the master branch. A comparison is only performed to determine if new plot files need to be uploaded. The pipeline should succeed in any case.")
      onMaster = True


    if not artifactsFolder:
      maxNoIterations = 5
      artifactsFound, artifactsPlotFolder, artifactsPlotFolderPDF, referenceInfo = self.searchForArtifacts(maxNoIterations, onMaster)
      self.assertTrue(artifactsFound)
    else:
      from os.path import join as pjoin
      artifactsPlotFolder = pjoin(artifactsFolder,"dotCFiles")
      artifactsPlotFolderPDF = pjoin(artifactsFolder,"pdfFiles")
      referenceInfo = {"hash":"unknown"}

    self.printMessage("  Comparing .C files.")
    allFilesEqual, filesWithDifferences = self.compareDotCFiles(self.dotCFilesFolder, artifactsPlotFolder)
    self.compareDotPDFFiles(self.dotPDFFilesFolder, artifactsPlotFolderPDF, referenceInfo["hash"])

    if not allFilesEqual and not onMaster and filesWithDifferences != []:
      allFilesEqual, newFilesWithDifferences = self.fuzzyMatchDotCFiles(self.dotCFilesFolder, artifactsPlotFolder, filesWithDifferences)

    if allFilesEqual and referToOldPlotsIfIdentical:
      # If this file is written, a script will not upload the plots
      # to gitlab, but only this file. Note that this method finishes
      # early if on the master branch. So this file will never be
      # written and plots won't be uploaded.
      self.writeReadmeFile(referenceInfo)
    self.assertTrue(allFilesEqual or onMaster)

if __name__ == "__main__":
  from argparse import ArgumentParser
  parser = ArgumentParser()
  parser.add_argument("--protocol",type=str,dest="protocol",default="https")
  parser.add_argument("--plotter",type=str,dest="plotter",default=plotterClassDefault) #don't hardcode a default here, this segment doesn't run in all environments (RootCore). Hence, we need to make sure the default plotter name is also set to a valid one at some other place in the code (see settings at the top of this script)
  parser.add_argument("--select",type=str,nargs="*",dest="select",default=[])
  parser.add_argument("--verbose",action="store_true",dest="verbose",default=verboseDefault)
  parser.add_argument("--compare",action="store_true",dest="compare",default=True)
  parser.add_argument("--no-compare",action="store_false",dest="compare",default=False)
  parser.add_argument("--token", type=str, dest="token", default="")
  parser.add_argument("--consecutive-numbering", action="store_false", dest="hashNumbering", default=True)
  parser.add_argument("--hash-numbering", action="store_true", dest="hashNumbering", default=True)
  parser.add_argument("--reference", type=str, dest="reference", default="")
  parser.add_argument("--artifacts-folder", type=str, dest="artifactsFolder", default="")
  parser.add_argument("--git-dir", type=str, dest="git_dir", default="")
  args = parser.parse_args()
  import sys
  sys.argv = sys.argv[0:1]
  protocol = args.protocol
  select = args.select
  plotterClass = args.plotter
  verbose = args.verbose
  compare = args.compare
  token = args.token
  hashNumbering = args.hashNumbering
  reference = args.reference
  artifactsFolder = args.artifactsFolder
  git_dir = args.git_dir
  if git_dir == "":
    command = "git rev-parse --is-inside-work-tree"
    p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, executable='/bin/bash')
    out, err = p.communicate()
    if not out.decode("ascii").strip('\n ') == "true":
      # take an educated guess if we are not in a git directory
      git_dir = "/CAFCore/source/.git"
  unittest.main()
