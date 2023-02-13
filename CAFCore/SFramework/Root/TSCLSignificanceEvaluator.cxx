#include "SFramework/TSCLSignificanceEvaluator.h"
#include "QFramework/TQIterator.h"
#include "TSystem.h"
#include "QFramework/TQSampleDataReader.h"
#include "QFramework/TQSampleFolder.h"
#include "TStopwatch.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQLibrary.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"
#include "TFile.h"
#include "TMath.h"

#include <iostream>
#include <fstream>

ClassImp(TSCLSignificanceEvaluator)

double TSCLSignificanceEvaluator::getLuminosity(TString folderName, TString tagName){
	// retrieve the luminosity value from the given location (default info/luminosity)
	// within the sample folder structure (and save it internally)
	folderName = TQFolder::concatPaths(variations[0],folderName);
	if(this->baseFolder && this->baseFolder->getFolder(folderName))
		this->baseFolder->getFolder(folderName)->getTagDouble(tagName,this->luminosity);
	return this->luminosity;
}

TSCLSignificanceEvaluator::TSCLSignificanceEvaluator(TQSampleFolder* sf, TString discVariable, TString name) : 
	TQSignificanceEvaluator(name,NULL),
	baseFolder(TQSampleFolder::newSampleFolder("base")),
	workingFolder(TQSampleFolder::newSampleFolder(name)),
	config(new TQFolder("config")),
	tmpFileName("histograms_tmp.root"),
	tmpFileNameBase(""),
	modelName(name),
	discriminantVariable(discVariable),
	engine(NULL),
	sampleFolderFileName("samples.root"),
	significanceFileName("significance.txt"),
	evaluationCommand(""),
	internalEvaluation(true),
	recyclingAllowed(false),
	supportRegionSetHandling(false),
	nPreparedRegionSets(0),
	sampleFolderMode(NONE),
	significanceMode(NONE)
{
	// specialized constructor taking the target sample folder
	// the discriminant variable (histogram variable)
	// and the name of the desired working folder
	if(sf){
		this->baseFolder->addObject(sf,"::Nominal");
		this->variations.push_back("Nominal");
	}
	this->reader = new TQSampleDataReader(this->baseFolder);
}

TSCLSignificanceEvaluator::TSCLSignificanceEvaluator(TList* folders, TString discVariable, TString name, TString folderKeyTag) : 
	TQSignificanceEvaluator(name,NULL),
	baseFolder(TQSampleFolder::newSampleFolder("base")),
	workingFolder(TQSampleFolder::newSampleFolder(name)),
	config(new TQFolder("config")),
	tmpFileName("histograms_tmp.root"),
	tmpFileNameBase(""),
	modelName(name),
	discriminantVariable(discVariable),
	engine(NULL),
	sampleFolderFileName("samples.root"),
	significanceFileName("significance.txt"),
	evaluationCommand(""),
	internalEvaluation(true),
	recyclingAllowed(false),
	supportRegionSetHandling(false),
	nPreparedRegionSets(0),
	sampleFolderMode(NONE),
	significanceMode(NONE)
{
	// specialized constructor taking the list of target sample folders
	// the discriminant variable (histogram variable)
	// and the name of the desired working folder
	this->reader = new TQSampleDataReader(this->baseFolder);
	if(!folders || folders->GetEntries() < 1) return;
	if(folders->GetEntries() == 1){
		TQSampleFolder* sf = dynamic_cast<TQSampleFolder*>(folders->First());
		this->baseFolder->addObject(sf,"::Nominal");
		this->variations.push_back("Nominal");
		return;
	}
	TQIterator itr(folders);
	while(itr.hasNext()){
		TQSampleFolder* sf = dynamic_cast<TQSampleFolder*>(itr.readNext());
		if(!sf) continue;
		TString sfname = sf->getName();
		if(!folderKeyTag.IsNull()) sf->getTagString(folderKeyTag,sfname);
		this->baseFolder->addObject(sf,"::"+sfname);
		if(TQStringUtils::matches(sfname,"*Nominal*"))
			this->variations.insert(this->variations.begin(),sfname);
		else
			this->variations.push_back(sfname);
	}
}

