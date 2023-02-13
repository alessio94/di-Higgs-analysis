#!/usr/bin/env python

# Taken and modified from https://gitlab.cern.ch/ksuruliz/Scripts/tree/master
# Does not support all of the original options. Very hacky changes...

# Put this script in your gitlab base directory and execute it like this:
'''
setupATLAS
lsetup git
lsetup python
lsetup gitlab
git checkout master
git pull
./release_notes.py 18.09.0 master -t <your gitlab token> --branch master
                 (earliest latest)                      (what does this do?)
'''

import datetime
import time
import glob
import re

pyGitLabNotFound = False

try:
    import gitlab
except:
    pyGitLabNotFound = True


gl_project = None

description = """
Prepare ATLAS release notes with a list of merge requests

davide.gerbaudo@gmail.com
kerim.suruliz@gmail.com
Jul 2017
"""
usage = """
%prog target_release nightly_tag

where
target_release is the release you are creating, e.g. release/21.1.6
nightly_tag is the nightly tag the release will be based on, e.g. nightly/21.1/2017-06-07T2215

Example:
git clone  ssh://git@gitlab.cern.ch:7999/atlas/athena.git
cd athena

./release_notes.py release/21.2.11 release/21.2.12 --labels  -t $GITLAB_TOKEN --twiki

or

./release_notes.py release/21.1.10 nightly/21.1/2017-08-14T2135 --branch 21.1 --project AthenaP1 --labels -t $GITLAB_TOKEN --verbose

 ./release_notes.py  --labels -t $GITLAB_TOKEN --twiki

(this will dump the changes in the last --nnightlies nightlies.

"""


import optparse
import os
import re
import string
import subprocess

import sys

