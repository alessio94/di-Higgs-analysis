import ROOT
import QFramework
import CommonAnalysisHelpers


def addAlgorithms(visitor,config):

    unfoldingConfig = QFramework.TQFolder()
    unfoldingConfig.importTagsWithoutPrefix(config, "unfolding.")

#    unfoldingConfig.printTags()
    unfoldingCuts = CommonAnalysisHelpers.analyze.loadCuts(unfoldingConfig)

    QFramework.TQHistoMakerAnalysisJob.setDefaultAuthor("unfolding")
    CommonAnalysisHelpers.analyze.bookAnalysisJobs(unfoldingConfig,unfoldingCuts)
    executionAlgorithm = QFramework.TQAnalysisAlgorithm("AnalysisAlgorithm", unfoldingCuts)
    unfoldingCuts.printCuts()

    executionAlgorithm.setBaseCut(unfoldingCuts)

    visitor.addAlgorithm(executionAlgorithm)
    return True
