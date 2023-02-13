from ROOT import RooRealVar, RooBukinPdf, RooDataHist, RooArgList, RooFit, RooAbsReal, RooAbsData
from ROOT import kFALSE, kTRUE


def bukin_fit(hist):
    """
    Fit the Bukin distribution to a histogram.

    The Bukin distribution is described here: https://arxiv.org/abs/0711.4449

    Arguments:
        hist (TH1*): Histogram which is used in the fit.

    Returns:
        (RooPlot, float, float, float, float):
            - RooPlot: RooFit frame where the fit function is drawn
            - float: mean of the Bukin distribution
            - float: error on the mean
            - float: width of the Bukin distribution
            - float: error on the width
    """
    nbins = hist.GetNbinsX()
    xmin = hist.GetBinLowEdge(1)
    xmax = hist.GetBinLowEdge(nbins) + hist.GetBinWidth(nbins)
    x = RooRealVar("x", "x", xmin, xmax)

    # create original data hist for later normalization
    orig_data = RooDataHist("orig_data", "orig_data", RooArgList(x), hist)

    # normalize histogram to an integral of 1
    hist = hist.Clone(hist.GetName() + "_fit")
    hist.Scale(1.0 / hist.Integral("width"))

    # create RooFit variables for bukin function
    # see https://root.cern.ch/doc/master/classRooBukinPdf.html for the variables we need
    # x: the variable
    # mean: peak position
    # sigma: peak width
    # xi: peak asymmetry, use values around 0, min/max is -1/1
    # rho1: left tail, use slightly negative starting values, min/max is -1/0
    # rho2: right tail, use slightly positive starting values, min/max is 0/1
    # x = RooRealVar("x", "x", xmin, xmax)
    start_mean = hist.GetMean()
    start_sigma = hist.GetStdDev()
    mean = RooRealVar("mean", "mean", start_mean, start_mean - start_sigma, start_mean + start_sigma)
    sigma = RooRealVar("sigma", "sigma", start_sigma, start_sigma / 3, start_sigma * 3)
    xi = RooRealVar("xi", "xi", 0, -1, 1)
    rho1 = RooRealVar("rho1", "rho1", -1e-2, -1, 0)
    rho2 = RooRealVar("rho2", "rho2", 1e-2, 0, 1)

    bukin = RooBukinPdf("bukin", "bukin", x, mean, sigma, xi, rho1, rho2)

    data = RooDataHist("data", "data", RooArgList(x), hist)

    result = bukin.fitTo(
        data,
        RooFit.Save(),
        RooFit.SumW2Error(kFALSE),
        RooFit.Minos(kTRUE),
        RooFit.Verbose(kFALSE),
        RooFit.PrintLevel(-1),
        RooFit.Warnings(kFALSE),
        RooFit.PrintEvalErrors(-1),
    )
    result.Print("v")

    frame = x.frame()
    # first plot originial data hist, otherwise normalization will be random if combining with normal TH1*
    orig_data.plotOn(
        frame,
        RooFit.DataError(RooAbsData.None),
        RooFit.MarkerColor(hist.GetMarkerColor()),
        RooFit.MarkerSize(hist.GetMarkerSize()),
        RooFit.MarkerStyle(hist.GetMarkerStyle()),
        RooFit.LineColor(hist.GetLineColor()),
        RooFit.LineWidth(hist.GetLineWidth()),
        RooFit.LineStyle(hist.GetLineStyle()),
    )
    bukin.plotOn(
        frame,
        RooFit.LineColor(hist.GetLineColor()),
        RooFit.LineStyle(hist.GetLineStyle()),
        RooFit.LineWidth(hist.GetLineWidth() + 1),
    )

    return frame, mean.getVal(), mean.getError(), sigma.getVal(), sigma.getError()