class gitlabHelper:

    def __init__(self,**kwargs):

        # get options (from command line arg parser)
        for key, value in kwargs.items():
            setattr(self, key, value)

        self.gl = gitlab.Gitlab("https://gitlab.cern.ch",self.token,api_version=4)
        self.gl_project = self.gl.projects.get("atlas-caf/CAFCore")

    def get_nightly_name(self,dirs,project):

        nightly_name = ""

        for directory in dirs:
            subdirs = glob.glob(directory+"/*")

            for subdir in subdirs:
                if (subdir.split("/")[-1]==project):
                    nightly_name = subdir.split("/")[-2]
                    break

        return nightly_name

    def get_tag_date(self,nightly_tag):
        cmd = "git log -1 --format=%ai "+nightly_tag
        #print cmd
        p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        out, err = p.communicate()
        return out.split()[0]

    def parseCommit(self,commitLines,commits):
            # dict to store commit data
            commit = {}

            # iterate lines and save
            for nextLine in commitLines:

                    if nextLine == '' or nextLine == '\n':
                            # ignore empty lines
                            pass
                    elif bool(re.match('commit', nextLine, re.IGNORECASE)):
                            # commit xxxx
                            if len(commit) != 0:		## new commit, so re-initialize
                                    commits.append(commit)
                                    commit = {}
                            commit = {'hash' : re.match('commit (.*)', nextLine, re.IGNORECASE).group(1) }
                    elif bool(re.match('merge:', nextLine, re.IGNORECASE)):
                            # Merge: xxxx xxxx
                            pass
                    elif bool(re.match('author:', nextLine, re.IGNORECASE)):
                            # Author: xxxx <xxxx@xxxx.com>
                            m = re.compile('Author: (.*) <(.*)>').match(nextLine)
                            commit['author'] = m.group(1)
                            commit['email'] = m.group(2)
                    elif bool(re.match('date:', nextLine, re.IGNORECASE)):
                            # Date: xxx
                            commit['date'] = nextLine.split()[3]+"/"+nextLine.split()[2]+"/"+nextLine.split()[5]
                            pass
                    #elif (("See merge request" in nextLine) and ("atlas-caf/CAFCore" in nextLine)):
                    elif ("See merge request" in nextLine):
                            # See merge request
                            if commit.get('mr_id') is None:
                                    mr_id = nextLine.split()[-1].replace("!","").replace("atlas-caf/CAFCore","")

                                    # the next line is needed in recent gitlab versions which include the repo name in the git log

                                    # it may happen that the repo name is not atlas-caf/CAFCore, if there was a merge request
                                    # from another repo into the one from which the merge request is made - see
                                    # https://gitlab.cern.ch/<...>/merge_requests/7160/commits for an example

                                    mr_id = int(''.join([c for c in mr_id if c in '1234567890.']))
                                    commit['mr_id'] = mr_id


                                    #mr = self.gl_project.mergerequests.list(iid=mr_id)[0]
                                    print "Trying to retrieve MR ",mr_id
                                    mr = self.gl_project.mergerequests.get(mr_id)

                                    #print mr

                                    commit['title'] = mr.title
                                    commit['target_branch'] = mr.target_branch
                                    commit['description'] = mr.description
                                    commit['labels'] = mr.labels
                                    commit['changes'] = list_changed_packages(mr)


                    elif bool(re.match('    ', nextLine, re.IGNORECASE)):
                            # (4 empty spaces)
                            if commit.get('message') is None:
                                    commit['message'] = nextLine.strip()
                    else:
                            print ('ERROR: Unexpected Line: ' + nextLine)



            commits.append(commit)



    def dump_mrinfo(self,previous_release,nightly_tag,project,package_list):

        next_release = ""

        if ("release" in previous_release):
            next_release = previous_release.rsplit(".",1)[0]+"."+str(int(previous_release.split(".")[-1])+1)

        cmd = "git log --full-history --merges "+ previous_release+".."+nightly_tag+" -- "+package_list
        #cmd = "git log --merges "+ previous_release+".."+nightly_tag

        commits = []

        if self.verbose:
            print "Executing:"
            print cmd
        output_log = get_command_output(cmd)

        self.parseCommit(output_log['stdout'].split('\n'),commits)

        if (pyGitLabNotFound):
            print "WARNING: MR descriptions requested but pygitlab not found - descriptions won't be printed"
            descriptions = False


        fout = open(self.output,"a")

        rtag = (nightly_tag.split("-")[-1])[0:2]

        tag_date = self.get_tag_date(nightly_tag)


        if (self.twiki):
            if ("release" not in nightly_tag):
                rtag = (nightly_tag.split("-")[-1])[0:2]
                fout.write( "---++ [[#r"+rtag+"][Changes in "+nightly_tag.split("/")[-1]+" (r"+rtag+")]]\n")
            else:
                rtag = (nightly_tag.split("/")[-1])
                fout.write( "---+++ [[#r"+rtag+"]["+rtag+" (built on "+tag_date+")]]\n")
        else:
            if ("release" in previous_release) and ("nightly" in nightly_tag):
                fout.write("# Release notes for "+next_release+" (built on "+tag_date+")\n")
                fout.write("The release\n ["+next_release+"](https://gitlab.cern.ch/atlas-caf/CAFCore/tags/"+next_release+")\n")
                fout.write("was built from the tag ["+nightly_tag+"](https://gitlab.cern.ch/atlas-caf/CAFCore/tags/"+nightly_tag+")\n\n")
                fout.write("This is the list of merge requests that were included since the previous release\n ["+previous_release+"](https://gitlab.cern.ch/atlas-caf/CAFCore/tags/"+previous_release+")\n")


        for commit in commits:

            if (self.verbose):
                if ('target_branch' in commit):
                    print "Processing commit ",commit," with target branch ",commit['target_branch']


            if ('mr_id' in commit and commit['target_branch']==self.branch):

                if (self.filter!=''):
                    containscommit = False

                for f in commit['changes']:
                    if (self.filter in f):
                        containscommit = True
                if (not containscommit):
                    continue

                if (self.project=="AnalysisTop" and "AnalysisTop" not in commit['labels']):
                    continue
                if (self.project=="AthDerivation" and "Derivation" not in commit['labels']):
                    continue

                #if ((project=="AnalysisBase" or project=="AthAnalysis") and "Derivation" in commit['labels']):
                #    continue

                #if ((self.project=="AnalysisBase" or self.project=="AthAnalysis") and "AnalysisTop" in commit['labels']):
                #    continue

                mr_link = ""
                mr_descr = ""

                d = commit['description']
                d = d.strip("\n")
                d = d.replace('\r', "")
                d = d.replace("\n\n", "\n")
                d = d.replace("\n\n", "\n")
                d = d.replace("\n\n", "\n")

                allLinesBulletPoints = True
                description = ""
                for line in d.split("\n"):
                    # Remove the "Release notes" and "Details" lines
                    # Remove everything that's listed under details
                    if line.strip() == "# Release notes":
                        continue
                    if line.strip() == "# Details":
                        break
                    if re.match(r"\s*-\s+", line) == None:
                        allLinesBulletPoints = False
                    description = description + "\n" + line

                # If all lines start with " - ", remove the pattern
                if allLinesBulletPoints:
                    description = re.sub(r"\n\s*-\s+", "\n", description)
                # Format with bullet points
                description = description.replace("\n", "\n   * ")
                description.strip("\n ")

                # Create links for issues
                issues = re.findall(r"Closes #[0-9]+", description)
                for issue in issues:
                    number = issue.replace(r"Closes #", "")
                    link = "https://gitlab.cern.ch/atlas-caf/CAFCore/issues/"+number
                    replaceBy = "Closes [#" + number + "]("+ link +")"
                    description = description.replace(issue, replaceBy)

                if (self.twiki):
                    mr_link = "[[https://gitlab.cern.ch/atlas-caf/CAFCore/merge_requests/"+str(commit['mr_id'])+"]["+str(commit['mr_id'])+"]]"
                    mr_descr = (mr_link+'   '+ commit['title']).encode('latin-1','ignore')
                else:
                    mr_descr = (" * [!"+str(commit['mr_id'])+"](https://gitlab.cern.ch/atlas-caf/CAFCore/merge_requests/"+str(commit['mr_id'])+") : "+commit['title'] + description).encode('latin-1','ignore')

                if self.patches:
                    ## This doesn't give me the diff in the way I want it :( Leaving it here in case someone knows better
                    #diff = self.gl_project.commits.get(commit['hash']).diff()

                    fDate = time.strptime(commit['date'],"%d/%b/%Y")
                    fileName = '%s-%i-%02i-%02i_MR%i.patch' % (self.filter if self.filter else "AllPatches",
                                                               fDate.tm_year, fDate.tm_mon, fDate.tm_mday,
                                                               commit['mr_id'])

                    ### Can't find a way to do this via API :( Leaving it here in case someone knows better
                    os.system('git diff %s~ %s > %s' % (commit['hash'], commit['hash'], fileName))


                if (self.twiki):
                    if (not self.descriptions):
                        mr_descr = "   * "+mr_descr

                if (not self.labels):
                    fout.write(mr_descr+"\n")
                else:

                #fout.write(commit['target_branch']+"\n")

                    fout.write(mr_descr+" (")

                    labelList = commit['labels']

                    # clean the labels to be included in the release notes
                    removeLabels = ["review-approved", "review-pending-", "21.2", "sweep:", "urgent", "alsoTargeting:",
                                    "Squash commits", "RC Attention Required"]
                    labelsToRemove = [lab for lab in labelList for l in removeLabels if l in lab]
                    for l in labelsToRemove:
                        labelList.remove(l)

                    for label in commit['labels']:
                        label = label.strip()
                        if (label=="Recommendation"):
                            if (self.twiki):
                                label = "%ICON{twiki-alert}%%RED%"+label+"%ENDCOLOR%"
                            else:
                                label = "__**"+label+"**__"

                        if (label!=commit['labels'][-1].strip()):
                            fout.write(label+" ")
                        else:
                            fout.write(label)
                    fout.write(")\n")

                if (self.descriptions):
                    fout.write("-------\n")
                    fout.write(commit['description']+"\n")
                    fout.write("-------\n")


                if (self.changes):
                    fout.write("------Affected packages------\n")
                    #print commit['changes']
                    for f in commit['changes']:
                        fout.write(f+"\n")
                    fout.write("-------\n")

        fout.close()


