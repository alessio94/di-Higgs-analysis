int main() {

  /* load the library, because ROOT needs to know about the classes */
  gSystem->Load("$TQPATH/lib/libQFramework.so");
  gSystem->Load("lib/libQStatistics.so");

  THtml html;
  html.SetProductName("HWW Common Statistics Framework");

  TString rootinclude = gSystem->GetIncludePath();
  TString rootheaders = rootinclude(rootinclude.First('"')+1,rootinclude.Last('"')-rootinclude.First('"')-1);
  
  TString tmpfile = "/tmp/tqpath";
  gSystem->Exec("echo $TQPATH > "+tmpfile);
  TString CAFbase;
  ifstream in(tmpfile.Data());
  in >> CAFbase;
  TString CAFheaders = CAFbase + "/include";
  TString CAFsources = CAFbase + "/src";

  TString url = "http://wwwhep.physik.uni-freiburg.de/~cburgard/CAFstats-doc/";

  html.SetSearchStemURL(url+"index.php?search=%s");
  html.SetSearchEngine(url+"index.php?search=%s");

  /* define input and output directories */
  html.SetInputDir(".:include:"+rootheaders+":"+CAFheaders+":"+CAFsources);
  html.SetOutputDir("doc");

  /* open the list of classes*/
  std::ifstream * file = new std::ifstream("doc/classes.txt");
  /* check if file exists */
  if (file->is_open()) {

    /* read line by line */
    std::string line;
    while (getline(*file, line)) {
      TString tline = line.c_str();
      Ssiz_t spos = tline.Last('/');
      if(spos != kNPOS)
	tline.Remove(0, spos+1);
      Ssiz_t fpos = tline.Last('.');
      if (fpos != kNPOS)
	tline.Remove(fpos, tline.Length()-fpos);
      html.MakeClass(tline.Data());
    }
  }

  file->close();
  delete file;

  html.CreateAuxiliaryFiles();
  html.CreateHierarchy();

  gSystem->Exec("rm doc/classes.txt");

  return 0;

}

void Create_Documentation(){
  main();
}
