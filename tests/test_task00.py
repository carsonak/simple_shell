#!/usr/bin/env python3
"""Tests for simple_shell task0."""

from glob import glob
from pathlib import Path
import shutil

from ShellTest import ShellTestCase


def cat_and_rm_files(dest: str, srcs: list[str]):
    """Concatenate several files and delete them afterwards."""
    if not len(srcs):
        return

    with open(dest, "w+b") as d_file:
        for source_file in srcs:
            with open(source_file, "rb") as s_file:
                shutil.copyfileobj(s_file, d_file)

            Path(source_file).unlink(missing_ok=True)


class TestTask0(ShellTestCase):
    """Tests for simple_shell task0."""

    def teardown_test_shell(self):
        """Clean up after test."""
        cat_and_rm_files(self.asan_outfile, glob(f"{self.asan_outfile}.*"))

    def test_ls_x_3(self):
        """Execute `/bin/ls` 3 times."""
        command = b"/bin/ls\n/bin/ls\n/bin/ls"
        self.run_test_shell(command)
        self.run_ctrl_shell(command)