def main():
    #global gl_project
    parser = optparse.OptionParser(description=description, usage=usage)
    parser.add_option('-p', '--previous', help='previous release  wrt. which we diff')
    parser.add_option('-o', '--output', default='release_notes.md', help='where the notes are written')
    parser.add_option('-r', '--relaxed', action='store_true', help='do not stop on dubious configurations')
    parser.add_option('-n', '--nnightlies', type=int, default=20, help='number of last nightlies to dump changes for')
    parser.add_option('-v', '--verbose', action='store_true', help='print more info')
    parser.add_option('-P', '--project', default='AnalysisBase', help='specify project')
    parser.add_option('-d', '--descriptions', action='store_true', help='include MR descriptions in output')
    parser.add_option('-l', '--labels', action='store_true', help='include MR labels in output')
    parser.add_option('-c', '--changes', action='store_true', help='include lists of changed files in output')
    parser.add_option('-f', '--filter', default='', help='only dump MRs which affect the specified package')
    parser.add_option('-u', '--packagelist', action='store_true', help='restrict log to only include changes in packages which are part of the project')
    parser.add_option('-b', '--branch', default='21.2', help='branch name')
    parser.add_option('-T', '--twiki', action='store_true', help='output in twiki format')
    parser.add_option('-t', '--token',help='private GitLab user token')
    parser.add_option('-a', '--patches', action='store_true', help='construct a patch file for each commit')

    (opts, args) = parser.parse_args()

    if not opts.token:   # if filename is not given
        parser.error('You need to specify a personal access token with -t <token>')
        sys.exit(1)



    #sanitize_args(target_release, nightly_tag, keep_going=opts.relaxed)

    #previous_release = guess_previous_and_check(target_release=target_release) if not opts.previous else opts.previous
    verbose = opts.verbose
    pretty_format = '%b' # perhaps some combination of '%s%n%b' ?

    package_list = ""

    project = opts.project

    branchname = opts.branch

    usepackagelist = opts.packagelist

    if (usepackagelist):
        if (project!=''):
            package_list = get_package_list(project,verbose)

    previous_release = ""
    nightly_tag = ""

    output = opts.output
    descriptions = opts.descriptions
    labels = opts.labels
    changes = opts.changes
    filter_packages = opts.filter
    twiki = opts.twiki
    patches = opts.patches

    #sys.exit(1)

    glhelper = gitlabHelper(**vars(opts))

    if (len(args)==0):

        nnightlies = opts.nnightlies # number of nightlies to get output for

        dt = datetime.datetime.now().date() - datetime.timedelta(1)
        date = (datetime.datetime.today()-datetime.timedelta(1)).strftime('%Y-%m-%d')
        yesterday_date = (datetime.datetime.today()-datetime.timedelta(2)).strftime('%Y-%m-%d')

        if (project=="AthDerivation"):
            date = (datetime.datetime.today()).strftime('%Y-%m-%d')
            yesterday_date = (datetime.datetime.today()-datetime.timedelta(1)).strftime('%Y-%m-%d')

        cnt = 1

        while (cnt<nnightlies):
            date = (datetime.datetime.today()-datetime.timedelta(cnt)).strftime('%Y-%m-%d')
            yesterday_date = (datetime.datetime.today()-datetime.timedelta(cnt+1)).strftime('%Y-%m-%d')

            if (project=="AthDerivation"):
                date = (datetime.datetime.today()-datetime.timedelta(cnt-1)).strftime('%Y-%m-%d')
                yesterday_date = (datetime.datetime.today()-datetime.timedelta(cnt)).strftime('%Y-%m-%d')


            dirs = [ x for x in glob.glob("/cvmfs/atlas-nightlies.cern.ch/repo/sw/"+branchname+"/*") if date in x]

            yesterday_dirs = [ x for x in glob.glob("/cvmfs/atlas-nightlies.cern.ch/repo/sw/"+branchname+"/*") if yesterday_date in x]




            if (dirs==[]):
                print "couldn't find nightly for ",date
            elif (yesterday_dirs==[]):
                print "couldn't find nightly for ",yesterday_date
            else:
                # now figure out the nightly name for the selected project

                nightly_name = glhelper.get_nightly_name(dirs,project)
                yesterday_nightly_name =  glhelper.get_nightly_name(yesterday_dirs,project)

                #print "dirs ",dirs," nightly_name ",nightly_name
                #print "yesterday_dirs ",yesterday_dirs," yesterday_nightly_name ",yesterday_nightly_name

                if (nightly_name=="" or yesterday_nightly_name==""):
                    print "No nightly for current date ",date
                    fout = open(output,"a")
                    fout.write( "---++ *No nightly for "+date+"*\n")
                    fout.close()
                    cnt = cnt + 1
                    continue

                previous_release = "nightly/"+branchname+"/"+yesterday_nightly_name
                nightly_tag = "nightly/"+branchname+"/"+nightly_name



                #print "previous release ",previous_release
                #print "nightly tag ",nightly_tag

                previous = previous_release
                current = nightly_tag


                if (branchname=="21.1" and project=="AthenaP1"):
                    # this is almost the whole of athena, so don't bother picking out packages
                    package_list = ""

                glhelper.dump_mrinfo(previous,current,project,package_list)

            cnt = cnt + 1

        #sys.exit(1)
    elif len(args)==2:
        previous = args[0]
        current = args[1]
        glhelper.dump_mrinfo(previous,current,project,package_list)
    else:
        parser.error("either run with arguments or specify two tags")


    sys.exit(1)




