# Development

## Adding new samples

1. Add the MC16a DxAOD container of the new samples to the `bbll/config/samples/grid/SUSY2.txt` file.
   This is needed to generate the cross-sections file.
2. For manual control over the cross-section information edit either `bbll/config/samples/grid/kfactors.txt` (if you only want to change the k-factor) or `bbll/config/samples/grid/overwrite_XS_13TeV.txt` (if you want to set all cross-section information by yourself).
3. Update the cross-sections file with
   ```
   lsetup pyami
   voms-proxy-init -voms atlas -valid 96:00   # if you don't already have an active voms proxy
   ../tools/getAMICrossSections.py bbll/config/master/grid.cfg
   ```
4. Add the CxAOD containers to the corresponding sample lists in `bbll/config/samples/grid/`.
   E.g. for the V10 production you will need to edit `bbll/config/samples/grid/V10-mc16a.txt`, `bbll/config/samples/grid/V10-mc16d.txt`, and `bbll/config/samples/grid/V10-mc16e.txt`
   You can retrieve the name of the CxAOD containers e.g. with [pandamon](https://github.com/dguest/pandamonium):
   ```
   pandamon "user.<CERNUSER>.*BBLL_<TAG>*" -s OUT | grep CxAOD.root
   ```
   The r-tag indicates the campaign of the container:
   - `r9364`: MC16a
   - `r10201`: MC16d
   - `r10724`: MC16e
5. Replicate the new CxAOD samples to all grid storage endpoints of the sites you want to use them on. Currently CxAODs are stored on `CERN-PROD_PHYS-HDBS` and `UNI-FREIBURG_LOCALGROUPDISK`.
6. Update the input file lists with
   ```
   lsetup rucio
   voms-proxy-init -voms atlas -valid 96:00   # if you don't already have an active voms proxy
   ../tools/makeFileList.py bbll/config/master/grid_freiburg.cfg 2>&1 | tee file_list_freiburg.log
   ../tools/makeFileList.py bbll/config/master/grid_lxplus.cfg 2>&1 | tee file_list_lxplus.log
   # check that there are the same number of input files in the input lists for different clusters
   wc -l bbll/config/samples/inputFileLists/V10-*  # here as an example with the V10 production
   ```
7. Update the sample map at `bbll/config/samples/maps/default.map` and add the samples to the corresponding sample folder path
8. (Re)run the prepare and initialize steps
   ```
   prepare.py bbll/config/master/prepare.cfg
   initialize.py bbll/config/master/initialize_<cluster>.cfg
   ```
9. The samples are now integrated into CAF and you can run over them. Update the remaining config files (like job lists, ntuple dumping, plotting, etc) according to what you plan to do with these samples.

## Updating to a new CxAOD production

TODO
