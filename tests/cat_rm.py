#!/usr/bin/env python3
"""Concatenate and delete files."""

from pathlib import Path
import shutil


def cat_rm(dest: str, srcs: list[str]):
    """Concatenate several files and delete them afterwards."""
    if not len(srcs):
        return

    with open(dest, "w+b") as d_file:
        for source_file in srcs:
            with open(source_file, "rb") as s_file:
                shutil.copyfileobj(s_file, d_file)

            Path(source_file).unlink(missing_ok=True)
