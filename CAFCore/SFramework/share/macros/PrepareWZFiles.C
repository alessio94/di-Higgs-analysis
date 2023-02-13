#include "../../lib/libQFramework.so"


void error(TString msg) {
  // print error message
	cout << TQStringUtils::makeBoldRed(msg).Data() << endl;
}



int main(TString nomFilename, TString wzFilename) {

	// load nominal from standard file
	TQSampleFolder * samples = TQSampleFolder("samples");
	if (!samples->importObject(nomFilename + ":samples_Nominal >>::samples_Nominal")) {
		error("Failed to load nominal sample folder. Stopping!");
		return -1;
	}

	// load WZ input
	TQSampleFolder * wzSamples = TQSampleFolder::loadSampleFolder(wzFilename + ":samples_Nominal");
	if (!wzSamples) {
		error("Failed to load WZ sample folder. Stopping!");
		return -1;
	}
	
	// ===== prepare and merge inputs =====

	// make a copy of MT histograms
	samples->copyHistograms("*/MT*", "_fromWZCR");

	// create a new set of variations corresponding to d-d WZ shape
	samples->addObject(samples->getFolder("samples_Nominal")->copy("samples_WWShapeFromWZCRUp"));
	samples->addObject(samples->getFolder("samples_Nominal")->copy("samples_WWShapeFromWZCRDown"));
	
	
	
	

	// ====================================
	
	// write stuff to output file
	TFile * file = TFile::Open(nomFilename, "UPDATE");
	TQIterator itr(samples->getListOfFolders("?"), true);
	while (itr.hasNext()) {
		((TQFolder*)itr.readNext())->Write("", TObject::kOverwrite);
	}
	file->Close();

	// cleanup
	delete file;
	delete samples;
	delete wzSamples;
}