TSCLSignificanceEvaluator::TSCLSignificanceEvaluator(TString discVariable, TString name) : 
	TQSignificanceEvaluator(name,NULL),
	baseFolder(TQSampleFolder::newSampleFolder("base")),
	workingFolder(TQSampleFolder::newSampleFolder(name)),
	config(new TQFolder("config")),
	tmpFileName("histograms_tmp.root"),
	tmpFileNameBase(""),
	modelName(name),
	discriminantVariable(discVariable),
	engine(NULL),
	sampleFolderFileName("samples.root"),
	significanceFileName("significance.txt"),
	evaluationCommand(""),
	internalEvaluation(true),
	recyclingAllowed(false),
	supportRegionSetHandling(false),
	nPreparedRegionSets(0),
	sampleFolderMode(NONE),
	significanceMode(NONE)
{
	// default constructor 
	// taking the discriminant variable (histogram variable)
	// and the name of the desired working folder
	this->reader = new TQSampleDataReader(this->baseFolder);
}


void TSCLSignificanceEvaluator::deactivate(){
	// deactivate the evaluator, disabling all evaluation capabilities
	this->sampleFolderMode = NONE;
	this->significanceMode = NONE;
	this->internalEvaluation = false;
	this->evaluationCommand = "";
}

void TSCLSignificanceEvaluator::exportSampleFolder(bool doexport){
	// trigger exporting of the sample folder
	if(doexport)
		this->sampleFolderMode = EXPORT;
	else
		this->sampleFolderMode = NONE;
}
void TSCLSignificanceEvaluator::importSampleFolder(bool doimport){
	// trigger importing of the sample folder
	if(doimport){
		this->sampleFolderMode = IMPORT;
		this->setInternalEvaluation(true);
	} else
		this->sampleFolderMode = NONE;
}

void TSCLSignificanceEvaluator::exportSignificance(bool doexport){
	// trigger exporting of evaluation results
	if(doexport)
		this->significanceMode = EXPORT;
	else
		this->significanceMode = NONE;
}
void TSCLSignificanceEvaluator::importSignificance(bool doimport){
	// trigger importing of evaluation results
	if(doimport){
		this->significanceMode = IMPORT;
		this->setInternalEvaluation(false);
	} else 
		this->significanceMode = NONE;
}

void TSCLSignificanceEvaluator::setInternalEvaluation(bool eval){
	// set the evaluation to "internal" mode
	// currently not advised due to a rampant memory leak in HistFactory
	// https://sft.its.cern.ch/jira/browse/ROOT-5236
	this->internalEvaluation = eval;
	if(eval && this->significanceMode == IMPORT)
		this->significanceMode = NONE;
}

void TSCLSignificanceEvaluator::setExternalEvaluation(const TString& command, bool allowRecycling){
	// set the evaluation to "external" mode 
	// using the given command to be submitted to a console
	// currently the preferred way of evaluation
	if(command.IsNull()){
		this->internalEvaluation = true;
		return;
	}
	this->internalEvaluation = false;
	this->evaluationCommand = command;
	this->significanceMode = IMPORT;
	this->sampleFolderMode = EXPORT;
	this->recyclingAllowed = allowRecycling;
}

void TSCLSignificanceEvaluator::setSampleFolderFile(const TString& filename){
	// set the filename of the sample folder for import/export
	if(filename.IsNull())
		return;
	this->sampleFolderFileName = filename;
}

void TSCLSignificanceEvaluator::setRegionSetHandling(bool val){
	this->supportRegionSetHandling = val;
}

void TSCLSignificanceEvaluator::setSignificanceFile(const TString& filename){
	// set the filename for the import/export of results
	if(filename.IsNull())
		return;
	this->significanceFileName = filename;
}

void TSCLSignificanceEvaluator::keepTMP(TString tmpfilenamebase){
	// avoid deletion of the temporary files
	// instead, move them to a location specified by the given base name
	this->tmpFileNameBase = tmpfilenamebase;
}

