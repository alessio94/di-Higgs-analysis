def cleanup(text):
    return ''.join(i for i in text if ord(i)<128)

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

def generateSitemap(targetdir,url,repo,pages):
  import xml.etree.ElementTree as ET
  from os.path import join as pjoin

  xml = ET.Element("urlset",xmlns="http://www.sitemaps.org/schemas/sitemap/0.9")
  html = ET.Element("html",xmlns="http://www.w3.org/1999/xhtml")
  body = ET.SubElement(html, "body")
  table = ET.SubElement(body, "table")
  th = ET.SubElement(table, "tr")
  ET.SubElement(th, "th").text = "HTML"
  ET.SubElement(th, "th").text = "CXX"      
  for page in sorted(pages.keys()):
    xmlelem = ET.SubElement(xml, "url")
    htmlfile = page.lstrip("./")+".html"
    loc = ET.SubElement(xmlelem, "loc").text = pjoin(url.rstrip("/"),htmlfile)

    tr = ET.SubElement(table, "tr")
    src = pages[page]
    if repo:
        src = pjoin(repo,src)
    ET.SubElement(ET.SubElement(tr, "td"),"a",href=htmlfile).text=page
    ET.SubElement(ET.SubElement(tr, "td"),"a",href=src).text="source"
    
  indent(xml)
  xmltree = ET.ElementTree(xml)  
  xmltree.write(pjoin(targetdir,"sitemap.xml"))

  htmltree = ET.ElementTree(html)  
  htmltree.write(pjoin(targetdir,"index.html"))  

def main(args):
  pathList = args.path
  errorFound = False

  import re
  startRE = re.compile(".*<cafdoc name=([^ ].*)>(.*)")
  endRE = re.compile("(.*)</cafdoc>.*")
  
  allDoc = {}
  filemap = {}  
  import os
  
  for path in pathList:
    with open(path) as f:
      currentName = None
      for line in f:
        start = startRE.match(line)
        end = endRE.match(line)
        if start:
          currentName = start.group(1)
          if not currentName: continue
          if not currentName in allDoc:
            fname = os.path.split(path)[1]
            filemap[currentName] = fname
            text = start.group(2)
            if len(text.strip())>0:
              allDoc[currentName] = [text]
            else:
              allDoc[currentName] = []
            continue
        if end:
          allDoc[currentName].append(end.group(1)+"\n")
          currentName = None
        if currentName:
          allDoc[currentName].append(line)

  import markdown2
  md = markdown2.Markdown(extras=["fenced-code-blocks","cuddled-lists"])
  
  from os.path import join as pjoin

  for name,doc in allDoc.items():
    indent = -1
    newdoc = []
    for line in doc:
      if indent < 0: indent = len(line) - len(line.lstrip())
      newdoc.append(line[indent:])
    text = "".join(newdoc)

    with open(pjoin(args.output,name+".md"),"wt") as outmd:
      outmd.write(text)
    with open(pjoin(args.output,name+".html"),"wt") as outhtml:
      outhtml.write(md.convert(cleanup(text)))

    print("wrote "+name)

  generateSitemap(args.output,args.url,args.repo,filemap)

  return 0


if __name__ == "__main__":
  # parse the CLI arguments
  import argparse
  parser = argparse.ArgumentParser(description='Generates a documentation of tags used in the QFramework.')
  parser.add_argument('path', metavar='PATH', type=str, default="$(TQPATH)/Root/*", nargs = "*", help='path to the framework files to be read')
  parser.add_argument('--output', metavar='OUTPUT', type=str, default=".", help='output directory path')
  parser.add_argument('--url', type=str, help='url of final page',required=True)
  parser.add_argument('--repo', type=str, help='base url of the gitlab repository',required=False, default=None)  
  args = parser.parse_args()
  #get rid of command line arguments since ROOT is very greedy here (and tends to choke from it!)

  import QFramework
  QFramework.TQLibrary.getQLibrary().setApplicationName("makeMarkdownDoc")
  errorFound = main(args)
  import sys
  sys.exit(errorFound)
