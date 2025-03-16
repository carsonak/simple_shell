#!/usr/bin/env python3
"""Terminal ANSI escape codes."""

from enum import StrEnum

RESET = "\033[0m"


class TextStyle(StrEnum):
    """Terminal text style codes."""
    BOLD = "\033[1m"


class TextColors(StrEnum):
    """Terminal colour codes."""
    BLUE = "\033[34m"
    BRIGHT_GREEN = "\033[92m"
    BRIGHT_RED = "\033[91m"
    BRIGHT_WHITE = "\033[97m"
    CYAN = "\033[36m"
    YELLOW = "\033[33m"


def bold(text: str) -> str:
    """Make text bold."""
    return f"{TextStyle.BOLD}{text}{RESET}"


def blue(text: str) -> str:
    """Make text blue."""
    return f"{TextColors.BLUE}{text}{RESET}"


def bright_green(text: str) -> str:
    """Make text bright green."""
    return f"{TextColors.BRIGHT_GREEN}{text}{RESET}"


def bright_red(text: str) -> str:
    """Make text bright red."""
    return f"{TextColors.BRIGHT_RED}{text}{RESET}"


def bright_white(text: str) -> str:
    """Make text bright white."""
    return f"{TextColors.BRIGHT_WHITE}{text}{RESET}"


def cyan(text: str) -> str:
    """Make text cyan."""
    return f"{TextColors.CYAN}{text}{RESET}"


def yellow(text: str) -> str:
    """Make text yellow."""
    return f"{TextColors.YELLOW}{text}{RESET}"