void TSCLSignificanceEvaluator::addVariation(TQSampleFolder* sf, TString varname){
	// add a systematic variation to the evaluation
	this->reader->getSampleFolder()->addObject(sf,"::"+varname);
	this->variations.push_back(varname);
}

void TSCLSignificanceEvaluator::setInitialization(TClass* initClass){
	this->initialization = initClass;
}

bool TSCLSignificanceEvaluator::configure(const TString& configfilename, const TString& modelname){
	// configure the evaluator using the given stats configuration and model name
	// this function should be called only AFTER all other settings have been adjusted
	this->config->importFromTextFile(TQPathManager::getPathManager()->getTargetPath(configfilename).c_str());
	this->evaluationCommand.ReplaceAll("$(CONFIG)",configfilename);
	if(modelname.Length() > 0)
		this->modelName = modelname;
	TQFolder* createmodels = this->config->getFolder("CreateModels."+this->modelName);
	if(!createmodels){
		this->config->deleteAll();
		return false;
	}
	createmodels->getTagString("name",this->modelName);
	TQFolder* createworkspaces = this->config->getFolder("CreateWorkspaces");
	if(!createworkspaces){
		this->config->deleteAll();
		return false;
	}
	createworkspaces->getTagString("histogramsFile",tmpFileName);
	// remove variations folder for which there is no sample folder present
	TQIterator vitr(this->config->getListOfFolders("?/ModelConfiguration/Variations/?"),true);
	while (vitr.hasNext()){
	    TQFolder* var = (TQFolder*)vitr.readNext();
		var->setTagString(".Variation",var->getName());
	    TString varsfname = var->replaceInText(var->getTagStringDefault("~SampleFolder",var->getName()));
		if(TQStringUtils::removeLeading(varsfname,":") != 1) continue;
	    bool found = false;
	    for(size_t i=0; i<this->variations.size(); i++){
			if(this->variations[i] == varsfname){
				found = true;
				break;
			}
	    }
	    if(!found){ 
			config->removeObject(var->getPath());
		}
	}
	// remove systematics folder for variations that are not present
	TQIterator sysitr(this->config->getListOfFolders("?/ModelConfiguration/Systematics/?"),true);
	TQIterator sysvaritr(this->config->getListOfFolders("?/ModelConfiguration/Variations/?"),true);
	while(sysitr.hasNext()){
		TQFolder* f = dynamic_cast<TQFolder*>(sysitr.readNext());
		TString up = f->getTagStringDefault("Up","");
		TString down = f->getTagStringDefault("Down","");
		sysvaritr.reset();
		int found = 0;
		while(vitr.hasNext()){
			TQFolder* var = (TQFolder*)sysvaritr.readNext();
			if(var->getNameConst() == up || var->getNameConst() == down) found++;
			if(found > 1) break;
		}
		if(found < 2) this->config->removeObject(f->getPath());
	}

	// create a list of all locations and histogram names
	if(!this->baseFolder){
		std::cout << "ERROR in TSCLSignificanceEvaluator(" << this->GetName() << ")::configure(...) : base sample folder is NULL, terminating now!" << std::endl;
		return false;
	}
	if(this->variations.size() < 1){
		std::cout << "ERROR in TSCLSignificanceEvaluator(" << this->GetName() << ")::configure(...) : no variations scheduled - not even 'Nominal'!" << std::endl;
		return false;
	}
	TQSampleFolder* nomfolder = this->baseFolder->getSampleFolder(this->variations[0]);
	if(!nomfolder){
		std::cout << "ERROR in TSCLSignificanceEvaluator(" << this->GetName() << ")::configure(...) : base sample folder does not contain '" << this->variations[0] << "' subfolder, terminating now!" << std::endl;
		return false;
	}
	TQSampleDataReader* nomreader = new TQSampleDataReader(nomfolder);
	// loop over all samples listed in the configuration file
	TQFolder* parameters = this->config->getFolder("?/ModelConfiguration/Parameters/");
	if(parameters)
		parameters->setTagDouble("lumi",this->luminosityScale);
	TQIterator sitr(this->config->getListOfFolders("?/ModelConfiguration/Samples/?"),true);
	TQIterator chitr(this->config->getListOfFolders("?/ModelConfiguration/Channels/?"),true);
	while (sitr.hasNext()) { 
		chitr.reset();
		// obtain the sample path
		TQFolder * f = (TQFolder*)sitr.readNext();
		TString pathorig; 
		f->getTagString("Path", pathorig);
		while (chitr.hasNext()) { 
			// loop over all channels listed in the configuration file
			// fill the channel information into the sample path string
			TQTaggable tags((TQFolder*)(chitr.readNext()));
			TString hName = tags.getTagStringDefault("Histogram",this->discriminantVariable);
			if(this->initialization == TQGridScanner::Class()){
				tags.getTagString("CounterGrid",hName);
			}
			bool foundHistogram = false;
			for(size_t k=0; k<histNames.size(); k++){
				if(histNames[k] == hName)
					foundHistogram = true;
			}
			if(!foundHistogram)
				this->histNames.push_back(hName);
			TString pathreplaced = tags.replaceInText(pathorig);
			TQIterator fitr(nomreader->getListOfSampleFolders(pathreplaced),true);
			// expand the corresponding sample folder paths and loop over them
			while (fitr.hasNext()){
				TQSampleFolder* sf = (TQSampleFolder*)(fitr.readNext());
				if(!sf)
					continue;
				// get a full variant (starting with "Nominal")
				// and a bare variant (without the head)
				TString pathfull = sf->getPath();
				TString pathbare = TQFolder::getPathWithoutHead(pathfull);
				// check if the corresponding folder already exists
				TQSampleFolder* newsf = this->workingFolder->getSampleFolder(pathfull);
				// if not, we create it
				// and save the location
				if(!newsf){
					newsf = this->workingFolder->getSampleFolder(pathfull+"+");
					this->locations.push_back(pathbare);
				}
			}
		}
	}
	delete nomreader;
	for(size_t i=1; i<variations.size(); i++){
		for(size_t j=0; j<locations.size(); j++){
			this->workingFolder->getSampleFolder(TQFolder::concatPaths(this->variations[i],this->locations[j])+"+");
		}
	}
	return true;
}

