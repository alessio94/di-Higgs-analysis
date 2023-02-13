# -*- coding: utf-8 -*-

"""Class for writing simple LaTeX files"""

from __future__ import print_function

import os

from CAFbbll.textfile import TextFile


class LaTeXFile(TextFile):

    """Class for writing simple LaTeX files"""

    def __init__(self, path, mode="w", enc="utf-8"):
        """Constructor, opens file"""
        TextFile.__init__(self, path, mode, enc)
        self.title = ""
        self.author = ""
        self.date = r"\today"
        self.document_settings = ["12pt", "a4paper"]

    def write_header(self):
        """Writes header for LaTeX file"""
        self.writeline(r"\documentclass[" + ",".join(self.document_settings) + "]{scrartcl}")
        self.writeline(r"\usepackage[T1]{fontenc}")
        self.writeline(r"\usepackage[utf8]{inputenc}")
        self.writeline(r"\usepackage{lmodern}")
        self.writeline(r"\usepackage{textgreek}")
        self.writeline(r"\usepackage[english]{babel}")

        self.writeline(r"\usepackage{amsmath}")
        self.writeline(r"\usepackage{amssymb}")
        self.writeline(r"\usepackage{amsfonts}")
        self.writeline(r"\usepackage{mathtools}")
        self.writeline(r"\usepackage{tensor}")
        self.writeline(r"\usepackage{commath}")
        self.writeline(r"\usepackage{bm}")
        self.writeline(r"\usepackage{siunitx}")

        self.writeline(r"\usepackage{float}")
        self.writeline(r"\usepackage{booktabs}")
        self.writeline(r"\usepackage{longtable}")
        self.writeline(r"\usepackage{array}")

        self.writeline(r"\usepackage[margin=1cm]{geometry}")

        self.writeline(r"\title{" + self.title + "}")
        self.writeline(r"\author{" + self.author + "}")
        self.writeline(r"\date{" + self.date + "}")

    def begin_document(self):
        """Writes '\\begin{document}' to file"""
        self.writeline(r"\begin{document}")

    def end_document(self):
        """Writes '\\end{document}' to file"""
        self.writeline(r"\end{document}")

    def make_title(self):
        """Writes '\\maketitle' to file"""
        self.writeline(r"\maketitle")

    def write_table(
        self, data, format_string, thead, caption="", label="", long_table=False, format_rows=""
    ):  # pylint: disable=arguments-differ
        """writes a list of lists to a formatted latex table

        Arguments:
        data          -- 2darray, list of rows, each row is a list of data
        thead         -- list of descriptions for columns, is used as first row
        format_string -- list of formatting rules, how to convert numbers into strings
        caption       -- caption of table in latex
        label         -- label of table in latex
        """
        if len(thead) == len(format_string):
            length = len(thead)
            i = "  "  # indentation
            format_rows_string = "c" * length
            if format_rows != "":
                format_rows_string = format_rows
            caption = caption.replace("_", r"\_")
            if not long_table:
                self.writeline(r"\begin{table}[H]")
                if caption != "":
                    self.writeline(r"\caption{" + caption + "}")
                self.writeline(r"\begin{center}")
                self.writeline(r"\begin{tabular}{" + format_rows_string + "}")
            else:
                self.writeline(r"\begin{longtable}{" + format_rows_string + "}")
                if caption != "":
                    self.writeline(r"\caption{" + caption + r"} \\")

            self.writeline(r"\toprule")
            self.writeline(i + " & ".join(thead) + r" \\ \midrule")

            if long_table:
                self.writeline(r"\endfirsthead")
                if caption != "":
                    self.writeline(r"\caption{" + caption + r" (continued)} \\")
                self.writeline(r"\toprule")
                self.writeline(i + " & ".join(thead) + r" \\ \midrule")
                self.writeline(r"\endhead")
                self.writeline(r"\bottomrule")
                self.writeline(r"\endfoot")

            for row in data:
                row = [cell.replace("_", r"\_") if isinstance(cell, str) else cell for cell in row]
                self.writeline(i + " & ".join(format_string).format(*row) + r" \\")

            if not long_table:
                self.writeline(r"\bottomrule")
                self.writeline(r"\end{tabular}")
                self.writeline(r"\end{center}")
                if label != "":
                    self.writeline(r"\label{" + label + "}")
                self.writeline(r"\end{table}")
            else:
                if label != "":
                    self.writeline(r"\label{" + label + "}")
                self.writeline(r"\end{longtable}")

        else:
            print("WARNING[{}]: lists have to be the same length.".format(self.__class__.__name__))

    def write_make_file(self):
        """Compiles the LaTeX file"""
        path = os.path.join(os.path.dirname(self._path), "Makefile")
        with TextFile(path, "w") as makefile:
            makefile.writeline("SOURCES  = $(wildcard *.tex)")
            makefile.writeline("TARGETS = $(SOURCES:%.tex=%.pdf)")
            makefile.writeline("$(TARGETS): $(SOURCES)")
            makefile.writeline("\tlatexmk -f -pdf $^")
