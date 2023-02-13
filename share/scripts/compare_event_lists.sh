#!/bin/bash
date
./scripts/compare_event_lists.py results/eventlists/HH results/eventlists/H 2>&1 | tee eventlists.log
date