void TSCLSignificanceEvaluator::printMode(){
	// print the current evaluation mode/settings
	std::cout << TQStringUtils::makeBoldWhite("== Configuration for Significance Evaluator ");
	std::cout << TQStringUtils::makeBoldRed(this->GetName());
	std::cout << TQStringUtils::makeBoldWhite(" ==") << std::endl;
	if(this->sampleFolderMode == EXPORT){
		std::cout << "export sample folder to " << this->sampleFolderFileName << " (compression:" << this->getTagIntegerDefault("tmpFileCompression",0) << ")" << std::endl;
	} else if(this->sampleFolderMode == IMPORT){
		std::cout << "import sample folder from " << this->sampleFolderFileName << std::endl;
	}
	if(this->internalEvaluation){
		std::cout << "evaluate internally" << std::endl;
	} else if(!this->evaluationCommand.IsNull()){
		std::cout << "evaluate externally using '" << this->evaluationCommand << "' (recycle: " << (this->recyclingAllowed ? "yes" : "no") << ")" << std::endl;
	} else {
		std::cout << "do not perform evaluation" << std::endl;
	}
	if(this->initialization){
		std::cout << "initialized with '" << this->initialization->GetName() << "'" << std::endl;
	} else {
		std::cout << "not (yet) initialized" << std::endl;
	}
	if(this->supportRegionSetHandling){
		std::cout << "using combined fit for region-splitting" << std::endl;
	} else {
		std::cout << "using separate fits for split regions" << std::endl;
	}
	if(this->significanceMode == EXPORT){
		std::cout << "export results to " << this->significanceFileName << std::endl;
	} else if(this->significanceMode == IMPORT){
		std::cout << "import results from " << this->significanceFileName << std::endl;
	}
	std::cout << TQStringUtils::makeBoldWhite("-------------------------------------------------") << std::endl;
}

