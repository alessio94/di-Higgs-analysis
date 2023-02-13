//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef MODEL_FACTORY_H
#define MODEL_FACTORY_H

#include "QFramework/TQFolder.h"
#include "RooWorkspace.h"
#include "RooStats/HistFactory/Measurement.h"

using namespace RooStats::HistFactory;

class TSModelFactory {

	private:

		Bool_t fShowInfo;
		Bool_t fShowError;
		Bool_t fShowWarn;

		void splitHistoLocation(TString input, TString &histoFile, TString &histoName, TString &histoPath);

		Bool_t addChannel(TQFolder * definition, RooStats::HistFactory::Measurement * measurement);
		Bool_t addSample(TQFolder * definition, RooStats::HistFactory::Channel * channel);
		Bool_t addNormFactor(TQFolder * definition, Sample * sample);
		Bool_t addOverallSys(TQFolder * definition, Sample * sample);
		Bool_t addHistoSys(TQFolder * definition, Sample * sample);
        Bool_t addShapeFactor(TQFolder * definition, Sample * sample);
		Bool_t addShapeSys(TQFolder * definition, Sample * sample);


	protected:

		virtual void info(TString message);
		virtual void error(TString message);
		virtual void warn(TString message);


	public:

		TSModelFactory();

		virtual RooStats::HistFactory::Measurement * createMeasurement(TQFolder * model);
    virtual Bool_t writeHistograms(TQFolder* model, const TString& histosFile);

		virtual Bool_t reviseWorkspace(RooWorkspace * workspace, TQFolder * model);

    virtual Bool_t createXML(RooStats::HistFactory::Measurement * measurement, const TString& xmlDir, const TString& histoFile);
		virtual Bool_t createXML(TQFolder * model, const TString& xmlDir);

		virtual ~TSModelFactory();
		
		ClassDef(TSModelFactory, 0);

};

#endif
