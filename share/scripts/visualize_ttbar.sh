#!/bin/bash

for version in dilepton herwig madgraph hdamp; do
  visualize.py bbll/config/master/visualize-ttbar-${version}.cfg &
done
