//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef STATISTICS_PLOTTER
#define STATISTICS_PLOTTER

#include "TCanvas.h"
#include "TH1.h"
#include "QFramework/TQFolder.h"

class TGraph;
class TGraph2D;

using namespace std;

class TSStatisticsPlotter {

	private:

		TQFolder * fTemplates;

		void applyStyle(TH1 * h);
		void setStyleAtlas();
		Double_t getMaxRange(Double_t min, Double_t max, Double_t vetoFrac, Bool_t logScale);


	public:

		TSStatisticsPlotter();

    static TQFolder* convertGraph(TGraph* g);
  static TGraph2D* createGraph2D(TQFolder* source,TQFolder* config);
    static TQFolder* findGraphMinimum(TQFolder* source);


		void setTemplates(TQFolder * templates);

		TCanvas * plot(TQFolder * input);

		virtual ~TSStatisticsPlotter();
		
		ClassDef(TSStatisticsPlotter, 0);

};

#endif