double TSCLSignificanceEvaluator::evaluate(){
	// run the evaluation 
	TString sfname = this->sampleFolderFileName;
	sfname.ReplaceAll("$(ID)",this->fileIdentifier);
	TString sigfname = this->significanceFileName;
	sigfname.ReplaceAll("$(ID)",this->fileIdentifier);
	// clean up the tmp space
	TQSampleFolder* wf = NULL;
	gSystem->Exec("rm -f "+this->tmpFileName);
	bool canRecycle = this->recyclingAllowed && !this->internalEvaluation && TQUtils::fileExists(sfname) && TQUtils::fileExists(sigfname);	
	if(this->sampleFolderMode == IMPORT){
		// if we are in in IMPORT sampleFolder (i.e. IMPORT and INTERNAL) mode, we need to import it
		wf = TQSampleFolder::loadSampleFolder(sfname+":"+this->GetName());
		if(wf){
			std::cout << "ERROR in TSCLSignificanceEvaluator('" << this->GetName() << "')::evaluate() : error importing sample folder from " << sfname << ":" << this->GetName() << "!" << std::endl;
			return -1;
		}
	} else if(!canRecycle){
		wf = this->workingFolder;
		// check for a valid initialization
		if(!this->isPrepared()){
			if(!this->prepareNextRegionSet("")){
				std::cout << "ERROR in TSCLSignificanceEvaluator('" << this->GetName() << "')::evaluate : unable to prepare region!" << std::endl;
			}
		}
	}
	this->nPreparedRegionSets = 0;
	if(!canRecycle){
		bool cutoffsPassed = this->checkCutoffs();
		if(!cutoffsPassed){
			this->cleanWorkingFolder();
			return 0;
		}
	}
	if(this->sampleFolderMode == EXPORT && !(this->recyclingAllowed && TQUtils::fileExists(sfname))){
		// if we are in in EXPORT sampleFolder (i.e. EXPORT or EXTERNAL) mode, we need to export it
		if(!wf){
			std::cout << "ERROR in TSCLSignificanceEvaluator('" << this->GetName() << "')::evaluate() : cannot export, invalid working folder!" << std::endl;
			return -1;
		}
		TFile* f = new TFile(sfname,"RECREATE",sfname,this->engine->getHistogramsFileCompression());
		if(!f || !f->IsOpen()){
			std::cout << "ERROR in TSCLSignificanceEvaluator('" << this->GetName() << "')::evaluate() : cannot export, unable to open file " << sfname  << "!" << std::endl;
			return -1;
		}
		f->Add(wf->copy());
		f->Write();
		f->Close();
		delete f;
	}
	// we initialize the significance to zero
	double significance = 0;
	TQFolder* results = NULL;
	TString evalcmd(this->evaluationCommand);
	evalcmd.ReplaceAll("$(ID)",this->fileIdentifier);
	if(this->internalEvaluation){
		// if we are in INTERNAL mode, we need to
		// reset the engine
		if(!this->engine){
			std::cout << "ERROR in TSCLSignificanceEvaluator('" << this->GetName() << "')::evaluate() : cannot export, no statistics manager given! " << sfname  << "!" << std::endl;
			if(wf != this->workingFolder) delete wf;
			return -1;
		}
		this->engine->setDefaultSampleFolder(wf);
		this->engine->reset();
		// apply verbosity
		if(!this->verbose) TQLibrary::redirect("/dev/null");
		// run the statistics code
		this->engine->run(this->config);
		// restore verbosity
		if(!this->verbose) TQLibrary::restore();
		// obtain the results
		results = this->engine->getResults();
		// clean up the mess afterwards 
		// but only if the user didn't request to be left with the tmp file
		TString cmd;
		if(this->tmpFileNameBase.Length() > 0){
			cmd = "mv "+this->tmpFileName+" "+this->tmpFileNameBase+this->fileIdentifier+".root";
		} else {
			cmd = "rm -f "+this->tmpFileName;
		}
		gSystem->Exec(cmd);
	} else if(!evalcmd.IsNull()){
		if(canRecycle){
			this->info += " recycled results";
		} else {
			// if we are in EXTERNAL mode, we just need to execute whatsoever
			// external command was given to us
			TStopwatch s;
			s.Start();
			if(gSystem->Exec(evalcmd.Data()) > 0){
				std::cout << "ERROR in TSCLSignificanEvaluator::evaluate() : gSystem->Exec() reported an error on the following command:" << std::endl << evalcmd << std::endl;
				return -1;
			} 
			this->info += TString::Format(" (~%.2fs)",s.RealTime());
		}
	}
	if(this->significanceMode == IMPORT){
		// if we are in IMPORT significance (i.e. IMPORT or EXTERNAL) mode
		// we need to read in the results afterwards
		if(results) delete results;
		results = new TQFolder("results");
		results->importFromTextFile(TQPathManager::getPathManager()->getTargetPath(sigfname).c_str());
	} else if(this->significanceMode == EXPORT){
		// if we are in EXPORT significance (i.e. EXPORT and INTERNAL) mode
		// we need to write the results to a file
		if(!results){
			std::cout << "ERROR in TSCLSignificanceEvaluator::evaluate() : results pointer is NULL in EXPORT significance mode - this should never happen!" << this->modelName << "!" << std::endl;
			if(wf != this->workingFolder) delete wf;
			return -1;
		}
		if(canRecycle) results->exportToTextFile(TQPathManager::getPathManager()->getTargetPath(sigfname).c_str());
	}
	if(!results){
		if(wf != this->workingFolder) delete wf;
		return 0;
	}
	TQFolder* modsig = results->getFolder(TQFolder::concatPaths(this->modelName,"Significance"));
	if(modsig) modsig->getTagDouble("Z0_exp",significance);
	else {
		std::cout << "ERROR in TSCLSignificanceEvaluator::evaluate() : unable to obtain results from " << this->modelName << "!" << std::endl;
		if(!this->internalEvaluation && !evalcmd.IsNull())
			std::cout << "tried to evaluate externally using '" << evalcmd << "'..." << std::endl;
		results->print("rd");
		if(this->significanceMode == IMPORT) delete results;
		if(wf != this->workingFolder) delete wf;
		return -1;
	}
	// if everything went well, we can return
	if(this->significanceMode == IMPORT) delete results;
	if(wf != this->workingFolder) delete wf;
	return significance;
}