def sanitize_args(target_release, nightly_tag, keep_going=False):
    if not target_release.startswith('release/'):
        print "The target release should start with 'release/', you are using:\n%s"%target_release
        if not keep_going:
            raise RuntimeWarning('Fix target release')
    if not nightly_tag.startswith('nightly/'):
        print "The nightly tag should start with 'nightly/', you are using:\n%s"%nightly_tag
        if not keep_going:
            raise RuntimeWarning('Fix nightly tag')

    rel_match = re.search(r'release/(?P<ver>\d+)\.(?P<maj>\d+?)\.(?P<rev>\d+?)', target_release)
    nig_match = re.search(r'nightly/(?P<branch>(\d+\.\d+))/(?P<date>\d{4}-\d{2}-\d{2})T(?P<time>\d{4})', nightly_tag)
    if not rel_match:
        print "The target release is not formatted as xx.y.z (version.major.revision semantic)"
        if not keep_going:
            raise RuntimeWarning('Fix formatting target release')
    elif not nig_match:
        print "The nightly tag is not formatted as expected, nightly/branch/yyyy-mm-ddThhmm\n%s"%nightly_tag
        if not keep_going:
            raise RuntimeWarning('Fix formatting nightly tag')
    else:
       branch_rel = '.'.join([rel_match.group('ver'), rel_match.group('maj')])
       branch_nig = nig_match.group('branch')
       if branch_rel!=branch_nig:
           print "You are creating a tag for %s from a nightly from %s" % (branch_rel, branch_nig)
           if not keep_going:
               raise RuntimeWarning('Create a tag for the correct branch')

