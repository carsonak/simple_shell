#!/usr/bin/env python3
"""Tests for simple_shell task01."""

from glob import glob
from pathlib import Path
import shutil

from cat_rm import cat_rm
from shell_test import ShellTestCase


class TestTask01(ShellTestCase):
    """Tests for simple_shell task01."""

    def teardown_test_shell(self):
        """Clean up after test."""
        cat_rm(self.asan_logfile, glob(f"{self.asan_logfile}.*"))

    def test_bin_ls(self):
        """Execute `/bin/ls`."""
        command = b"/bin/ls"
        self.run_ctrl_shell(command)
        self.run_test_shell(command)

    def test_bin_ls_x3(self):
        """Execute `/bin/ls` 3 times."""
        command = b"/bin/ls\n/bin/ls\n/bin/ls"
        self.run_ctrl_shell(command)
        self.run_test_shell(command)

    def test_bin_ls_with_spaces_x4(self):
        """Execute `/bin/ls` surrounded by spaces 4 times."""
        command = b"/bin/ls\n     /bin/ls\n/bin/ls\n   /bin/ls     "
        self.run_ctrl_shell(command)
        self.run_test_shell(b"/bin/ls\n     /bin/ls\n/bin/ls\n        ")

    def test_spaces_only(self):
        """Input only spaces."""
        command = b"                "
        self.run_ctrl_shell(command)
        self.run_test_shell(command)

    def test_spaces_only_medium(self):
        """Input more spaces."""
        command = b"                                                       \n"
        self.run_ctrl_shell(command)
        self.run_test_shell(command)

    def test_spaces_only_large(self):
        """Input even more spaces."""
        command = b"                                                                                                                                                                            \n"  # noqa: E501,B950
        self.run_ctrl_shell(command)
        self.run_test_shell(command)


class TestTask01CopyFile(ShellTestCase):
    """Tests for simple shell task01 with different setup."""

    def teardown_test_shell(self):
        """Clean up after test."""
        cat_rm(self.asan_logfile, glob(f"{self.asan_logfile}.*"))

    def setup(self) -> None:
        """Copy ls program into current directory."""
        shutil.copy("/bin/ls", "./hbtn_ls")

    def teardown(self) -> None:
        """Delete copied file."""
        Path("./hbtn_ls").unlink()

    def test_hbtn_ls_with_argument(self):
        """Copy `/bin/ls` into current directory and execute with args."""
        command = b"./hbtn_ls /var"
        self.run_ctrl_shell(command)
        self.run_test_shell(command)
