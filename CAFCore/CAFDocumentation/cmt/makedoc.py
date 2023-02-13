#!/bin/env python2
# -*- python -*-

def sysexec(cmd):
    import subprocess
    subprocess.call(cmd,shell=True)

def readfile(fname):
    try:
        import xml.etree.ElementTree as ET
        tree = ET.parse(fname)
        return tree.getroot()
    except Exception as err:
        raise RuntimeError("unable to parse file '"+fname+"': "+str(err))

def indent(elem, level=0):
  i = "\n" + level*"  "
  if len(elem):
    if not elem.text or not elem.text.strip():
      elem.text = i + "  "
    if not elem.tail or not elem.tail.strip():
      elem.tail = i
    for elem in elem:
      indent(elem, level+1)
    if not elem.tail or not elem.tail.strip():
      elem.tail = i
  else:
    if level and (not elem.tail or not elem.tail.strip()):
      elem.tail = i

def generateSitemap(targetdir,url,pages):
    import xml.etree.ElementTree as ET
    from os.path import join as pjoin

    root = ET.Element("urlset",xmlns="http://www.sitemaps.org/schemas/sitemap/0.9")
    for page in pages:
        elem = ET.SubElement(root, "url")
        loc = ET.SubElement(elem, "loc").text = pjoin(url.rstrip("/"),page.lstrip("./"))
    
    indent(root)
    tree = ET.ElementTree(root)
    tree.write(pjoin(targetdir,"sitemap.xml"))

def generateHTML(basedir,targetdir,url):
    pages = []
    from sys import argv
    from os import environ as env
    from os.path import join as pjoin

    sysexec('mkdir -p '+targetdir)
    suffix = "index.php?search=%s"
   
    inputdirs = [ pjoin(basedir,"QFramework","QFramework"), pjoin(basedir,"QFramework","Root"), 
                  pjoin(basedir,"SFramework","SFramework"), pjoin(basedir,"SFramework","Root"), 
                  env["ROOTSYS"]+"/include" ]

    inputdir = ":".join(inputdirs)
    print(inputdir)

    import ROOT
    ROOT.gROOT.SetBatch(1)
    html = ROOT.THtml()
    html.SetProductName("Common Analysis Framework")    
    html.SetSearchStemURL(url+suffix)
    html.SetSearchEngine(url+suffix)
    html.SetOutputDir(targetdir)
    html.SetHeader(pjoin(targetdir,"header.html"))
    html.SetInputDir(inputdir)
    html.MakeIndex("TQ*")

    import QFramework
    for k, v in QFramework.__dict__.iteritems(): 
        if k.startswith("TQ") and not "_" in k:
            html.MakeClass(k)
            pages.append(html.GetHtmlFileName(k))

    try:
        import SFramework
        html.MakeIndex("TS*")
        for k, v in SFramework.__dict__.iteritems(): 
            if k.startswith("TS") and not "_" in k:
                html.MakeClass(k)
                pages.append(html.GetHtmlFileName(k))
    except ImportError:
        pass

    html.CreateAuxiliaryFiles()
    html.CreateHierarchy()
    return pages