Int_t TSCLSignificanceEvaluator::cleanWorkingFolder(){
	Int_t nCleaned = 0;
	for(size_t ivar=0; ivar<this->variations.size(); ivar++){
		for(size_t iloc=0; iloc<this->locations.size(); iloc++){
			TQFolder* f =  this->workingFolder->getFolder(TQFolder::concatPaths(this->variations[ivar],this->locations[iloc],".histograms+"));
			if(f){
				f->deleteAll();
				nCleaned++;
			}
		}
	}
	return nCleaned;
}

bool TSCLSignificanceEvaluator::checkCutoffs(){
	if(!this->workingFolder) return false;
	TQSampleDataReader rd(this->workingFolder);
	TList* l = rd.getListOfHistogramNames();
	if(!l) return false;
	if(l->GetEntries() < 1) { delete l; return false; };
	l->SetOwner(true);
	TQIterator itr(l,true);
	for(size_t ivar=0; ivar<this->variations.size(); ivar++){
		double sum = 0;
		double sumError2 = 0;
		while(itr.hasNext()){
			TObject* name = itr.readNext();
			if(!name) continue;
			for(size_t iloc=0; iloc<this->locations.size(); iloc++){
				TH1* h = dynamic_cast<TH1*>(this->workingFolder->getObject(name->GetName(),TQFolder::concatPaths(this->variations[ivar],this->locations[iloc],".histograms")));
				Double_t err = 0;
				Double_t val = TQHistogramUtils::getIntegralAndError(h,err);
				sum += val;
				sumError2 += err*err;
				if(this->verbose) this->info += TString::Format(" %s:%s:%s = %.2f +/- %.2f     ", variations[ivar].Data(),locations[iloc].Data(),name->GetName(),val,err);
			}
		}
		itr.reset();
		double error = sqrt(sumError2);
		this->info += TString::Format(" %s: %.3f +/- %.3f ",variations[ivar].Data(),sum,error);
		if(sum < this->getTagDoubleDefault("cutoff",0)){
			this->info += " - evaluation skipped, absolute cutoff underrun";
			return false;
		}
		if(error/sum > this->getTagDoubleDefault("relErrCutoff",1.0)){
			this->info += " - evaluation skipped, rel. err. cutoff underrun";
			return false;
		}
	}
	return true;
}


