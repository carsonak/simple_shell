#!/usr/bin/env python3
"""Tests for simple_shell task2."""

from glob import glob

from cat_rm import cat_rm
from shell_test import ShellTestCase


class TestTask02(ShellTestCase):
    """Tests for simple_shell task01."""

    def teardown_test_shell(self):
        """Clean up after test."""
        cat_rm(self.asan_logfile, glob(f"{self.asan_logfile}.*"))

    def test_bin_ls_l(self):
        """Execute `/bin/ls -l`."""
        command = b"/bin/ls -l"
        self.run_ctrl_shell(command)
        self.run_test_shell(command)

    def test_bin_ls_var(self):
        """Execute `/bin/ls /var`."""
        command = b"/bin/ls /var"
        self.run_ctrl_shell(command)
        self.run_test_shell(command)

    def test_bin_echo_holberton(self):
        """Execute `/bin/echo Holberton`."""
        command = b"/bin/echo Holberton"
        self.run_ctrl_shell(command)
        self.run_test_shell(command)
