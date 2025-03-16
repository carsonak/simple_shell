#!/usr/bin/env python3
"""Prettify line diffs."""

from collections.abc import Sequence
from difflib import unified_diff
import terminal_styling


def decode_bytes(lines: Sequence[str | bytes]) -> list[str]:
    """Turn the list of bytes in `lines` into a list of strings."""
    return [
        line.decode() if isinstance(line, bytes) else line
        for line in lines
    ]


def pretty_unified_diff(
        a: Sequence[str | bytes], b: Sequence[str | bytes],
        fromfile="", tofile="", fromfiledate="", tofiledate="",
        n=3, lineterm="\n"
) -> str:
    """Prettify a unified diff of lines in a and b."""
    u_diff = list(unified_diff(
        decode_bytes(a), decode_bytes(b), fromfile=fromfile, tofile=tofile,
        tofiledate=tofiledate, fromfiledate=fromfiledate, n=n,
        lineterm=lineterm
    ))

    for i, line in enumerate(u_diff[:]):
        if i < 2:
            continue

        if line.startswith("+"):
            u_diff[i] = terminal_styling.bright_green(line)
        elif line.startswith("-"):
            u_diff[i] = terminal_styling.bright_red(line)
        elif line.startswith("@@"):
            u_diff[i] = terminal_styling.cyan(line)

    return "".join(u_diff)
