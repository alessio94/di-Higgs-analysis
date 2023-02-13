#!/usr/bin/env python

import re


def create_sample_list(input_file, output_file):
    search = r"((?:mc|data)1\d_13TeV)\.(\d+)\..+\.([easrpfm\d_]+)"
    regex = re.compile(search)
    replace = r"user.brottler.\1.\2.CxAOD_SUSY2.\3.BBLL_V02.1_CxAOD.root"

    with open(input_file) as input_file_handle:
        output_file_content = regex.sub(replace, input_file_handle.read())

    with open(output_file, "w") as output_file_handle:
        output_file_handle.write(output_file_content)


def main():
    input_path = "/home/br1033/bbll/CxAODFramework/source/CxAODOperations_HH_bbll/data/DxAOD/"
    output_path = "bbll/config/samples/grid/"
    version = "V02"

    files = list()
    files.append(("list_sample_grid.mc_a.SUSY2.txt", "{version}-mc16a.txt"))
    files.append(("list_sample_grid.mc_d.SUSY2.txt", "{version}-mc16d.txt"))
    files.append(("list_sample_grid.mc_e.SUSY2.txt", "{version}-mc16e.txt"))
    files.append(("list_sample_grid.data_a.SUSY2.txt", "{version}-data1516.txt"))
    files.append(("list_sample_grid.data_d.SUSY2.txt", "{version}-data17.txt"))
    files.append(("list_sample_grid.data_e.SUSY2.txt", "{version}-data18.txt"))

    for input_file, output_file in files:
        create_sample_list(input_path + input_file, output_path + output_file.format(version=version))


if __name__ == "__main__":
    main()