def guess_previous_and_check(target_release='release/xx.y.z'):
    previous_release = None
    version_major_revision = target_release.split('.')
    revision = version_major_revision[-1] if len(version_major_revision)>2 else None
    missing_revision = revision is None or not str(revision).isdigit()
    if missing_revision or int(revision)==0:
        perhaps_previous = target_release[:-1]+'n-1'
        raise RuntimeWarning("Cannot guess previous release for '%s'" % target_release
                             +'\nPlease use something like'
                             +"\n --previous %s" % perhaps_previous)
    else:
        previous_revision = int(revision)-1
        version_major_previousrevision = version_major_revision[:-1] + [str(previous_revision)]
        previous_revision = '.'.join(version_major_previousrevision)
    return previous_revision

def get_command_output(command, with_current_environment=False):
     "lifted from supy (https://github.com/elaird/supy/blob/master/utils/io.py)"
     env = None if not with_current_environment else os.environ.copy()
     p = subprocess.Popen(command, shell = True, stdout = subprocess.PIPE, stderr = subprocess.PIPE, env=env)
     stdout, stderr = p.communicate()
     return {"stdout":stdout, "stderr":stderr, "returncode":p.returncode}

def get_package_list(project,verbose=False):

    f = open("Projects/"+project+"/package_filters.txt")

    gitline = ""

    for l in f.readlines():
        #print l

        if ("#" in l):
            continue

        if ("-" in l):
            continue

        package = l.strip().replace("+ ","").replace("/.*","")

        newname = package.split('/')[0]

        if (("+" in package) or ("*" in package)):
            dirs = package.split('/')

            for i in range(1,len(dirs)-1):
                newname = newname+"/"+dirs[i]

        else:
            newname = package

        if (os.path.isdir(newname)):
            gitline = gitline+" "+newname

    if (verbose):
        print gitline

    return gitline

def list_changed_packages(mr):
    """
    mr ... Gitlab merge request object

    return: sorted set of package names affected by this merge request
    """
    changed_files = set([c[p] for c in mr.changes()['changes'] for p in ['old_path','new_path']])

    #print "changed_files: ",changed_files

    return sorted(set([map_filename_to_package(f) for f in changed_files]))


def map_filename_to_package(fname):
    """
    fname ... full path of filename

    note: only works if this file resides in <ATHENA_ROOT>/CI

    return: package path for given full file path
    """
    # get Athena root directory (specific to current layout) which is one level up
    athena_root = os.path.dirname(os.path.abspath(os.path.join(os.path.realpath(__file__),'.')))
    #logging.debug("found Athena root directory '%s'",athena_root)

    # start from directory name
    pkg_name = os.path.dirname(fname)

    # recursively move up in directory hierarchy and look for CMakeLists.txt
    while pkg_name and pkg_name != '/':
        if os.path.isfile(os.path.join(athena_root,pkg_name,'CMakeLists.txt')):
            break
        pkg_name = os.path.dirname(pkg_name)

    #logging.debug("mapped file '%s' to package '%s'" % (fname,pkg_name))

    return pkg_name

if __name__=='__main__':

    #mrs = project.mergerequests.list()

    #print mrs
    #commit = project.mergerequests.get(48845)
    #mr = project.mergerequests.list(iid=3182)[0]

    #print mr

    main()
