#include "CAFbbll/PNN_KL.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQTaggable.h"

#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/LightweightNeuralNetwork.hh"
#include "lwtnn/NNLayerConfig.hh"
#include "lwtnn/parse_json.hh"

#include <fstream>
#include <iostream>
#include <iterator>

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// you can perform debug printouts with statements like this
// DEBUG("error number %d occurred",someInteger);

// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

ClassImp(PNN_KL)

    // initialize static member variables
    std::map<std::string, lwt::LightweightNeuralNetwork*> PNN_KL::fNetworkCache;
std::map<std::string, lwt::LightweightGraph*> PNN_KL::fGraphCache;
std::map<std::string, std::string> PNN_KL::fInputNodeNameCache;
std::map<std::string, std::map<std::string, TString>> PNN_KL::fObservableExpressionCache;

//______________________________________________________________________________________________

PNN_KL::PNN_KL() {
    // default constructor
    DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

PNN_KL::~PNN_KL() {
    // default destructor
    DEBUGclass("destructor called");
}

//______________________________________________________________________________________________

TObjArray* PNN_KL::getBranchNames() const {
    // retrieve the list of branch names for this observable
    // ownership of the list belongs to the caller of the function
    DEBUGclass("retrieving branch names");
    TObjArray* bnames = new TObjArray();
    for (const std::pair<std::string, TQObservable*>& kv : fObservables) {
        TQObservable* obs = kv.second;
        if (!obs)
            throw std::runtime_error(
                TString::Format("encountered invalid sub-observable corresponding key '%s'!", kv.first.c_str()).Data());
        DEBUGclass(
            "retrieving branches of observable '%s' of class '%s'", obs->getExpression().Data(), obs->ClassName());
        TCollection* c = obs->getBranchNames();
        if (c) {
            c->SetOwner(false);
            bnames->AddAll(c);
            delete c;
        }
    }
    DEBUGclass("returning");
    return bnames;
}

//______________________________________________________________________________________________

Long64_t PNN_KL::getCurrentEntry() const {
    DEBUGclass("get current entry");
    // retrieve the current entry from the tree
    if (fObservables.size() == 0)
        return -1;
    for (const std::pair<std::string, TQObservable*> kv : fObservables) {
        if (kv.second->getCurrentEntry() >= 0)
            return kv.second->getCurrentEntry();
    }
    return -1;
}

//______________________________________________________________________________________________

int PNN_KL::getNevaluations() const { return fOutputNames.size(); }

//______________________________________________________________________________________________

void PNN_KL::evalSequential(std::map<std::string, double>& output) const {
    DEBUGclass("eval sequential");
    std::map<std::string, double> features;
    for (const std::pair<std::string, TQObservable*>& input : fObservables) {
        features[input.first] = input.second->getValue();
    }
    output = fNetwork->compute(features);
    return;
}

//______________________________________________________________________________________________

void PNN_KL::evalFunctional(std::map<std::string, double>& output) const {
    DEBUGclass("eval functional");
    std::map<std::string, double> features;
    for (const std::pair<std::string, TQObservable*>& input : fObservables) {
        features[input.first] = input.second->getValue();
    }
    std::map<std::string, std::map<std::string, double>> wrappedFeatures;
    wrappedFeatures[fInputNodeName] = features;
    output = fGraph->compute(wrappedFeatures);
    return;
}

//______________________________________________________________________________________________

double PNN_KL::getValue() const {
    DEBUGclass("get value");
    // forward to the implementation capable of handling multiple values.
    if (getObservableType() != TQObservable::ObservableType::vector)
        return getValueAt(0);
    else
        throw std::runtime_error(TString::Format(
            "Caught attempt to perform scalar evaluation on vector valued instance of PNN_KL with expression '%s'",
            getExpression().Data())
                                     .Data());
}

double PNN_KL::getValueAt(int i) const {
    // value retrieval function, called on every event for every cut and histogram
    DEBUGclass("get value at %d", i);

    const Long64_t entry = getCurrentEntry();
    if (entry != fCachedEntry) { // new event, calculate NN once
        fCachedValue.clear();
        if (fSequential) {
            // evaluate a "Sequential API" model
            evalSequential(fCachedValue);
        } else {
            // evaluate a "Functional API" model
            evalFunctional(fCachedValue);
        }
        fCachedEntry = entry;
    }

    const double retval = fCachedValue[fOutputNames[i]];
    return retval;
}

//______________________________________________________________________________________________

bool PNN_KL::initializeSequential(const std::string& fileName) {
    DEBUGclass("initializing sequential");
    std::string cache_name(fCacheName.Data());
    DEBUGclass("Cache name = '%s'", cache_name.c_str());
    if (fNetworkCache.find(cache_name) == fNetworkCache.end()) { // key not in map
        DEBUGclass("Creating new instance of neural network");
        // initialization in case the Sequential API style json is used
        std::ifstream netFile(fileName);

        lwt::JSONConfig netDef = lwt::parse_json(netFile);
        for (lwt::Input& input : netDef.inputs) { // explicitly non-const, as we'll possibly shorten the input keys!
            // parse input names into observable names and keys
            // expected syntax:
            // key=observableExpression
            // if no observableExpression is found, use the key itself
            TString key, obsExpr;
            obsExpr = TString(input.name);
            // move part before "=" to after
            TQStringUtils::readUpTo(obsExpr, key, "=", "", "\"\"''");
            TQStringUtils::removeLeading(obsExpr, "=");
            if (obsExpr.Length() < 1) {
                obsExpr = key; // if no dedicated obsExpr is specified, fall back to key itself
            }
            // write back key to input definition, i.e., removing the obsExpr to make keys shorter
            input.name = key; // implicit conversion TString->std::string
            // we cannot cache the initialized observables because they might depend on
            // sample folder specific tags, so we just store the observable expression
            // without replacing the tags and initialize the observables for each instance later
            fObservableExpressionCache[cache_name][key.Data()] = obsExpr;
        }
        // after inputs were pre-parsed, construct the net from them
        fNetwork = new lwt::LightweightNeuralNetwork(netDef.inputs, netDef.layers, netDef.outputs);
        fNetworkCache.insert({ cache_name, fNetwork });
    } else {
        DEBUGclass("Using existing instance of neural network");
        fNetwork = fNetworkCache.at(cache_name);
    }

    return initializeObservables(cache_name);
}

//______________________________________________________________________________________________

bool PNN_KL::initializeObservables(const std::string& cache_name) {
    DEBUGclass("initializing observables");
    std::shared_ptr<TQTaggable> aliases = TQTaggable::getGlobalTaggable("aliases");
    aliases->setTagDouble("KL", fKappaLambdaValue);
    for (auto const& obsData : fObservableExpressionCache[cache_name]) {
        std::string key = obsData.first;
        TString obsExpr = obsData.second;
        obsExpr = aliases->replaceInTextRecursive(obsExpr);

        TQObservable* inputObs = TQObservable::getObservable(obsExpr, fSample);
        if (!inputObs->initialize(fSample))
            return false;
        fObservables[key] = inputObs;
    }

    return true;
}

//______________________________________________________________________________________________

bool PNN_KL::initializeFunctional(const std::string& fileName) {
    DEBUGclass("initializing functional");
    std::string cache_name(fCacheName.Data());
    DEBUGclass("Graph cache name = '%s'", cache_name.c_str());
    if (fGraphCache.find(cache_name) == fGraphCache.end()) { // key not in map
        DEBUGclass("Creating new instance of graph");

        // initialization in case the Sequential API style json is used
        std::ifstream netFile(fileName);

        lwt::GraphConfig netDef = lwt::parse_json_graph(netFile);
        std::shared_ptr<TQTaggable> aliases = TQTaggable::getGlobalTaggable("aliases");
        if (netDef.inputs.size() != 1) {
            throw std::runtime_error(
                TString::Format("Failed to load functional lwtnn definition from file '%s': unsupported number of "
                                "input nodes (must be 1 node withany number of variables)",
                    fileName.c_str())
                    .Data());
        }

        fInputNodeName = netDef.inputs[0].name;
        fInputNodeNameCache.insert({ cache_name, fInputNodeName });
        for (lwt::Input& input :
            netDef.inputs[0].variables) { // explicitly non-const, as we'll possibly shorten the input keys!
            // parse input names into observable names and keys
            // expected syntax:
            // key=observableExpression
            // if no observableExpression is found, use the key itself
            TString key, obsExpr;
            obsExpr = TString(input.name);
            // move part before "=" to after
            TQStringUtils::readUpTo(obsExpr, key, "=", "", "\"\"''");
            TQStringUtils::removeLeading(obsExpr, "=");
            if (obsExpr.Length() < 1) {
                obsExpr = key; // if no dedicated obsExpr is specified, fall back to key itself
            }
            // write back key to input definition, i.e., removing the obsExpr to make keys shorter
            input.name = key; // implicit conversion TString->std::string
            // we cannot cache the initialized observables because they might depend on
            // sample folder specific tags, so we just store the observable expression
            // without replacing the tags and initialize the observables for each instance later
            fObservableExpressionCache[cache_name][key.Data()] = obsExpr;
        }

        // after inputs were pre-parsed, construct the net from them
        fGraph = new lwt::LightweightGraph(netDef);
        fGraphCache.insert({ cache_name, fGraph });
    } else {
        DEBUGclass("Using existing instance of graph");
        fInputNodeName = fInputNodeNameCache.at(cache_name);
        fGraph = fGraphCache.at(cache_name);
    }

    return initializeObservables(cache_name);
}

//______________________________________________________________________________________________

bool PNN_KL::initializeSelf() {
    DEBUGclass("initializing");
    // get your saved JSON file as an std::istream object
    std::string resolvedFileName =
        TQPathManager::getPathManager()->findFileFromEnvVar(fNetworkFileName, "CAFANALYSISSHARE");
    if (fSequential) {
        return initializeSequential(resolvedFileName);
    }
    return initializeFunctional(resolvedFileName);
}

//______________________________________________________________________________________________

bool PNN_KL::finalizeSelf() {
    // finalize this observable on a sample/tree
    DEBUGclass("finalizing");
    // here, you should undo anything that you did while initializing
    /* example block for TTreeFormula method:
    delete fFormula;
    fFormula = NULL;
    */
    for (const std::pair<std::string, TQObservable*>& input : fObservables) {
        if (!input.second->finalize())
            return false;
    }
    std::string network_cache_name(fCacheName.Data());
    if (fNetworkCache.find(network_cache_name) != fNetworkCache.end()) { // key in map
        fNetworkCache.erase(network_cache_name);
        delete fNetwork;
        fNetwork = nullptr;
    }

    if (fGraphCache.find(network_cache_name) != fGraphCache.end()) { // key in map
        fGraphCache.erase(network_cache_name);
        delete fGraph;
        fGraph = nullptr;
    }

    if (fInputNodeNameCache.find(network_cache_name) != fInputNodeNameCache.end()) { // key in map
        fInputNodeNameCache.erase(network_cache_name);
        fInputNodeName = "";
    }

    fObservables.clear();

    return true;
}

//______________________________________________________________________________________________

void PNN_KL::setOutputs(const std::vector<std::string> outputNames) {
    // Set a list of output names from the network to expose
    fOutputNames = outputNames;
}

void PNN_KL::setOutputs(const std::string& outputNames_) { setOutputs(TString(outputNames_)); }

void PNN_KL::setOutputs(const TString& outputNames_) {
    DEBUGclass("setting output names '%s'", outputNames_.Data());
    // Set a list of output names from the network to expose
    // This version expects a comma separated list of names,
    // optionally enclosed by {} (resembling a C++ initializer list)
    TString outputNames = outputNames_;
    TQStringUtils::trim(outputNames); // remove white spaces
    TString block = "";
    TQStringUtils::readBlock(outputNames, block,
        "()"); // in principle we'd like to use {} here, but TQObservable already replaces {} brackets with () ones...
    if (block.Length() < 1) { // block wasn't enclosed, so let's use the original part directly
        block = outputNames;
    }
    std::vector<TString> asTStrings = TQStringUtils::tokenizeVector(block, ",", true /*, "()[]{}"*/);
    fOutputNames.clear();
    for (const TString& tstr : asTStrings) {
        fOutputNames.push_back(tstr.Data());
    }
}

//______________________________________________________________________________________________

void PNN_KL::parseArguments(TString arguments) {
    DEBUGclass("parsing arguments '%s'", arguments.Data());
    // input is arguments string without surrounding parentheses
    TString filename = "";
    TString output_names = "";
    TString kl_value = "";
    TQStringUtils::readUpTo(arguments, filename, ",");
    TQStringUtils::removeLeading(arguments, ",");
    TQStringUtils::readBlock(arguments, output_names, "()");
    output_names = TQStringUtils::trim(output_names);
    TQStringUtils::removeLeading(arguments, ",");
    kl_value = TQStringUtils::trim(arguments);
    if (kl_value.Length() < 1) {
        throw std::runtime_error(
            TString::Format("Unable to parse expression for PNN_KL Observable: too few arguments (did you forget to "
                            "specify the output names to expose or the kl value?). Arguments were: '%s,%s,%s'",
                filename.Data(), output_names.Data(), kl_value.Data())
                .Data());
    }
    fNetworkFileName = filename;
    fKappaLambdaValue = kl_value.Atof();
    setOutputs(output_names);
    DEBUGclass("parsed arguments: filename = '%s', output_names = '%s', kl_value = '%s'", filename.Data(),
        output_names.Data(), kl_value.Data());
    // don't use kl_value for name, as we want to use the same cache for different kl values
    fCacheName = "PNN_KL(" + filename + ", " + output_names + ")";
}

//______________________________________________________________________________________________

PNN_KL::PNN_KL(const TString& arguments)
    : TQObservable(arguments) {
    // constructor with name argument
    DEBUGclass("constructor called with '%s'", arguments.Data());
    parseArguments(arguments);
}

DEFINE_OBSERVABLE_FACTORY(PNN_KL, TString expression) {
    // try to create an instance of this observable from the given expression
    TQStringUtils::trim(expression);
    if (TQStringUtils::removeLeadingText(expression, "PNN_KL")) {
        bool sequential = false;
        if (TQStringUtils::removeLeadingText(expression, "_Seq")) {
            // create instance configured for sequential API style inputs (using LightWeightNeuralNetwork)
            // else: create instance configured for functional API style inputs (using LightWeightGraph)
            sequential = true;
        }

        TQStringUtils::trim(expression);
        TString arguments = "";
        TQStringUtils::readBlock(expression, arguments);
        PNN_KL* obs = new PNN_KL(arguments);
        obs->setSequential(sequential);
        return obs;
    }
    return nullptr;
}
