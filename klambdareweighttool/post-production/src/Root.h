#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TH1.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

class root_code{
 public:
  void OpenFile();

  float Get1DFrac(TH1F* h, int mhh_bin);

  void Get1DHist(TString file, TString truth_mhh, TString tree_name, float SCALE);

};
