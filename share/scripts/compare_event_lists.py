#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import csv
import logging
import os
import sys


class Event(object):
    """The Event class stores properties of each event."""

    def __init__(self, run_number, event_number):
        self.run_number = run_number
        self.event_number = event_number
        self.hh_exists = False
        self.hh_pt0 = 0
        self.hh_pt1 = 0
        self.hh_trigger = 0
        self.hh_nelec = 0
        self.hh_nmu = 0
        self.hh_exists = False
        self.h_pt0 = 0
        self.h_pt1 = 0
        self.h_trigger = 0
        self.h_nelec = 0
        self.h_nmu = 0
        self.h_exists = False

    def set_hh(self, pt0, pt1, trigger, nelec, nmu):
        """Set properties of the HH event."""
        self.hh_exists = True
        self.hh_pt0 = pt0
        self.hh_pt1 = pt1
        self.hh_trigger = trigger
        self.hh_nelec = nelec
        self.hh_nmu = nmu

    def set_h(self, pt0, pt1, trigger, nelec, nmu):
        """Set properties of the H event."""
        self.h_exists = True
        self.h_pt0 = pt0
        self.h_pt1 = pt1
        self.h_trigger = trigger
        self.h_nelec = nelec
        self.h_nmu = nmu

    def __str__(self):
        """Return the string representation of this event."""
        if self.hh_exists and not self.h_exists:
            return "runNumber={}, eventNumber={}; HH: pt0={}, pt1={}, trigger={}, nElec={}, nMu={}".format(
                self.run_number,
                self.event_number,
                self.hh_pt0,
                self.hh_pt1,
                self.hh_trigger,
                self.hh_nelec,
                self.hh_nmu,
            )
        elif not self.hh_exists and self.h_exists:
            return "runNumber={}, eventNumber={}; H: pt0={}, pt1={}, trigger={}, nElec={}, nMu={}".format(
                self.run_number, self.event_number, self.h_pt0, self.h_pt1, self.h_trigger, self.h_nelec, self.h_nmu
            )
        else:
            string_rep = "runNumber={}, eventNumber={}; HH: pt0={}, pt1={}, trigger={}, nElec={}, nMu={}; H: pt0={}, pt1={}, trigger={}, nElec={}, nMu={}"
            return string_rep.format(
                self.run_number,
                self.event_number,
                self.hh_pt0,
                self.hh_pt1,
                self.hh_trigger,
                self.hh_nelec,
                self.hh_nmu,
                self.h_pt0,
                self.h_pt1,
                self.h_trigger,
                self.h_nelec,
                self.h_nmu,
            )


class EventList(object):
    """The EventList class manages all events of one run."""

    def __init__(self, run_number):
        self.events = dict()
        self.run_number = run_number

    def get_event(self, run_number, event_number):
        if not run_number == self.run_number:
            logging.error("Event list has run_number '%d', but '%d' requested!", self.run_number, run_number)
            return None
        else:
            if event_number not in self.events:
                event = Event(run_number, event_number)
                self.events[event_number] = event
            return self.events[event_number]

    def check_missing_events(self):
        missing_in_h = 0
        missing_in_hh = 0
        for event_number in sorted(self.events.keys()):
            event = self.events[event_number]
            if event.hh_exists and not event.h_exists:
                missing_in_h += 1
                logging.info("Missing in H: %s", event)
            elif not event.hh_exists and event.h_exists:
                missing_in_hh += 1
                logging.info("Missing in HH: %s", event)
        return missing_in_h, missing_in_hh


class EventListCollection(object):
    """The EventListCollection class manages several EventList instances."""

    def __init__(self):
        self.eventlists = dict()

    def get_event(self, run_number, event_number):
        if run_number not in self.eventlists:
            eventlist = EventList(run_number)
            self.eventlists[run_number] = eventlist
        return self.eventlists[run_number].get_event(run_number, event_number)

    def check_missing_events(self):
        logging.info("Checking missing events")
        missing_in_h = 0
        missing_in_hh = 0
        for run_number in sorted(self.eventlists.keys()):
            logging.info("Checking run '%s'", run_number)
            n_h, n_hh = self.eventlists[run_number].check_missing_events()
            missing_in_h += n_h
            missing_in_hh += n_hh
        logging.info("There are '%d' events missing in H", missing_in_h)
        logging.info("There are '%d' events missing in HH", missing_in_hh)


def find_files(root_path, extension="csv"):
    """Recursively find all files with the given extension in a directory.

    Arguments:
        root_path (str): Starting directory to look for files.
        extension (str): Extension of the files (default=csv).

    Returns:
        list(str): A list of all file paths relative to the `root_path`.
    """
    files = list()
    for root, directories, filenames in sorted(os.walk(root_path)):
        for filename in filenames:
            if filename.endswith(extension):
                file_path = os.path.join(root, filename)
                logging.debug("Found csv file '%s'", file_path)
                files.append(file_path)
    return files


def parse_csv(file_path, analysis_type, event_collection):
    logging.info("Parsing file '%s' for analysis '%s'", file_path, analysis_type)
    trigger_dict = dict()
    if analysis_type == "HH":
        trigger_dict["10"] = "SET"
        trigger_dict["11"] = "SET"
        trigger_dict["20"] = "SMT"
        trigger_dict["21"] = "SMT"
        trigger_dict["30"] = "DET"
        trigger_dict["31"] = "DET"
        trigger_dict["40"] = "DMT"
        trigger_dict["41"] = "DMT"
        trigger_dict["50"] = "EMT"
        trigger_dict["51"] = "EMT"
    elif analysis_type == "H":
        trigger_dict["10"] = "SMT"
        trigger_dict["11"] = "SMT"
        trigger_dict["12"] = "DMT"
        trigger_dict["20"] = "SET"
        trigger_dict["21"] = "SET"
        trigger_dict["22"] = "DET"
        trigger_dict["211"] = "EMT"

    with open(file_path, "rb") as csvfile:
        header = [h.replace('"', "").strip() for h in csvfile.next().split(",")]  # remove whitespaces from header
        reader = csv.DictReader(csvfile, delimiter=",", quotechar='"', fieldnames=header)
        for line in reader:
            run_number = int(line["runNumber"])
            event_number = int(float(line["eventNumber"]))
            trigger = trigger_dict[line["trigger"]]
            pt0 = float(line["Lepton0Pt"])
            pt1 = float(line["Lepton1Pt"])
            nElec = int(line["nElec"])
            nMu = int(line["nMu"])

            event = event_collection.get_event(run_number, event_number)
            if analysis_type == "HH":
                event.set_hh(pt0, pt1, trigger, nElec, nMu)
            elif analysis_type == "H":
                event.set_h(pt0, pt1, trigger, nElec, nMu)


def main(args):
    csv_files_hh = find_files(args.input_dir_hh)
    csv_files_h = find_files(args.input_dir_h)

    event_collection = EventListCollection()

    for csv_file in csv_files_hh:
        parse_csv(csv_file, "HH", event_collection)
    for csv_file in csv_files_h:
        parse_csv(csv_file, "H", event_collection)

    event_collection.check_missing_events()

    logging.info("Done")


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Compare event lists from HH and H analysis")
    PARSER.add_argument("input_dir_hh", metavar="INPUT_DIR_HH", type=str, help="Input directory for HH")
    PARSER.add_argument("input_dir_h", metavar="INPUT_DIR_H", type=str, help="Input directory for H")
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)
    ARGS = PARSER.parse_args()

    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    main(ARGS)
