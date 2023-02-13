#!/usr/bin/env python

import unittest
import os
from ROOT import TString
from QFramework import TQPythonTest,TQFolder,TQCut
from CommonAnalysisHelpers.analyze import PyCut,loadCuts
import tempfile
#os.environ['CAFCOREDIR'] = os.path.join(str(TQLibrary.getQLibrary().getTQPATH().Data()),"..")
tempFileBase     = os.path.join(tempfile.gettempdir(),tempfile.gettempprefix())
tempPathPyCut    = tempFileBase+"cuts.py"
tempPathTQFolder = tempFileBase+"cuts.txt"


#dummyFileName = "dummy.py"
#dummyScriptPath = os.path.join(str(TQPathManager.readEnvVarValue("CAFCOREDIR")),"CommonAnalysisHelpers","share","algorithms",dummyFileName)



class pyCutTest(TQPythonTest):
    def setUp(self):
        #create the files to be imported
        with open(tempPathPyCut,"w") as pcf:
            pcf.write(

"""
#!/usr/bin/env python
from CommonAnalysisHelpers.analyze import PyCut

def addCuts(config,baseCut):
  print("adding cuts via python...")
  #use += if you want to continue assigning more subsequent cuts
  baseCut+= PyCut("CutChannels", cut="$(fitsChannel)", weight="Weight_$(weightname):$(cand)", options={".title":"Channel Selection"} )
  baseCut+= PyCut("CutVgammaVjet_overlap", cut="{ $(isVjets) ? $(Truth_hasFSRPhotonDR01)==0 : 1 }", options={".title":"Overlap: Vgamma/Vjets"} )
  baseCut+= PyCut("CutOtherLep", cut="$(lllFinalState)", options={".title":"lll final state"} )
  baseCut+= PyCut("CutZMass", cut="[ZBosonPairFakeIndex]==3 || [ZBosonPairFakeIndex]==2 || [ZBosonPairFakeIndex]==1", options={".title":"Z-tagging"} )
  baseCut+= PyCut("CutLeptonsPt", cut="$(lep0).pt() > 15000 && $(lep1).pt() > 15000 && $(otherPart0).pt() > 15000", options={".title":"lep $p_T > 15$ GeV"} )
  baseCut+= PyCut("CutWZVeto", cut="$(PassWZVeto)", options={".title":"WZ veto"} )

  #create cut branches after "CutWZVeto"
  fakeEl = baseCut["CutWZVeto"]
  fakeEl += PyCut("CutFakeEl", cut="$(fakeLepCand_type) == $(electron)", options={".title":"fake type: electron"} )
  fakeEl += PyCut("CutEtaFakeElec", cut="(fabs($(fakeLepCand_eta)) < 2.47 && (fabs($(fakeLepCand_eta)) < 1.37 || fabs($(fakeLepCand_eta)) > 1.52))", options={".title":"Fake el eta cut"} )

  #create more sub branches (without explicit re-assignment)
  fakeEl["CutEtaFakeElec"] + PyCut("CutFakeElecID", cut = "$(fakeLepCand_id)", options={".title": "Fake el ID"} )
  fakeEl["CutEtaFakeElec"] + PyCut("CutFakeElecAntiID", cut = "$(fakeLepCand_antiid)", options={".title": "Fake el Anti-ID"} )

  #create cut branches after "CutWZVeto"
  fakeMu = baseCut["CutWZVeto"]
  fakeMu += PyCut("CutFakeMu", cut="$(fakeLepCand_type) == $(muon)", options={".title":"fake type: muon"} )
  fakeMu += PyCut("CutEtaFakeMuon", cut="fabs($(fakeLepCand_eta)) < 2.5", options={".title":"Fake mu |eta|<2.5"} )

  #create more sub branches (without explicit re-assignment)
  fakeMu["CutEtaFakeMuon"] + PyCut("CutFakeMuonID", cut = "$(fakeLepCand_id)", options={".title": "Fake mu ID"} )
  fakeMu["CutEtaFakeMuon"] + PyCut("CutFakeMuonAntiID", cut = "$(fakeLepCand_antiid)", options={".title": "Fake mu Anti-ID"} )


  return True
"""
            )
        with open(tempPathTQFolder,"w") as tcf:
            tcf.write(
"""
+CutChannels {
    <.cutExpression = "$(fitsChannel)", .weightExpression = "Weight_$(weightname):$(cand)", .title="Channel Selection">
    +CutVgammaVjet_overlap {
        <.cutExpression = "{ $(isVjets) ? $(Truth_hasFSRPhotonDR01)==0 : 1 }",  .title="Overlap: Vgamma/Vjets">
        +CutOtherLep {
            <.cutExpression = "$(lllFinalState)", .title = "lll final state">

            # find the fake index by finding the most optimal Z-boson pair
            +CutZMass {
                <.cutExpression = "[ZBosonPairFakeIndex]==3 || [ZBosonPairFakeIndex]==2 || [ZBosonPairFakeIndex]==1", .title = "Z-tagging">

                # make sure all leptons are above 15 GeV
                +CutLeptonsPt {
                    <.cutExpression = " $(lep0).pt() > 15000 && $(lep1).pt() > 15000 && $(otherPart0).pt() > 15000", .title = "lep $p_T > 15$ GeV">

                    +CutWZVeto {
                        <.cutExpression = "$(PassWZVeto)",   .title = "WZ veto">

                        +CutFakeEl {
                            <.cutExpression = "$(fakeLepCand_type) == $(electron)", .title = "fake type: electron">
                            +CutEtaFakeElec {
                                <.cutExpression = "(fabs($(fakeLepCand_eta)) < 2.47 && (fabs($(fakeLepCand_eta)) < 1.37 || fabs($(fakeLepCand_eta)) > 1.52))", .title = "Fake el eta cut">

                                ##ID Selection
                                +CutFakeElecID {
                                    <.cutExpression = "$(fakeLepCand_id)", .title = "Fake el ID">

                                }
                                ##Anti-ID selection
                                +CutFakeElecAntiID {
                                    <.cutExpression = "$(fakeLepCand_antiid)", .title = "Fake el Anti-ID">

                                }

                            } #End: CutEtaFakeElec
                        } #End: CutFakeEl
                        +CutFakeMu {
                            <.cutExpression = "$(fakeLepCand_type)== $(muon)", .title = "fake type: muon">
                            +CutEtaFakeMuon {
                                <.cutExpression = "fabs($(fakeLepCand_eta)) < 2.5", .title = "Fake mu |eta|<2.5 ">

                                ##ID Selection
                                +CutFakeMuonID {
                                    <.cutExpression = "$(fakeLepCand_id)", .title = "Fake mu ID">

                                }
                                ##Anti-ID selection
                                +CutFakeMuonAntiID {
                                    <.cutExpression = "$(fakeLepCand_antiid)", .title = "Fake mu Anti-ID">

                                }

                            } #End: CutEtaFakeMuon
                        } #End: CutFakeMu
                    } #End: CutWZVeto
                } #End: CutLeptonsPt
            } #End: CutZMass
        } #End: CutOtherLep
    } #End: CutVgammaVjet_overlap
} #End: CutChannels
"""
            )

        return

    def tearDown(self):
        os.remove(tempPathPyCut)
        os.remove(tempPathTQFolder)
        return

    def compareCuts(self,cutA,cutB):
        #helper function for test_compareImports
        #compare cut expressions
        self.assertEqual(cutA.getCutExpression().Data(),cutB.getCutExpression().Data())
        #compare weight expressions
        self.assertEqual(cutA.getWeightExpression().Data(),cutB.getWeightExpression().Data())
        #compare titles
        self.assertEqual(cutA.GetTitle(),cutB.GetTitle())
        subCutsA = cutA.getCuts()
        subCutsB = cutB.getCuts()
        #make sure we have the same number of sub cuts
        self.assertEqual(subCutsA.GetEntries(),subCutsB.GetEntries())
        for subA in subCutsA:
            #get the corresponding subcut from B explicitly
            subB = cutB.getCut(subA.GetName())
            self.assertIsNot(subB,None)
            #iterate over sub-cuts
            self.compareCuts(subA,subB)
        return

    @unittest.skip("")
    def test_compareImports(self):
        #create a minimal config
        config = TQFolder("config")
        config.setTagString("cuts.0",tempPathPyCut)
        cuts_py = loadCuts(config)
        config.setTagString("cuts.0",tempPathTQFolder)
        cuts_tqfolder = loadCuts(config)
        #compare cuts

        return


if __name__ == "__main__":
    unittest.main()
