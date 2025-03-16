#!/usr/bin/env python3
"""Tests for simple_shell task5."""

from glob import glob

from cat_rm import cat_rm
from shell_test import ShellTestCase


class TestTask05(ShellTestCase):
    """Tests for simple_shell task5."""

    def teardown_test_shell(self):
        """Clean up after test."""
        cat_rm(self.asan_logfile, glob(f"{self.asan_logfile}.*"))
