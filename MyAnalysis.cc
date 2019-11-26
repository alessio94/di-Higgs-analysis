// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/PromptFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Math/Vector4.hh"
#include "Rivet/Projections/WFinder.hh"
namespace Rivet {

/// @brief MC validation analysis for isolated di-photon events
	class MC_2LEP : public Analysis {
		public:

		/// Constructor
		MC_2LEP()
		: Analysis("MC_2LEP")
		{
		}


		void init() {



//Cuts definition
			Cut eta_full = (Cuts::abseta < 5.0);
			FinalState fs0(eta_full);

			IdentifiedFinalState leptons_ifs(Cuts::abseta < 2.5 && Cuts::pT>10.*GeV);
			vector<PdgId> pids={PID::ELECTRON,PID::MUON,PID::POSITRON,PID::ANTIMUON};
			leptons_ifs.acceptIds(pids);
			addProjection(leptons_ifs, "IFS");
      


			IdentifiedFinalState neutrinos_ifs(Cuts::abseta < 2.5 && Cuts::pT>10.*GeV);
			vector<PdgId> pidsn={PID::NU_E,PID::NU_EBAR,PID::NU_MU,PID::NU_MUBAR};
			neutrinos_ifs.acceptIds(pidsn);
			addProjection(neutrinos_ifs, "IFS2");

     
			VetoedFinalState fs(FinalState(Cuts::abseta < 4.2));
			fs.addVetoOnThisFinalState(leptons_ifs);
			FastJets fj (fs, FastJets::ANTIKT, 0.4);
			addProjection(fj, "Jets");

//plots
     
			_h_pT_lep_lead = bookHisto1D("pT_lep_lead",logspace( 50, 1.0, 500.0));
			_h_pT_lep_sublead = bookHisto1D("pT_lep_sublead",logspace( 50, 1., 500.0));
			_h_pT_bjet_lead = bookHisto1D("pT_bjet_lead",logspace( 50, 1., 500.0));
			_h_pT_bjet_sublead = bookHisto1D("pT_bjet_sublead", logspace(50, 1.0, 500.0));
			_h_deltaeta_bjet = bookHisto1D("deltaeta_bjet", 50, 0., 6.);
			_h_deltaeta_lep = bookHisto1D("deltaeta_lep", 50, 0., 6.);
			_h_deltaphi_bjet = bookHisto1D("deltaphi_bjet", 50, 0., 3.14159);
			_h_deltaphi_lep = bookHisto1D("deltaphi_lep", 50, 0., 3.14159);
			_h_deltaR_lep = bookHisto1D("deltaR_lep", 50, 0., 5);
			_h_deltaR_bjet = bookHisto1D("deltaR_bjet", 50, 0., 5);
			_h_pT_bb = bookHisto1D("pT_bb",logspace( 50, 1., 500.));
			_h_pT_ll = bookHisto1D("pT_ll", logspace(50, 1., 500.));
			_h_M_bb = bookHisto1D("M_bb", logspace(50, 1., 500.));
			_h_M_ll = bookHisto1D("M_ll", logspace(50, 1., 500.));
			_h_deltaR_bjet_lep = bookHisto1D("deltaR_bjet_lep", 50, 0., 5.);
			_h_M_bjet = bookHisto1D("M_bjet", logspace(50, 1., 100.));
			_h_pT_neu = bookHisto1D("pT_neu", 50, .0, 500.0);
			_h_M_bl_minM = bookHisto1D("M_bl_minM",logspace( 50, 1., 500.));
			_h_M_bl_maxE = bookHisto1D("M_bl_maxE",logspace( 50, 1., 500.));
			_h_M_W = bookHisto1D("M_W", 100, 79., 82.);
			}

 
//analysis
		void analyze(const Event& event) {
			const double weight = event.weight();
	
			FourMomentum bl1, bl2, bl3, bl4;
			FourMomentum b1, b2, btot, n1;
			FourMomentum l1, l2, ltot, n2;
			FourMomentum miss, Wp, Wm;
			FourMomentum lepp, lepm, ne, neb;
			Particles leptons = applyProjection<IdentifiedFinalState>(event, "IFS").particles();
			Particles neutrinos = applyProjection<IdentifiedFinalState>(event, "IFS2").particles();
			const FastJets& jetpro = apply<FastJets>(event, "Jets");
			const Jets jets = discardIfAnyDeltaRLess(jetpro.jetsByPt(25*GeV), leptons, 0.4);
//selection events 	
			if (leptons.size() < 2) {
				vetoEvent;
				}
			if (neutrinos.size() < 2) {
				vetoEvent;
				}
			sortByPt(leptons);
			if (leptons[0].charge()*leptons[1].charge()> 0)
				{
				vetoEvent;
				}
			std::vector<Rivet::Jet> Bjets;
		
			for (const Jet& jet : jets) {
				if (jet.bTagged())  Bjets.push_back(jet);
		
				}
		
			if (Bjets.size() != 2) {
				vetoEvent;
			}
//assign 4vectors their values		
			double b1px = Bjets[0].px();
			double b2px = Bjets[1].px();
			b1.setPx(b1px);
			b2.setPx(b2px);
			double b1py = Bjets[0].py();
			double b2py = Bjets[1].py();
			b1.setPy(b1py);
			b2.setPy(b2py);
			double b1pz = Bjets[0].pz();
			double b2pz = Bjets[1].pz();
			b1.setPz(b1pz);
			b2.setPz(b2pz);
			double b1E = Bjets[0].E();
			double b2E = Bjets[1].E();
			b1.setE(b1E);
			b2.setE(b2E);
		
			double n1px = neutrinos[0].px();
			double n2px = neutrinos[1].px();
			n1.setPx(n1px);
			n2.setPx(n2px);
			double n1py = neutrinos[0].py();
			double n2py = neutrinos[1].py();
			n1.setPy(n1py);
			n2.setPy(n2py);
			double n1pz = neutrinos[0].pz();
			double n2pz = neutrinos[1].pz();
			n1.setPz(n1pz);
			n2.setPz(n2pz);
			double n1E = neutrinos[0].E();
			double n2E = neutrinos[1].E();
			n1.setE(n1E);
			n2.setE(n2E);
		
		      
			btot = b1 + b2;
			double PTbb;
			PTbb = btot.pT();
			double Mbb;
			Mbb = btot.mass(); 

			double l1px = leptons[0].px();
			double l2px = leptons[1].px();
			l1.setPx(l1px);
			l2.setPx(l2px);
			double l1py = leptons[0].py();
			double l2py = leptons[1].py();
			l1.setPy(l1py);
			l2.setPy(l2py);
			double l1pz = leptons[0].pz();
			double l2pz = leptons[1].pz();
			l1.setPz(l1pz);
			l2.setPz(l2pz);
			double l1E = leptons[0].E();
			double l2E = leptons[1].E();
			l1.setE(l1E);
			l2.setE(l2E);
			double misspx = miss.px();
			double misspy = miss.py();
			double misspz = miss.pz();
			miss.setPx(misspx);	
			miss.setPy(misspy);
			miss.setPz(misspz);
		
		 
			ltot = l1 + l2;
			double PTll;
			PTll = ltot.pT();
			double Mll;
			Mll = ltot.mass();
			double minR = std::min(std::min(deltaR(Bjets[0],leptons[0]),deltaR(Bjets[0],leptons[1])),std::min(deltaR(Bjets[1],leptons[0]),deltaR(Bjets[1],leptons[1])));
			double massbj1 = Bjets[0].mass();
			double massbj2 = Bjets[1].mass();
//neutrinos 
		
			if ((neutrinos[0].pid() > 0)) {
				double nepx = neutrinos[0].px();
				ne.setPx(nepx);
				double nepy = neutrinos[0].py();
				ne.setPy(nepy);	
				double nepz = neutrinos[0].pz();
				ne.setPz(nepz);
				double nepE = neutrinos[0].E();
				ne.setE(nepE);
				}		
		
			if ((neutrinos[1].pid() > 0)) {
				double nepx = neutrinos[1].px();
				ne.setPx(nepx);
				double nepy = neutrinos[1].py();
				ne.setPy(nepy);
				double nepz = neutrinos[1].pz();	
				ne.setPz(nepz);
				double neE = neutrinos[1].E();
				ne.setE(neE);
				}
		
			if ((neutrinos[0].pid() < 0)) {
				double nebpx = neutrinos[0].px();
				neb.setPx(nebpx);
				double nebpy = neutrinos[0].py();
				neb.setPy(nebpy);
				double nebpz = neutrinos[0].pz();
				neb.setPz(nebpz);
				double nebE = neutrinos[0].E();
				neb.setE(nebE);
				}
		
			if ((neutrinos[1].pid() < 0)) {
				double nebpx = neutrinos[1].px();
				neb.setPx(nebpx);
				double nebpy = neutrinos[1].py();
				neb.setPy(nebpy);
				double nebpz = neutrinos[1].pz();
				neb.setPz(nebpz);
				double nebE = neutrinos[1].E();
				neb.setE(nebE);
				}
//leptons
			if ((leptons[0].pid() > 0)) {
				double lepx = leptons[0].px();
				lepp.setPx(lepx);
				double lepy = leptons[0].py();
				lepp.setPy(lepy);
				double lepz = leptons[0].pz();
				lepp.setPz(lepz);
				double lepE =leptons[0].E();
				lepp.setE(lepE);
				}         
    		
			if ((leptons[1].pid() > 0)) {
				double lepx =leptons[1].px();
				lepp.setPx(lepx);
				double lepy = leptons[1].py(); 
				lepp.setPy(lepy);
				double lepz = leptons[1].pz();
				lepp.setPz(lepz);
				double leE = leptons[1].E();
				lepp.setE(leE);
				}
		
			if ((leptons[0].pid() < 0)) {
				double lebpx = leptons[0].px();
				lepm.setPx(lebpx);
				double lebpy = leptons[0].py();
				lepm.setPy(lebpy);
				double lebpz = leptons[0].pz();
				lepm.setPz(lebpz);
				double lebE = leptons[0].E();
				lepm.setE(lebE);
				}
		
			if ((leptons[1].pid() < 0)) {
				double lebpx = leptons[1].px();
				lepm.setPx(lebpx);	
				double lebpy = leptons[1].py();
				lepm.setPy(lebpy);
				double lebpz = leptons[1].pz();
				lepm.setPz(lebpz);
				double lebE = leptons[1].E();
				lepm.setE(lebE);
				}
		
		
		
			Wp = lepm + ne;
			Wm = lepp + neb;
		    
		     
//reconstructed mass lep + bjet
		
			bl1 = b1 + l1;
			bl2 = b1 + l2;
			bl3 = b2 + l1;
			bl4 = b2 + l2;
			int index, index2, ind;
			double max1, max2;
			double Mbl = std::min((std::min(bl1.mass(),bl2.mass())),(std::min(bl3.mass(),bl4.mass())));
			if (bl1.E() > bl2.E()) {
				index = 1;
				max1 = bl1.E();	
				}
			else {
				index = 2;
				max1 = bl2.E();
				}
			if (bl3.E() > bl4.E()) {
				index2 = 3;
				max2 = bl3.E();
				}
			else {
				index2 = 4;
				max2 = bl4.E();
				}
			if ((max1 > max2) && index == 1) {
				ind = 1;
				}	
			if ((max1 > max2) && index == 2) {
				ind = 2;
				}
			if ((max1 < max2) && index2 == 3) {
				ind = 3;
				}
			if ((max1 < max2) && index2 == 4) {
				ind = 4;
				}
			if (ind == 1) {
				_h_M_bl_maxE->fill(bl1.mass(), weight);
				}
			if (ind == 2) {
				_h_M_bl_maxE->fill(bl2.mass(), weight);
				}
			if (ind == 3) {
				_h_M_bl_maxE->fill(bl3.mass(), weight);
				}
			if (ind == 4) {
				_h_M_bl_maxE->fill(bl4.mass(), weight);
				}
//fill in plots
			_h_M_W->fill(Wp.mass(),weight);
			_h_M_W->fill(Wm.mass(),weight);
		 //     _h_pT_W->fill(PTW1,weight);
		 //     _h_pT_W->fill(PTW2,weight);
			_h_pT_lep_lead->fill(leptons[0].pT(), weight); 
			_h_pT_lep_sublead->fill(leptons[1].pT(), weight);	
			_h_pT_bjet_lead->fill(Bjets[0].pT(), weight);
			_h_pT_bjet_sublead->fill(Bjets[1].pT(), weight);
			_h_deltaeta_bjet->fill(deltaEta(Bjets[0],Bjets[1]), weight);
			_h_deltaeta_lep->fill(deltaEta(leptons[0],leptons[1]), weight);
			_h_deltaphi_lep->fill(deltaPhi(leptons[0],leptons[1]), weight);
			_h_deltaphi_bjet->fill(deltaPhi(Bjets[0],Bjets[1]), weight);
			_h_deltaR_lep->fill(deltaR(leptons[0],leptons[1]), weight);
			_h_deltaR_bjet->fill(deltaR(Bjets[0],Bjets[1]), weight);
			_h_pT_bb->fill(PTbb, weight);
			_h_M_bb->fill(Mbb, weight);
			_h_M_ll->fill(Mll, weight);	
			_h_pT_ll->fill(PTll, weight);
			_h_deltaR_bjet_lep->fill(minR, weight);
			_h_M_bjet->fill(massbj1,weight);
			_h_M_bjet->fill(massbj2,weight);
			_h_pT_neu->fill(neutrinos[0].pT(), weight);
			_h_pT_neu->fill(neutrinos[1].pT(), weight);
			_h_M_bl_minM->fill(Mbl, weight);
			_h_M_W->fill(Wp.mass(),weight);
			_h_M_W->fill(Wm.mass(),weight);
			}
		
//normalize	
		void finalize() {
			normalize(_h_pT_lep_lead); 
			normalize(_h_pT_lep_sublead);
			normalize(_h_pT_bjet_sublead);
			normalize(_h_pT_bjet_lead);
			normalize(_h_deltaeta_bjet);
			normalize(_h_deltaeta_lep);
			normalize(_h_deltaphi_lep);
			normalize(_h_deltaphi_bjet);
			normalize(_h_deltaR_lep);
			normalize(_h_deltaR_bjet);
			normalize(_h_pT_bb);
			normalize(_h_M_bb);	
			normalize(_h_M_ll);
			normalize(_h_pT_ll);
			normalize(_h_deltaR_bjet_lep);
			normalize(_h_M_bjet);
			normalize(_h_pT_neu);
			normalize(_h_M_bl_minM);
			normalize(_h_M_bl_maxE);
			normalize(_h_M_W);
			}
    


		private:

    

		Histo1DPtr _h_pT_lep_lead;
		Histo1DPtr _h_pT_lep_sublead;
		Histo1DPtr _h_pT_bjet_lead;
		Histo1DPtr _h_pT_bjet_sublead;
		Histo1DPtr _h_deltaeta_bjet;
		Histo1DPtr _h_deltaeta_lep;
		Histo1DPtr _h_deltaphi_bjet;
		Histo1DPtr _h_deltaphi_lep;
		Histo1DPtr _h_deltaR_bjet;
		Histo1DPtr _h_deltaR_lep;
		Histo1DPtr _h_pT_bb;
		Histo1DPtr _h_M_bb;
		Histo1DPtr _h_M_ll;
		Histo1DPtr _h_pT_ll;
		Histo1DPtr _h_deltaR_bjet_lep;
		Histo1DPtr _h_M_bjet;
		Histo1DPtr _h_pT_neu;
		Histo1DPtr _h_M_bl_minM;
		Histo1DPtr _h_M_bl_maxE;
		Histo1DPtr _h_M_W;
		};

  

// The hook for the plugin system
	DECLARE_RIVET_PLUGIN(MC_2LEP);

	}
