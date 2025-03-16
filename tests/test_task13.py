#!/usr/bin/env python3
"""Tests for simple_shell task13."""

from glob import glob

from cat_rm import cat_rm
from shell_test import ShellTestCase


class TestTask13(ShellTestCase):
    """Tests for simple_shell task13."""

    def teardown_test_shell(self):
        """Clean up after test."""
        cat_rm(self.asan_logfile, glob(f"{self.asan_logfile}.*"))
