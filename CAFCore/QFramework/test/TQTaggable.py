#!/usr/bin/env python

import os
import tempfile
import unittest

import ROOT

import QFramework
from QFramework import TQTaggable

from QFramework import TQPythonTest

class TQTaggableTest(TQPythonTest):
    def test_overwritingLists(self):
        tags = TQTaggable()
        tags.setTagAuto("tag", "a,b,c,d")
        self.assertEqual(tags.getTagStringDefault("tag.0", ""), "a")
        self.assertEqual(tags.getTagStringDefault("tag.1", ""), "b")
        self.assertEqual(tags.getTagStringDefault("tag.2", ""), "c")
        self.assertEqual(tags.getTagStringDefault("tag.3", ""), "d")

        tags.setGlobalOverwrite(True)
        tags.setTagAuto("tag", "a0,a1,a2")
        self.assertEqual(tags.getTagStringDefault("tag.0", ""), "a0")
        self.assertEqual(tags.getTagStringDefault("tag.1", ""), "a1")
        self.assertEqual(tags.getTagStringDefault("tag.2", ""), "a2")
        self.assertEqual(tags.getTagStringDefault("tag.3", ""), "")

    def test_default_constructor(self):
        """
        Check that the default constructor returns a non-null object and does
        not throw an exception.
        """
        try:
            tags = TQTaggable()
        except Exception as e:
            self.fail("Default constructor raise an exception: %s" % e)

        self.assertTrue(tags)

    def test_constructor_with_tag_string(self):
        """
        Check that when a TQTaggable is created with a comma separated list of
        tags, the tags retrievable.
        """
        tags = TQTaggable("tagA=1,tagB=2,tagC=hello")

        # check for existing tags
        self.assertEqual(tags.getTagIntegerDefault("tagA", 0), 1)
        self.assertEqual(tags.getTagIntegerDefault("tagB", 0), 2)
        self.assertEqual(tags.getTagStringDefault("tagC", ""), "hello")

        # check that no other tags exists
        self.assertEqual(tags.getTagIntegerDefault("tagD", 42), 42)
        self.assertEqual(tags.getTagStringDefault("tagC", ""), "hello")

    def test_addingTagsWithoutGlobalOverwrite(self):
        """
        When fGlobalOverwrite is set to false, determining if a tag should be
        set or ignored can become tricky. This is important when config files
        include other config files. This method tests all cases.
        """
        mainTagList = [{"tag.0":"main.0"},
                      {"tag.1":"main.1"},
                      {"tag.0":"main.0" ,"tag.1":"main.1"}]

        includeTagList = [{"tag.0": "incl.0"},
                          {"tag.1": "incl.1"},
                          {"tag.0": "incl.0", "tag.1": "incl.1"},
                          {"tag"  : "incl.0"},
                          {"tag"  : "incl.0, incl.1"}]

        # The easy case: first, "tag" is defined. Since globalOverwrite is
        # deactivated, nothing else can be written (no tag.0, tag.1 etc)
        for includeTagSet in includeTagList:
            taggable = TQTaggable("tag=main")
            taggable.setGlobalOverwrite(False)
            for key in includeTagSet:
                value = includeTagSet[key]
                taggable.setTagAuto(key, value)
            self.assertEqual(taggable.getTagStringDefault("tag", ""), "main")
            self.assertEqual(taggable.getTagStringDefault("tag.0", ""), "")
            self.assertEqual(taggable.getTagStringDefault("tag.1", ""), "")

        # The ugly case: In the first three loops over includeTagList, the
        # tags are all defined explicitly. Therefore, we can compare "taggable"
        # to a reference taggable, in which globalOverwrite is activated and
        # all tags are set in opposite order.
        # For the other two cases, the cases are simply hard-coded.
        for i, mainTagSet in enumerate(mainTagList):
            for j, includeTagSet in enumerate(includeTagList):
                initString = ""
                for key in mainTagSet:
                    value = mainTagSet[key]
                    initString += key+"="+value+","
                initString.strip(",")
                taggable = TQTaggable(initString)

                taggable.setGlobalOverwrite(False)
                for inclKey in includeTagSet:
                    inclValue = includeTagSet[inclKey]
                    taggable.setTagAuto(inclKey, inclValue)

                initString = ""
                for key in includeTagSet:
                    value = includeTagSet[key]
                    initString += key+"="+value+","
                initString.strip(",")
                taggableCompare = TQTaggable(initString)

                for key in mainTagSet:
                    value = mainTagSet[key]
                    taggableCompare.setTagAuto(key, value)

                taggableCompare.setTagAuto(key, value)

                for testKey in ["tag", "tag.0", "tag.1"]:
                    if j < 3:
                        self.assertEqual(taggable.getTagStringDefault(testKey, ""), taggableCompare.getTagStringDefault(testKey, ""))
                    else:
                        # if tag.<integer> is already set, tag should never be set anymore
                        self.assertEqual(taggable.getTagStringDefault("tag", ""), "")
                        if j == 3:
                            if i == 0:
                                self.assertEqual(taggable.getTagStringDefault("tag.0", ""), "main.0")
                                self.assertEqual(taggable.getTagStringDefault("tag.1", ""), "")
                            if i == 1:
                                self.assertEqual(taggable.getTagStringDefault("tag.0", ""), "incl.0")
                                self.assertEqual(taggable.getTagStringDefault("tag.1", ""), "main.1")
                            if i == 2:
                                self.assertEqual(taggable.getTagStringDefault("tag.0", ""), "main.0")
                                self.assertEqual(taggable.getTagStringDefault("tag.1", ""), "main.1")
                        if j == 4:
                            if i == 0:
                                self.assertEqual(taggable.getTagStringDefault("tag.0", ""), "main.0")
                                self.assertEqual(taggable.getTagStringDefault("tag.1", ""), "incl.1")
                            if i == 1:
                                self.assertEqual(taggable.getTagStringDefault("tag.0", ""), "incl.0")
                                self.assertEqual(taggable.getTagStringDefault("tag.1", ""), "main.1")
                            if i == 2:
                                self.assertEqual(taggable.getTagStringDefault("tag.0", ""), "main.0")
                                self.assertEqual(taggable.getTagStringDefault("tag.1", ""), "main.1")


if __name__ == "__main__":
    unittest.main()
