#!/usr/bin/env python3
"""Runs shell tests specified in the command line."""

import argparse
from collections.abc import Iterable, Sequence
import importlib.util
from pathlib import Path
import sys
import types

import terminal_styling
import shell_test


def import_from_path(module_name: str, file_path: str) -> types.ModuleType:
    """Import module directly from path."""
    spec = importlib.util.spec_from_file_location(module_name, file_path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module


def parse_argv(argv: Sequence[str]) -> argparse.Namespace:
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(
        description="Test the given shell program against the given tests.")
    parser.add_argument("--fail-fast", action=argparse.BooleanOptionalAction,
                        help="stop tests after the first failed test.")
    parser.add_argument("-s", "--silent", action="store_true",
                        help="do not print reports for tests.")
    parser.add_argument("--control-shell", default="sh",
                        help="the shell to compare output against.")
    parser.add_argument("test_shell", help="the shell to be tested.")
    parser.add_argument("test_files", nargs="+",
                        help="list of python files to load tests from.")

    return parser.parse_args(argv)


def main(
        test_shell: str, test_files: Iterable[str],
        control_shell="sh", fail_fast=True, silent=False
):
    """Entry point of the program."""
    test_modules: list[types.ModuleType] = [
        import_from_path(f"{Path(file).parent}.{Path(file).stem}", file)
        for file in set(test_files)
    ]

    test_modules.sort(key=lambda mod: mod.__name__)
    module_map: dict[str, list[shell_test.ShellTestSuite]] = dict()
    for test_mod in test_modules:
        suite_list: list[shell_test.ShellTestSuite] = list()
        for suite_class in sorted(dir(test_mod)):
            mod_attr = getattr(test_mod, suite_class)
            if (
                isinstance(mod_attr, type) and
                issubclass(mod_attr, shell_test.ShellTestCase) and
                mod_attr is not shell_test.ShellTestCase
            ):
                suite = shell_test.ShellTestSuite(
                    mod_attr, test_shell, control_shell,
                    no_report=silent, fail_fast=fail_fast
                )
                if suite.total_tests > 0:
                    suite_list.append(suite)

        if len(suite_list) > 0:
            module_map[test_mod.__name__] = suite_list

    for module, suite_list in module_map.items():
        print(
            terminal_styling.bold(terminal_styling.bright_white(module)),
            file=sys.stderr
        )
        for test_suite in suite_list:
            test_suite.run()
            if test_suite.failed_tests > 0 and fail_fast is True:
                break

        if test_suite.failed_tests > 0 and fail_fast is True:
            break


if __name__ == "__main__":
    main(**vars(parse_argv(sys.argv[1:])))
