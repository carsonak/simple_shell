#!/usr/bin/env python3
"""Module for ShellTest."""

import atexit
from collections.abc import Callable, Iterable
from dataclasses import dataclass
import io
import os
from pathlib import Path
import subprocess
import sys
import tempfile

from pretty_diff import pretty_unified_diff
import terminal_styling as t_style


@dataclass(slots=True)
class ShellTestResult:
    """Holds status of a test result."""
    command: bytes
    args: Iterable[bytes]
    return_status: int
    stdout: bytes
    stderr: bytes


TIMED_OUT = f"{t_style.bold(t_style.yellow("TIMED OUT"))}\n".encode()


class ShellTestCase:
    """Test Case."""

    stdin: None | io.FileIO | int = None
    stdout: None | io.FileIO | int = None
    stderr: None | io.FileIO | int = None

    def __init__(
        self, method_name: str, test_shell: str,
        control_shell="sh", environment=os.environ,
        no_report=False, check_asan=True
    ):
        """Create class instance that uses given method when run.

        Args:
            method_name: A method of this class to run.

        Raises:
            ValueError: if the instance does not have a method with the
                specified name.
        """
        self.test_method: Callable[[], None]
        try:
            self.test_method = getattr(self, method_name)
        except AttributeError as exp:
            raise ValueError(
                f"no such test method in {self.__class__}: {method_name}"
            ) from exp

        self.test_shell = test_shell
        self.control_shell = control_shell
        self.environment: dict[str, str] = environment
        self.no_report = no_report
        self.asan_logfile = None
        self.timed_out = False
        self.test_failed = False
        if no_report is False and check_asan is True:
            self.asan_logfile = (
                f"ASAN-{"-".join(self.test_method.__module__.split("."))}-"
                f"{"-".join(self.test_method.__qualname__.split("."))}-"
            )
            self.environment.update({
                "ASAN_OPTIONS": (
                    "detect_leaks=1"
                    ":detect_stack_use_after_return=true"
                    ":strict_string_checks=true"
                    ":replace_str=true"
                    f":log_path={self.asan_logfile}"
                )
            })

    @property
    def asan_logfile(self) -> str | None:
        """Return name of the ASAN logfile."""
        return self.__asan_logfile

    @asan_logfile.setter
    def asan_logfile(self, filename: str | None) -> None:
        """Create the ASAN log file."""
        if filename is None:
            self.__asan_logfile = None
            return

        with tempfile.NamedTemporaryFile(
                prefix=filename, delete=False) as asan_logfile:
            self.__asan_logfile = asan_logfile.name

        atexit.register(Path(self.__asan_logfile).unlink, missing_ok=True)

    @classmethod
    def class_setup(cls):
        """Class setup."""
        pass

    @classmethod
    def class_teardown(cls):
        """Class teardown."""
        pass

    def setup(self):
        """Setup for a single test."""
        pass

    def teardown(self):
        """Cleanup after a single test."""

    def setup_test_shell(self):
        """Test case setup for the test shell."""
        pass

    def teardown_test_shell(self):
        """Test case teardown for the test shell."""
        pass

    def setup_ctrl_shell(self):
        """Test case setup for the control shell."""
        pass

    def teardown_ctrl_shell(self):
        """Test case teardown for the control shell."""
        pass

    def _run_shell(
        self, *args: bytes,
        command: bytes | None = None, is_ctrl: bool = False,
        timeout: float | None = 3.0
    ) -> subprocess.CompletedProcess:
        """Run a shell with the command given."""
        setup = self.setup_test_shell
        teardown = self.teardown_test_shell
        shell = self.test_shell
        if is_ctrl is True:
            setup = self.setup_ctrl_shell
            teardown = self.teardown_ctrl_shell
            shell = self.control_shell

        setup()
        stdin = self.stdin
        if command is not None:
            stdin = None

        if stdin is not None or self.stdout is not None:
            capture_output = False
        else:
            capture_output = True

        try:
            process_output = subprocess.run(
                [shell, *args], input=command,
                stdin=stdin, stdout=self.stdout,
                capture_output=capture_output, timeout=timeout,
                env=self.environment
            )
        finally:
            teardown()

        return process_output

    def run_test_shell(
        self, command: bytes | None = None, *args: bytes,
        timeout: float | None = 5.0
    ):
        """Run the test shell with the command given."""
        self.timed_out = False
        try:
            res = self._run_shell(
                *args, command=command, is_ctrl=False, timeout=timeout)
        except subprocess.TimeoutExpired:
            self.test_failed = True
            self.timed_out = True

        if self.timed_out:
            self.test_result = ShellTestResult(
                command=command if command else b"", args=args,
                return_status=-1, stdout=TIMED_OUT,
                stderr=TIMED_OUT
            )
        else:
            self.test_result = ShellTestResult(
                command=command if command else b"", args=args,
                return_status=res.returncode,
                stdout=res.stdout, stderr=res.stderr
            )

    def run_ctrl_shell(
        self, command: bytes | None = None, *args: bytes,
        timeout: float | None = 5.0
    ):
        """Run the control shell with the command given."""
        self.timed_out = False
        try:
            res = self._run_shell(
                *args, command=command, is_ctrl=True, timeout=timeout)
        except subprocess.TimeoutExpired:
            self.test_failed = True
            self.timed_out = True

        if self.timed_out:
            self.ctrl_result = ShellTestResult(
                command=command if command else b"", args=args,
                return_status=-1, stdout=TIMED_OUT,
                stderr=TIMED_OUT
            )
        else:
            self.ctrl_result = ShellTestResult(
                command=command if command else b"", args=args,
                return_status=res.returncode,
                stdout=res.stdout, stderr=res.stderr
            )

    def _report_test(self) -> str:
        """Print out status of the test."""
        leaky = False
        if self.asan_logfile is not None:
            leaky = Path(self.asan_logfile).stat().st_size > 0

        if not leaky and self.test_result == self.ctrl_result:
            return ""

        self.test_failed = True
        diff_stdout = pretty_unified_diff(
            self.test_result.stdout.splitlines(keepends=True),
            self.ctrl_result.stdout.splitlines(keepends=True),
            fromfile="test_shell_result", tofile="ctrl_shell_result"
        )
        diff_stderr = pretty_unified_diff(
            self.test_result.stderr.splitlines(keepends=True),
            self.ctrl_result.stderr.splitlines(keepends=True),
            fromfile="test_shell_result", tofile="ctrl_shell_result"
        )
        report = "\n".join([
            t_style.bold(t_style.blue("(command)")),
            f"echo {self.test_result.command.decode()} | {self.test_shell} {b" ".join(self.test_result.args).decode().strip()}",  # noqa: E501,B950
            f"{t_style.blue("(status)")} {self.test_shell}:{self.test_result.return_status} {self.control_shell}:{self.ctrl_result.return_status}",  # noqa: E501,B950
            t_style.blue("(diff-stdout)"), diff_stdout.strip(),
            t_style.blue("(diff-stderr)"), diff_stderr.strip()
        ])
        if leaky and self.asan_logfile is not None:
            with open(self.asan_logfile) as f:
                report += "\n" + f.read()

        return report

    def run(self) -> None:
        """Run the instance with the given method."""
        self.setup()
        try:
            self.test_method()
        finally:
            self.teardown()

        testname = f"{self.test_method.__qualname__}"
        ok = t_style.bright_green("OK")
        fail = t_style.bright_red("FAILED")
        doc = ""
        if self.test_method.__doc__ is not None:
            doc = self.test_method.__doc__

        report = self._report_test()
        if self.test_failed:
            print(f"{testname}: {fail} {doc}", file=sys.stderr)

            if self.no_report is False:
                print(report, file=sys.stderr)

            return

        print(f"{testname}: {ok}.", file=sys.stderr)


