#!/bin/bash

echo "Generating sample lists for emtopo"
pandamon "user.brottler.*.CxAOD_SUSY2.*.BBLL_190909_EMTOPO*" -d 30 -s | grep -v hist | grep mc16 | grep r9364 | sort > bbll/config/samples/grid/emtopo-mc16a.txt
pandamon "user.brottler.*.CxAOD_SUSY2.*.BBLL_190909_EMTOPO*" -d 30 -s | grep -v hist | grep mc16 | grep r10201 | sort > bbll/config/samples/grid/emtopo-mc16d.txt
pandamon "user.brottler.*.CxAOD_SUSY2.*.BBLL_190909_EMTOPO*" -d 30 -s | grep -v hist | grep mc16 | grep r10724 | sort > bbll/config/samples/grid/emtopo-mc16e.txt
pandamon "user.brottler.*.CxAOD_SUSY2.*.BBLL_190909_EMTOPO*" -d 30 -s | grep -v hist | grep data15 | sort > bbll/config/samples/grid/emtopo-data1516.txt
pandamon "user.brottler.*.CxAOD_SUSY2.*.BBLL_190909_EMTOPO*" -d 30 -s | grep -v hist | grep data16 | sort >> bbll/config/samples/grid/emtopo-data1516.txt
pandamon "user.brottler.*.CxAOD_SUSY2.*.BBLL_190909_EMTOPO*" -d 30 -s | grep -v hist | grep data17 | sort > bbll/config/samples/grid/emtopo-data17.txt
pandamon "user.brottler.*.CxAOD_SUSY2.*.BBLL_190909_EMTOPO*" -d 30 -s | grep -v hist | grep data18 | sort > bbll/config/samples/grid/emtopo-data18.txt

echo "Generating sample lists for pflow"
cat bbll/config/samples/grid/emtopo-mc16a.txt | sed "s/BBLL_190909_EMTOPO/BBLL_V02.1/" > bbll/config/samples/grid/pflow-mc16a.txt
cat bbll/config/samples/grid/emtopo-mc16d.txt | sed "s/BBLL_190909_EMTOPO/BBLL_V02.1/" > bbll/config/samples/grid/pflow-mc16d.txt
cat bbll/config/samples/grid/emtopo-mc16e.txt | sed "s/BBLL_190909_EMTOPO/BBLL_V02.1/" > bbll/config/samples/grid/pflow-mc16e.txt
cat bbll/config/samples/grid/emtopo-data1516.txt | sed "s/BBLL_190909_EMTOPO/BBLL_V02.1/" > bbll/config/samples/grid/pflow-data1516.txt
cat bbll/config/samples/grid/emtopo-data17.txt | sed "s/BBLL_190909_EMTOPO/BBLL_V02.1/" > bbll/config/samples/grid/pflow-data17.txt
cat bbll/config/samples/grid/emtopo-data18.txt | sed "s/BBLL_190909_EMTOPO/BBLL_V02.1/" > bbll/config/samples/grid/pflow-data18.txt
