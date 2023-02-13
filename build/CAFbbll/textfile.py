# -*- coding: utf-8 -*-

"""Basic writing to encoded text files"""

import codecs
import logging
import os


class TextFile(object):
    """Basic writing to encoded text files"""

    def __init__(self, path, mode="w", enc="utf-8"):
        """Constructor, opens file.

        Arguments:
        path -- absolute path to file
        mode -- write mode (usually 'w' for overwriting or 'a' for appending)
        enc  -- encoding (default = 'utf-8')
        """
        # if file/directory does not exist create dirs and file
        if not os.path.exists(path):
            if not os.path.exists(os.path.dirname(path)):
                os.mkdir(os.path.dirname(path))
            codecs.open(path, "a", enc).close()
        # open file with actual mode
        self._file = codecs.open(path, mode, enc)
        self._path = path

    @classmethod
    def from_rel_path(cls, path, mode="w", enc="utf-8"):
        """Creates a new instance of TextFile from a relative path.

        Arguments:
        path -- relative path to file
        mode -- write mode (usually 'w' for overwriting or 'a' for appending)
        enc  -- encoding (default = 'utf-8')
        """
        current_dir = os.getcwd()
        full_path = os.path.abspath(os.path.join(current_dir, path))
        return cls(full_path, mode, enc)

    def get_file(self):
        """returns handle to file"""
        return self._file

    def write(self, text):
        """writes text to file

        Arguments:
        text -- text to write
        """
        self._file.write(text)

    def writeline(self, text="", *args):
        """writes text and finishes line with a linebreak.
        If more than one argument is given, joins 2nd to last argument
        with first one and finishes line with a linebreak.

        Arguments:
        text  -- text to write or text, which is used to join other arguments
        *args -- additional arguments, which are joined by first one
        """
        if not args:
            self._file.write(text + "\n")
        else:
            self._file.write(text.join(args) + "\n")

    def writelines(self, lines):
        """writes multiple lines

        Arguments:
        lines -- a list of lines to write
        """
        self._file.writelines(lines)

    def write_table(self, data, format_string, head=None, ntabs=1):
        """writes a list of lists to an formatted table

        Arguments:
        data    -- 2darray, list of rows, each row is a list of data
        format  -- list of formatting rules, how to convert numbers into strings
        head    -- list of descriptions for columns, is used as first row (default=None)
        ntabs   -- number of tabulator indentations (default=1)
        """
        if head:
            self.writeline("\t" * ntabs, *head)
        for row in data:
            if len(row) == len(format_string):
                self.writeline(("\t" * ntabs).join(format_string).format(*row))
            else:
                logging.error(
                    "Length of row (%d) does not match length of format list (%d).", len(row), len(format_string)
                )

    def close(self):
        """closes file"""
        self._file.close()

    def __enter__(self):
        """if entering with-statement, returns instance"""
        return self

    def __exit__(self, exc_type, value, traceback):
        """if exiting with-statement, close file"""
        self.close()