class ShellTestSuite:
    """Test Suite."""

    def __init__(self, suite: type[ShellTestCase],
                 test_shell: str, control_shell: str,
                 no_report=False, check_asan=True,
                 fail_fast=True):
        """Initialise ShellTestSuite."""
        if not issubclass(suite, ShellTestCase):
            raise TypeError(f"suite must be a subclass of {ShellTestCase}")

        self.fail_fast = fail_fast
        self.test_suite = suite
        self.test_cases = [
            self.test_suite(
                case, test_shell, control_shell,
                no_report=no_report, check_asan=check_asan
            )
            for case in self._extract_test_cases()
        ]
        self.total_tests = len(self.test_cases)
        self.failed_tests = 0

    def _extract_test_cases(self) -> list[str]:
        """Extract test cases from a test suite."""
        test_cases: list[str] = list()
        for attr_name, attr in vars(self.test_suite).items():
            if not attr_name.startswith("test"):
                continue

            if not callable(attr):
                continue

            test_cases.append(attr_name)

        test_cases.sort()
        return test_cases

    def run(self) -> None:
        """Run all tests in the suite."""
        self.test_suite.class_setup()
        try:
            for test_case in self.test_cases:
                test_case.run()
                if test_case.test_failed is True:
                    self.failed_tests += 1
                    if self.fail_fast is True:
                        break
        finally:
            self.test_suite.class_teardown()
