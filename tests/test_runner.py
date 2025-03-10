#!/usr/bin/env python3
"""Runs shell tests specified in the command line."""

import argparse
import importlib.util
from pathlib import Path
import sys
import types

import ShellTest


def import_from_path(module_name: str, file_path: str) -> types.ModuleType:
    """Import module directly from path."""
    spec = importlib.util.spec_from_file_location(module_name, file_path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module


def parse_argv(argv: list[str]) -> argparse.Namespace:
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(
        description="Test the given shell program against the given tests.")
    parser.add_argument("--control-shell", default="sh",
                        help="the shell to compare output against.")
    parser.add_argument("test_shell", help="the shell to be tested.")
    parser.add_argument("test_files", nargs="+",
                        help="list of python files to load tests from.")

    return parser.parse_args(argv)


def main(test_shell: str, test_files: list[str], control_shell: str = "sh"):
    """Entry point of the program."""
    test_files.sort()
    test_modules: set[types.ModuleType] = {
        import_from_path(f"{Path(file).parent}.{Path(file).stem}", file)
        for file in test_files
    }
    module_map: dict[str, list[ShellTest.ShellTestSuite]] = dict()
    for test_mod in test_modules:
        suite_list: list[ShellTest.ShellTestSuite] = list()
        for suite in dir(test_mod):
            mod_attr = getattr(test_mod, suite)
            if (
                isinstance(mod_attr, type) and
                issubclass(mod_attr, ShellTest.ShellTestCase) and
                mod_attr is not ShellTest.ShellTestCase
            ):
                suite_list.append(ShellTest.ShellTestSuite(
                    mod_attr, test_shell, control_shell))

        module_map[test_mod.__name__] = suite_list

    for test_suite in module_map.values():
        for case in test_suite:
            case.run()


if __name__ == "__main__":
    main(**vars(parse_argv(sys.argv[1:])))
