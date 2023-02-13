#!/usr/bin/env python
"""
The python script toy_controller.py behaves as a submission controller without
actually executing any jobs. It can be used during testing to see if
submissions work as expected. The tool comes with a command-line interface
similar to popular submission frameworks.  The submissions are stored in a
temporary file. All jobs run as long as specified by their maximum time limit.
When the script is invoked with the 'queue' argument, the script prints the
current queue status. The mock controller supports dependencies.
"""

import argparse
import shelve
import time
import json

def read_database(database):
    """
    Open the database file and read all its information. The content is
    returned as a dictionary. The keys are the job ids, the items are
    dictionaries with the following information about a job: identifier, start
    time, end time, dependency. If the database does to exist, an empty
    dictionary is returned.
    """
    db = shelve.open(database)
    if "queue" in db:
        content = db["queue"]
    else:
        content = {}  # new database without any jobs
    db.close()
    return content

def write_database(database, content):
    """
    Write the given queue information to the database. The structure of the
    content is expected to match the structured returned by read_database.
    """
    db = shelve.open(database)
    db["queue"] = content
    db.close()


def add_job(database, depend, time_limit, identifier):
    """
    Opens the database, resolves dependencies, calculates the start and end
    times of the job and writes the changes to the database. The method
    returns the job id of the new job.
    """
    queue = read_database(database)

    # Determine next job id
    if len(queue):
        # There are some jobs
        next_job_id = max(queue) + 1
    else:
        next_job_id = 10042  # initial job id

    # Resolve dependencies and calculate start/end times
    starttime = time.time()
    recognized_dependencies = [i for i in depend if i in queue]
    dependencies_end = [queue[i]["endtime"] for i in recognized_dependencies]
    if len(dependencies_end):
        starttime = max(time.time(), *dependencies_end)
    else:
        starttime = time.time()
    endtime = starttime + time_limit

    # Add the new job
    queue[next_job_id] = {"identifier": identifier,
                          "starttime": starttime,
                          "endtime": endtime,
                          "dependencies": recognized_dependencies}
    write_database(database, queue)

    # Return job id
    return next_job_id

def submit_job(database, depend, time_limit, identifier):
    """
    Calls add_job to enter the job into the database and print the json job
    object.
    """
    job_id = add_job(database, depend, time_limit, identifier)

    output = {"identifier": identifier, "job_id": job_id}
    print(json.dumps(output, sort_keys=True, indent=4, separators=(',', ': ')))

def print_queue(database):
    """
    Reads the database and print status information about all jobs. Jobs can
    be either done (not in the list), running or waiting for a dependency.
    """
    queue = read_database(database)

    now = time.time()
    output = [{"identifier": j["identifier"],
               "job_id": i,
               "state": "running" if j["starttime"] < now else "pending"}
               for i, j in sorted(queue.items()) if j["endtime"] > now]

    print(json.dumps(output, sort_keys=True, indent=4, separators=(',', ': ')))

def command_select(database, command, depend=None, time_limit=None, identifier=None):
    """
    Check the command value and call the corresponding method.
    """
    if command == "queue":
        print_queue(database)
    elif command == "submit":
        submit_job(database, depend, time_limit, identifier)
    else:
        raise Exception("Argument parser returned an illegal value.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=
                                          "Moch script which imitates a basic "
                                          "submission script. This mock script"
                                          " can be sued during testing to "
                                          "submit jobs and poll the queue. The"
                                          " controller supports job "
                                          "dependencies.")

    parser.add_argument("--database", "-f",
                        help="Path to the file which stores the queue "
                             "infromation.")

    parser.add_argument("command", metavar="COMMAND",
                        choices=["submit", "queue"],
                        help="Selects the operation to perform: Add a new job "
                             "to the queue and print its status in json "
                             "format, or print the whole queue in json format.")

    parser.add_argument("--depend", "-d", type=int, default=[], action='append',
                        help="Dependency specification. The new job will not "
                             "start before the job with the given job id is "
                             "done, i.e. it reached its maximal run time. Any "
                             "unknown job id is ignored. This option is ignored"
                             " if 'queue' is selected.")

    parser.add_argument("--time-limit", "-t", type=int, default=60,
                        help="The maximal run time of the job in seconds. In "
                             "this mock implementation, the maximal run time "
                             "equals the actual run time. This option is "
                             "ignored if 'queue' is selected.")

    parser.add_argument("--identifier", "-i",  default=None,
                        help="An arbitrary string used to identify the job. "
                             "This option is ignored if 'queue' is selected.")

    command_select(**vars(parser.parse_args()))
