#!/usr/bin/env python3
"""Module for ShellTest."""

import atexit
from collections.abc import Callable, Iterable
from dataclasses import dataclass
from enum import StrEnum
import io
import os
from pathlib import Path
import subprocess
import sys
import tempfile


class Colors(StrEnum):
    """Terminal colour codes."""
    RESET = "\033[0m"
    BOLD = "\033[1m"
    BRIGHT_GREEN_TEXT = "\033[92m"
    BRIGHT_WHITE_TEXT = "\033[97m"
    CYAN_TEXT = "\033[36m"
    YELLOW_TEXT = "\033[33m"


@dataclass(slots=True)
class ShellTestResult:
    """Holds status of a test result."""
    command: bytes
    args: Iterable[bytes]
    return_status: int
    timed_out: bool
    stdout: bytes
    stderr: bytes


TIMED_OUT = f"{Colors.BOLD}{Colors.YELLOW_TEXT}TIMED OUT{Colors.RESET}\n".encode()  # noqa: E501


class ShellTestCase:
    """Test Case."""

    stdin: None | io.FileIO | int = None
    stdout: None | io.FileIO | int = None
    stderr: None | io.FileIO | int = None
    test_shell_result = ShellTestResult(b"", list(), -1, False, b"", b"")
    ctrl_shell_result = ShellTestResult(b"", list(), -1, False, b"", b"")

    def __init__(
        self, method_name: str, test_shell: str,
        control_shell: str = "sh", environment=os.environ,
        check_asan: bool = True
    ):
        """Create class instance that uses given method when run.

        Args:
            method_name: A method of this class to run.

        Raises:
            ValueError: if the instance does not have a method with the
                specified name.
        """
        self.test_method: Callable[[None], None]
        try:
            self.test_method = getattr(self, method_name)
        except AttributeError as exp:
            raise ValueError(
                f"no such test method in {self.__class__}: {method_name}"
            ) from exp

        self.test_shell = test_shell
        self.control_shell = control_shell
        self.environment: dict[str, str] = environment
        self.asan_outfile = None
        if check_asan is True:
            filename = (
                f"ASAN-{"-".join(self.test_method.__module__.split("."))}-"
                f"{"-".join(self.test_method.__qualname__.split("."))}-"
            )
            with tempfile.NamedTemporaryFile(
                    prefix=filename, delete=False) as asan_outfile:
                self.asan_outfile = asan_outfile.name

            atexit.register(Path(self.asan_outfile).unlink, missing_ok=True)
            self.environment["ASAN_OPTIONS"] = (
                "detect_leaks=1"
                ":detect_stack_use_after_return=true:strict_string_checks=true"
                f":replace_str=true:log_path={self.asan_outfile}"
            )

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
        self, is_ctrl: bool, command: bytes | None = None,
        *args: bytes, timeout: float | None = 3.0
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
        timeout: float | None = 2.0
    ):
        """Run the test shell with the command given."""
        self.test_shell_result.timed_out = False
        try:
            res = self._run_shell(False, command, *args, timeout=timeout)
        except subprocess.TimeoutExpired:
            self.test_shell_result.timed_out = True

        self.test_shell_result.command = command if command else b""
        self.test_shell_result.args = args
        if self.test_shell_result.timed_out:
            self.test_shell_result.return_status = -1
            self.test_shell_result.stdout = TIMED_OUT
            self.test_shell_result.stderr = TIMED_OUT
        else:
            self.test_shell_result.return_status = res.returncode
            self.test_shell_result.stdout = res.stdout
            self.test_shell_result.stderr = res.stderr

    def run_ctrl_shell(
        self, command: bytes | None = None, *args: bytes,
        timeout: float | None = 2.0
    ):
        """Run the control shell with the command given."""
        self.ctrl_shell_result.timed_out = False
        try:
            res = self._run_shell(True, command, *args, timeout=timeout)
        except subprocess.TimeoutExpired:
            self.ctrl_shell_result.timed_out = True

        self.ctrl_shell_result.command = command if command else b""
        self.ctrl_shell_result.args = args
        if self.ctrl_shell_result.timed_out:
            self.ctrl_shell_result.return_status = -1
            self.ctrl_shell_result.stdout = TIMED_OUT
            self.ctrl_shell_result.stderr = TIMED_OUT
        else:
            self.ctrl_shell_result.return_status = res.returncode
            self.ctrl_shell_result.stdout = res.stdout
            self.ctrl_shell_result.stderr = res.stderr

    def _report_test(self) -> str:
        """Print out status of the test."""
        leaky = False
        if self.asan_outfile is not None:
            leaky = Path(self.asan_outfile).stat().st_size > 0

        if not leaky or self.test_shell_result == self.ctrl_shell_result:
            return Colors.BRIGHT_GREEN_TEXT + "OK." + Colors.RESET

        report = "\n".join([
            "",
            Colors.BOLD + Colors.BRIGHT_WHITE_TEXT + "(command)" + Colors.RESET,  # noqa: E501,B950
            f"echo {self.test_shell_result.command.decode()} | {self.test_shell} {b" ".join(self.test_shell_result.args).decode().strip()}",  # noqa: E501,B950
            Colors.BOLD + Colors.BRIGHT_WHITE_TEXT + "[GOT]:" + Colors.RESET,
            Colors.CYAN_TEXT + "(status)" + Colors.RESET + f" {self.test_shell_result.return_status}",  # noqa: E501,B950
            Colors.CYAN_TEXT + "(stdout)" + Colors.RESET,
            f"{self.test_shell_result.stdout.decode()}" + Colors.BOLD + Colors.BRIGHT_WHITE_TEXT + f"(Lines: {self.test_shell_result.stdout.count(b"\n")})" + Colors.RESET,  # noqa: E501,B950
            Colors.CYAN_TEXT + "(stderr)" + Colors.RESET,
            f"{self.test_shell_result.stderr.decode()}" + Colors.BOLD + Colors.BRIGHT_WHITE_TEXT + f"(Lines: {self.test_shell_result.stderr.count(b"\n")})" + Colors.RESET,  # noqa: E501,B950
            Colors.BOLD + Colors.BRIGHT_WHITE_TEXT + "[EXPECTED]:" + Colors.RESET,  # noqa: E501,B950
            Colors.CYAN_TEXT + "(status)" + Colors.RESET + f" {self.ctrl_shell_result.return_status}",  # noqa: E501,B950
            Colors.CYAN_TEXT + "(stdout)" + Colors.RESET,
            f"{self.ctrl_shell_result.stdout.decode()}" + Colors.BOLD + Colors.BRIGHT_WHITE_TEXT + f"(Lines: {self.ctrl_shell_result.stdout.count(b"\n")})" + Colors.RESET,  # noqa: E501,B950
            Colors.CYAN_TEXT + "(stderr)" + Colors.RESET,
            f"{self.ctrl_shell_result.stderr.decode()}" + Colors.BOLD + Colors.BRIGHT_WHITE_TEXT + f"(Lines: {self.ctrl_shell_result.stderr.count(b"\n")})" + Colors.RESET  # noqa: E501,B950
        ])
        if leaky and self.asan_outfile is not None:
            with open(self.asan_outfile) as f:
                report += "\n" + f.read()

        return report

    def run(self):
        """Run the instance with the given method."""
        self.setup()
        try:
            self.test_method()
        finally:
            self.teardown()

        print(
            f"{self.test_method.__module__}.{self.test_method.__qualname__}"
            f" {self._report_test()}",
            file=sys.stderr
        )


class ShellTestSuite:
    """Test Suite."""

    def __init__(self, suite: type[ShellTestCase],
                 test_shell: str, control_shell: str):
        """Initialise ShellTestSuite."""
        if not issubclass(suite, ShellTestCase):
            raise TypeError(f"suite must be a subclass of {ShellTestCase}")

        self.test_suite = suite
        self.test_cases = [self.test_suite(case, test_shell, control_shell)
                           for case in self._extract_test_cases()]

    def _extract_test_cases(self) -> list[str]:
        """Extract test cases from a test suite."""
        test_cases: list[str] = list()
        for attr_name, attr in vars(self.test_suite).items():
            if not attr_name.startswith("test"):
                continue

            if not callable(attr):
                continue

            test_cases.append(attr_name)

        return test_cases

    def run(self) -> None:
        """Run all tests in the suite."""
        self.test_suite.class_setup()
        try:
            for test_case in self.test_cases:
                test_case.run()
        finally:
            self.test_suite.class_teardown()
