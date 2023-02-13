Overview
==========
The Common Analysis Framework (CAF) is the back-end to multiple ATLAS analyses. Most analyses use it to loop over events with calibrated particles (xAODs or ntuples, but other inputs can easily be added), apply a set of cuts and produce cutflows and histograms. This functionality (and much more) can be found in `QFramework`. The `SFramework` provides a convenient interface to `RooFit` via the `C++` interface of `HistFactory`.

Contributing
==========

General
----------
We - the current developer team - try hard to keep the code very general so that a wide range of analyses can benefit. This being said, maintaining this code and developing new features requires some work and we have more ideas for improvements than we have time to implement them. If you are interested in some of them, take a look at our [issues](https://gitlab.cern.ch/atlas-caf/CAFCore/issues), where we keep track of both our good ideas (which sometimes sit around for quite a while) and urgent problems that need immediate attention.

If you want to contribute to CAF (be it because you need a new feature in your analysis, you want to familiarize yourself with the code or you just want to contribute to a cool framework), feel free to get in touch! For all questions, we maintain the mailing list qframework-users@cern.ch.

Working with git
----------
We use `git` to keep a record of earlier code versions. `git` also helps with simultaneously collaborating on the same code. If you want to make changes to the code, the standard procedure is to
 *  create an issue [here](https://gitlab.cern.ch/atlas-caf/CAFCore/issues) (this step is optional, but it can be helpful to specify the general purpose of your changes)
 *  create a merge request (MR) [here](https://gitlab.cern.ch/atlas-caf/CAFCore/merge_requests) or create it directly from your issue
 *  check out the branch that's created at the same time as the MR
 *  work on it (general advice: commit early, commit often)
 *  push the changes
 *  remove the `WIP` label from your MR and tag someone to review your changes
 *  ask someone in the developer team to merge your changes into the master branch
 This procedure is also summarized in the beginning of the most recent [CAF tutorial](https://indico.cern.ch/event/771763/).

Specifics in CAF
----------
The main branch in our repository is the `master` branch. It is not allowed to push directly to this branch and it can only be changed with a merge request on gitlab. We also have a `cosmetic` branch. It can be used for minor issues (documentation, typos and trivial code changes), for which a new branch and MR would be too much effort. This branch is merged into master frequently. And of course, there are many feature branches for specific issues. Don't hesitate to create your own branch!

To visualize the severity and type of an issue, we use gitlab labels. A description of the labels can be found [here](https://gitlab.cern.ch/atlas-caf/CAFCore/labels).

We try to help inexpericend users by tagging certain versions of CAF. These tags have the format `YY.MM.V`, where `YY` indicates the year, `MM` the month and `V` the version (starting with 0 and counting up in case of updates), while the branch `YY.MM.X` always points to the latest version. The purpose of tagged versions is to provide versions of the code which have been tested and seem to work reliably.

Every MR should have a description and this description should concisely summarize new features. MR descriptions in CAF have two sections: **Release notes** and **Details**. The first one should tell the inexperienced user which new features to expect. The **Details** section can be a more elaborate version, but is also meant to contain all the nasty information about the implementation that you want to give to experienced CAF people. These two MR sections are separated, because we auto-generate a [list of MRs](https://gitlab.cern.ch/atlas-caf/CAFCore/tags) in an effort to document the changes between CAFCore tags. This auto-generated list will contain the text in **Release notes**, but not the text in **Details**.