Int_t TSCLSignificanceEvaluator::prepareFromGridScanner(const TString& /*suffix*/){
	// prepare the working folder from a TQGridScanner object
	// should be called automatically on evaluation 
	// if initialized with a TQGridScanner
	// CURRENTLY NOT IMPLEMENTED
	
	// if(!this->isPrepared()){
	// 	this->cleanWorkingFolder();
	// 	this->info = "";
	// }
	// Int_t nHistograms = 0;
	// for(size_t ivar=0; ivar<this->variations.size(); ivar++){
	// 	for(size_t iloc=0; iloc<this->locations.size(); iloc++){
	// 		TQFolder* f = this->workingFolder->getFolder(TQFolder::concatPaths(this->variations[ivar],this->locations[iloc],".histograms+"));
	// 		for(size_t ihist=0; ihist<histNames.size(); ihist++){
	// 			if(!suffix.IsNull() && histNames[ihist].EndsWith(suffix))
	// 				continue;
	// 			size_t index = ihist + (iloc * histNames.size()) + (ivar * histNames.size() * locations.size());
	// 			TQCounterGrid* g = this->scanner->getGrid(index);
	// 			if(!g){
  //                                 std::cerr << "ERROR in TSCLSignificanceEvaluator::evaluateGridScanner() : unable to retrieve grid " << index << " from TQGridScanner '" << scanner->GetName() << "', corresponding to the combination " << TQFolder::concatPaths(variations[ivar],locations[iloc]) << " : " << histNames[ihist] << "!" << std::endl;
  //                                 return -1;
	// 			}
	// 			TH1* h = g->getTH1F(this->discriminantVariable);
	// 			if(!h){
  //                                 std::cerr << "ERROR in TSCLSignificanceEvaluator::evaluateGridScanner() : unable to retrieve histogram '" << this->discriminantVariable << "' from TQGridScanner " << scanner->GetName() << " for location '" << this->locations[iloc] << "'!" << std::endl;
  //                                 return -1;
	// 			}
	// 			f->addObject(h,"::"+histNames[ihist]+suffix);
	// 			nHistograms++;
	// 		}
	// 	}
	// }
	// if(nHistograms > 0) nPreparedRegionSets++;
	// return nHistograms;
	return 0;
}

TQSampleFolder* TSCLSignificanceEvaluator::getWorkingFolder(TString path){
	// return a pointer to the current working folder
	if(!this->workingFolder)
		return NULL;
	if(path.IsNull())
		return this->workingFolder;
	return this->workingFolder->getSampleFolder(path);
}

TQSampleFolder* TSCLSignificanceEvaluator::getBaseFolder(TString path){
	// return a pointer to the current base folder
	if(!this->baseFolder)
		return NULL;
	if(path.IsNull())
		return this->baseFolder;
	return this->baseFolder->getSampleFolder(path);
}

TQFolder* TSCLSignificanceEvaluator::getConfigFolder(TString path){
	// return a pointer to the configuration folder
	if(!this->config)
		return NULL;
	if(path.IsNull())
		return this->config;
	return this->config->getFolder(path);
}