def generateMain(targetdir,sourcedir,url,cseid,dynamic):
    from os.path import join as pjoin
    import xml.etree.ElementTree as ET
    root = ET.Element("html")
    meta = ET.SubElement(root, "meta", {'http-equiv':"Content-Type",'content':"text/html; charset=utf-8"})

    if dynamic:
        phphead = ET.SubElement(root, '?', """php
            function getFileContents($fname){
              if(file_exists($fname))
                return addslashes(trim(preg_replace('/\s+/', ' ', file_get_contents($fname))));
              else 
                return "";
            }""")

    head = ET.SubElement(root, "head")
    ET.SubElement(head, "title").text="Common Analysis Framework - Online Documentation"
    ET.SubElement(head, "link",rel="stylesheet",type="text/css",href="content/styles/style.css")
    ET.SubElement(head, "link",rel="stylesheet",type="text/css",href="content/styles/tooltips.css")
    ET.SubElement(head, "script",src="content/scripts/tooltips.js",type="text/javascript").text=" "
    ET.SubElement(head, "script",src="content/scripts/email.js",type="text/javascript").text=" "
    ET.SubElement(head, "script",src="content/scripts/togglediv.js",type="text/javascript").text=" "

    body = ET.SubElement(root, "body")
    bodycontent = ET.SubElement(body, "div", id="body_content")
    rootheader =  ET.SubElement(bodycontent, "div", id="root_header").text=" "
    descrhead =  ET.SubElement(bodycontent, "div", id="descrhead")
    descrheadcontent =  ET.SubElement(descrhead, "div", id="descrheadcontent").text=" "

    ET.SubElement(bodycontent, "h1").text = 'Common Analysis Framework'
    ET.SubElement(bodycontent, "p").text = 'This website provides online documentation for the Common Analysis Framework, an extensive common, ROOT-based framework targeted at histogramming, nTuple and xAOD analysis with bindings to C++ and python. As the framework itself, it is constantly under developement.'
    ET.SubElement(bodycontent, "p").text = 'Type your search query here to search the documentation with Google!'

    ET.SubElement(bodycontent, "script").text="""
  (function() {
    var cx = '"""+cseid+"""';
    var gcse = document.createElement('script');
    gcse.type = 'text/javascript';
    gcse.async = true;
    gcse.src = 'https://cse.google.com/cse.js?cx=' + cx;
    var s = document.getElementsByTagName('script')[0];
    s.parentNode.insertBefore(gcse, s);
  })();"""
    ET.SubElement(bodycontent, "gcse:search").text=" "

    ET.SubElement(bodycontent, "p").text="""You can access further documentation under the following links:"""
    doclist = ET.SubElement(bodycontent, "ul")
    twiki = ET.SubElement(doclist, "li")
    ET.SubElement(twiki, "span").text="""Run 2 Analysis Code Guide """
    ET.SubElement(twiki, "a", href='https://twiki.cern.ch/twiki/bin/view/AtlasProtected/HWWAnalysisCode').text="""TWiki"""
    tags = ET.SubElement(doclist, "li")
    ET.SubElement(tags, "span").text="""Documentation of Tags and Options:"""
    ET.SubElement(tags, "a", href='tags.html').text="""html"""
    envVars = ET.SubElement(doclist, "li")
    ET.SubElement(envVars, "span").text="""Documentation of environment variables read by CAF:"""
    ET.SubElement(envVars, "a", href='env.html').text="""html"""
    actions = ET.SubElement(doclist, "li")
    ET.SubElement(actions, "span").text="""Documentation of SFramework Actions:"""
    ET.SubElement(actions, "a", href='SFrameworkActions/index.html').text="""html/src"""    
    observables = ET.SubElement(doclist, "li")
    ET.SubElement(observables, "span").text="""Documentation of observables:"""
    ET.SubElement(observables, "a", href='observables.html').text="""html"""        
    tuts2016 = ET.SubElement(doclist, "li")
    ET.SubElement(tuts2016, "span").text="""Slides from the """
    ET.SubElement(tuts2016, "a", href='https://indico.cern.ch/event/587413/timetable/?view=standard').text="""HWW Workshop 2016"""
    tuts2014 = ET.SubElement(doclist, "li")
    ET.SubElement(tuts2014, "span").text="""Tutorials from 2014 and 2015:"""
    ET.SubElement(tuts2014, "a", href='http://atlas-caf-material.web.cern.ch/pdf/HSG3AnalysisCode.pdf').text="""Introductory Tutorial (12/2014),"""
    ET.SubElement(tuts2014, "a", href='http://atlas-caf-material.web.cern.ch/pdf/observables.pdf').text="""Observable Tutorial (4/2015)"""
    codejam = ET.SubElement(doclist, "li")
    ET.SubElement(codejam, "span").text="""March 2013 VBF CodeJam Slides:"""
    ET.SubElement(codejam, "a", href='http://atlas-caf-material.web.cern.ch/pdf/Talk_HWWAnalysisCode_Guide.pdf').text="""Plotting and Cutflows,"""
    ET.SubElement(codejam, "a", href='http://atlas-caf-material.web.cern.ch/pdf/VBF_CAF_20130325_CodeJam_Hsu_v2.pdf').text="""VBF Analysis Tips,"""
    ET.SubElement(codejam, "a", href='http://atlas-caf-material.web.cern.ch/pdf/CAF-Improvements.pdf').text="""Recent Improvements"""
    tuts2012 = ET.SubElement(doclist, "li")
    ET.SubElement(tuts2012, "span").text="""May 2012 Tutorial Slides:"""
    ET.SubElement(tuts2012, "a", href='http://atlas-caf-material.web.cern.ch/pdf/Setup.pdf').text="""Setup,"""
    ET.SubElement(tuts2012, "a", href='http://atlas-caf-material.web.cern.ch/pdf/Tutorial_Introduction.pdf').text="""Introduction,"""
    ET.SubElement(tuts2012, "a", href='http://atlas-caf-material.web.cern.ch/pdf/Tutorial_Session1.pdf').text="""Session 1,"""
    ET.SubElement(tuts2012, "a", href='http://atlas-caf-material.web.cern.ch/pdf/Tutorial_Session2.pdf').text="""Session 2"""

    ET.SubElement(bodycontent, "p").text="""This section contains a number of frequently asked questions:"""
    faqlist = ET.SubElement(bodycontent, "ul")
    for question,element in [
        ('How do I compile the library?','download-code'),
        ('What is $TQPATH?','tqpath'),
        ("How do I load the library in ROOT?",'load-library'),
        ("How can I use the library in python?",'load-python'),
        ("Can I read xAOD files?",'xAOD'),
        ("How can I compare cutflows and counters?",'compare-cutflows'),
        ("How can I compare shapes of different variables or at different cut stages?",'compare-shapes'),
        ("How do I apply a reweighting at the analysis code stage?",'reweighting'),
        ("How can I read nTuples residing on EOS?",'eosget'),
        ]:
        li = ET.SubElement(faqlist,'li')
        ET.SubElement(li,'a',{'class':"faq",'onclick':"toggleDiv('"+element+"')"}).text=question
        answer = ET.SubElement(li,'div',{'class':"faq",'id':element,'style':"display:none"})
        if dynamic:
            answer.text = "<?php include 'content/res/"+element+".html'; ?>"
        else:
            answer.append(readfile(pjoin(sourcedir,"content","res",element+".html")))

    ET.SubElement(bodycontent, "p").text="""Additional Links:"""
    morelinks = ET.SubElement(bodycontent, "ul")
    bugtracker = ET.SubElement(morelinks, "li")
    ET.SubElement(bugtracker, "span").text="""Bug tracker """
    ET.SubElement(bugtracker, "a", href='https://its.cern.ch/jira/browse/HWWATLAS/component/18503/?selectedTab=com.atlassian.jira.jira-projects-plugin:component-summary-panel').text="""JIRA"""
    gitlab = ET.SubElement(morelinks, "li")
    ET.SubElement(gitlab, "span").text="""Project on """
    ET.SubElement(gitlab, "a", href='https://gitlab.cern.ch/atlas-caf/CAFCore').text="""gitlab"""
    patentry = ET.SubElement(morelinks, "li")
    ET.SubElement(patentry, "span").text="""PAT User Analysis Tools List Entry """
    ET.SubElement(patentry, "a", href='https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/PATUserAnalysisTools#HWWAnalysisCode').text="""TWiki"""

    indent(root)
    tree = ET.ElementTree(root)
    if dynamic:
        tree.write(pjoin(targetdir,"index.php"))
    else:
        tree.write(pjoin(targetdir,"index.html"))


if __name__ == "__main__":
    # parse the CLI arguments
    from argparse import ArgumentParser
    parser = ArgumentParser(description='auto-generate a header file with some local definitions')
    parser.add_argument('--outputdir', type=str, help='path of output directory to be created',required=True)
    parser.add_argument('--url', type=str, help='url of final page',required=True)
    parser.add_argument('--cse', type=str, help='google cseid to be used',required=True)
    parser.add_argument('--htmldir', type=str, help='path of html input resources',required=True)
    parser.add_argument('--basedir', type=str, help='base directory of all the input packages',required=True)
    args = parser.parse_args()
    sysexec('rm -rf '+args.outputdir)
    sysexec('cp -r '+args.htmldir+" "+args.outputdir)
    pages = generateHTML(args.basedir,args.outputdir,args.url)
    generateSitemap(args.outputdir,args.url,pages)
    generateMain(args.outputdir,args.htmldir,args.url,args.cse,False)