void TSCLSignificanceEvaluator::printPaths(){
	// print the paths of all registered histograms
	std::cout << this->GetName() << " INFO: the following histograms have been booked" << std::endl;
	for(size_t i=0; i<variations.size(); i++){
		for(size_t j=0; j<locations.size(); j++){
			for(size_t k=0; k<histNames.size(); k++){
				std::cout << TQFolder::concatPaths(variations[i],locations[j]) << "\t" << histNames[k] << std::endl;
			}
		}
	}
}

bool TSCLSignificanceEvaluator::initialize(const TString& /*importfilename*/, const TString& /*exportfilename*/){
	// initialize the evaluator for import/export only
	// CURRENTLY NOT IMPLEMENTED
	return true;
}

bool TSCLSignificanceEvaluator::initialize(TQGridScanner* /*scanner*/){
	// initialize the evaluator for use with a TQGridScanner
  // if(!scanner){
  //   ERRORclass("no TQGridScanner appointed to initialization!");
  //   return false;
  // }
  // if(!reader){
  //   ERRORclass("no TQSampleDataReader available!");
  //   return false;
  // }
  // if(m_regions.size() < 1){
  //   ERRORclass("No regions set!");
  //   return false;
  // }
  // this->initialization = scanner->Class();
  // this->scanner = scanner;

  //       for(size_t j=0; j<this->variations.size(); j++){
  //       	for(size_t i=0; i<this->locations.size(); i++){
  //       		TString path = TQFolder::concatPaths(variations[j],locations[i]);
  //       		for(size_t k=0; k<this->histNames.size(); k++){
  //       			TString histname = TQFolder::concatPaths(this->histNames[k],scanner->GetName());
  //       			TQCounterGrid* g = reader->getCounterGrid(path,histname);
  //       			if(!g){
  //       				std::cout << "ERROR in TSCLSignificanceEvaluator::initialize() : unable to retrieve TQCounterGrid " << histname << " from " << path << " in " << variations[i] << "!" << std::endl;
  //       				return false;
  //       			}
  //       			scanner->addGrid(g);
  //       			g->scale(this->luminosityScale);
  //       			g->compactify();
  //       		}
  //       	}
  //       	if(reader->getSampleFolder()->isLazy()){
  //       		TQFolder* f = reader->getSampleFolder()->getFolder(variations[j]);
  //       		if(f){
  //       			f->setExportName(f->getName());
  //       			reader->getSampleFolder()->collapse();
  //       		}
  //       	}
  //       }
  //       this->engine = new TSStatisticsManager();
  //       this->engine->setHistogramsFileCompression(this->getTagIntegerDefault("tmpFileCompression",0));
  //       return true;
  return true;
}


bool TSCLSignificanceEvaluator::hasNativeRegionSetHandling(){
	return this->supportRegionSetHandling;
}

bool TSCLSignificanceEvaluator::prepareNextRegionSet(const TString& suffix){
	if(0 == TQStringUtils::compare(this->initialization->GetName(),TQGridScanner::Class()->GetName())){
		if(this->prepareFromGridScanner(suffix) < 1){
			std::cout << "ERROR in TSCLSignificanceEvaluator('" << this->GetName() << "')::prepareNextRegionSet() : error preparing working folder from GridScanner!" << std::endl;
			return false;
		} 
		return true;
	} 
	std::cout<< "WARNING: TSCLSignificanceEvaluator('" << this->GetName() << "')::prepareNextRegionSet('" << suffix << "') : reached end of function without having found a matching initialization scheme. The initialization encountered was " << (this->initialization ? this->initialization->GetName() : "NULL") << std::endl;
	return false;
}

bool TSCLSignificanceEvaluator::isPrepared(){
	if(this->nPreparedRegionSets > 0){
		return true;
	} 
	return false;
}

//-----------------------------------------------------------
// Dummy functions that shouldn't be used
double TSCLSignificanceEvaluator::getSignificance(size_t /*iregion*/) {
  return 0;
}

double TSCLSignificanceEvaluator::getSignificance2(size_t /*iregion*/) {
  return 0;
}

std::vector<double> TSCLSignificanceEvaluator::evaluateMultiple() {
  return {};
}
//-----------------------------------------------------------
