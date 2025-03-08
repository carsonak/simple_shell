#!/bin/bash
# This script is a test driver for the simple shell project

# set some config variables
R=$RANDOM

TMP_DIR="/tmp"

COMMANDFILE_REAL_SHELL="$TMP_DIR/hbtn_checker_command_real_shell_$R"
COMMANDFILE="$TMP_DIR/hbtn_checker_command_$R"

ENV_TMP_FILE="$TMP_DIR/hbtn_env_$R"

ERROR_OUTFILE="$TMP_DIR/hbtn_checker_error_output_$R"
OUTFILE="$TMP_DIR/hbtn_checker_output_$R"
STATUS="$TMP_DIR/hbtn_checker_status_$R"

EXPECTED_ERROR_OUTFILE="$TMP_DIR/hbtn_checker_expected_error_output_$R"
EXPECTED_OUTFILE="$TMP_DIR/hbtn_checker_expected_output_$R"
EXPECTED_STATUS="$TMP_DIR/hbtn_checker_expected_status_$R"

LTRACE_ALLOWED_FUNCTIONS_FILE="./allowed_functions"
LTRACE_OUTFILE="$TMP_DIR/hbtn_checker_ltrace_output_$R"

ASAN_OUTFILE="$TMP_DIR/hbtn_checker_asan_$R"
ASAN_OPTIONS="detect_leaks=1:detect_stack_use_after_return=true:strict_string_checks=true:replace_str=true:log_path=$ASAN_OUTFILE"

# most used commands, in case the environment is missing.
WHICH="command -v"
CAT=$($WHICH cat)
CHMOD=$($WHICH chmod)
CUT=$($WHICH cut)
DIFF=$($WHICH diff)
ECHO=$($WHICH echo)
ENV=$($WHICH env)
GREP=$($WHICH grep)
HEAD=$($WHICH head)
HEAD=$($WHICH head)
KILLALL=$($WHICH killall)
LTRACE=$($WHICH ltrace)
PIDOF=$($WHICH pidof)
RM=$($WHICH rm)
SED=$($WHICH sed)
TAIL=$($WHICH tail)
TIMEOUT=$($WHICH timeout)
WC=$($WHICH wc)

TIMEOUT_SECONDS=5

# UTILITY FUNCTIONS.
function check_diff()
{
	(( differs=0 ))

	diff_out=$($DIFF "$OUTFILE" "$EXPECTED_OUTFILE")
	diff_err=$($DIFF "$ERROR_OUTFILE" "$EXPECTED_ERROR_OUTFILE")
	diff_status=$($DIFF "$STATUS" "$EXPECTED_STATUS")
	if [[ -n "$diff_out" ]] || [[ -n "$diff_err" ]] || [[ -n "$diff_status" ]]
	then
		(( differs=1 ))
		(( status=1 ))
	fi
}

function cleanup()
{
	stop_process "$HSHELL"
	$RM -f "$COMMANDFILE"
	$RM -f "$COMMANDFILE_REAL_SHELL"
	$RM -f "$STATUS"
	$RM -f "$OUTFILE"
	$RM -f "$ERROR_OUTFILE"
	$RM -f "$ASAN_OUTFILE"
	$RM -f "$LTRACE_OUTFILE"
	$RM -f "$EXPECTED_STATUS"
	$RM -f "$EXPECTED_OUTFILE"
	$RM -f "$EXPECTED_ERROR_OUTFILE"
	$RM -f "$ENV_TMP_FILE"
}

function stop_process()
{
	base_shell=${1##*/}
	running_shells=$($PIDOF "$base_shell" | $WC -l)
	if [[ "$running_shells" -ne "0" ]]
	then
		$KILLALL -9 "$base_shell" > /dev/null 2>&1
	fi
}

function print_diff()
{
	input=$1
	params=$2

	$ECHO -n "(command)["
	if [[ -n "$input" ]]
	then
		$ECHO -e -n "echo \"$input\" | "
	fi

	$ECHO -n "$HSHELL"
	if [[ -n "$params" ]]
	then
		$ECHO -n " $params"
	fi

	$ECHO -e "]\n\n[GOT]:"
	$ECHO "(stdout)[$($CAT "$OUTFILE")](Length: $($WC -c "$OUTFILE"))"
	$ECHO "(stderr)[$($CAT "$ERROR_OUTFILE")](Length: $($WC -c "$ERROR_OUTFILE"))"
	$ECHO -e "(status)[$($CAT "$STATUS")]\n"

	$ECHO "[EXPECTED]:"
	$ECHO "(stdout)[$($CAT "$EXPECTED_OUTFILE")](Length: $($CAT "$EXPECTED_OUTFILE" | $WC -c))"
	$ECHO "(stderr)[$($CAT "$EXPECTED_ERROR_OUTFILE")](Length: $($CAT "$EXPECTED_ERROR_OUTFILE" | $WC -c))"
	$ECHO -e "(status)[$($CAT "$EXPECTED_STATUS")]\n"

	$ECHO "Environment:"
	$CAT "$ENV_TMP_FILE" | $GREP -v "LS_COLORS"
}

function usage()
{
	exitcode=$1

$ECHO "Usage: $0 shell test_file

Options:
--ltrace                      Checks for forbidden system/library calls

--diff                        Force to print the output diff

--help                        Prints the help and exit"

	exit "$exitcode"
}

# parse command-line parameters
usage=0
ltrace=0
force_diff=0
positional_args=()
while [[ $# -gt 0 ]]; do
  case $1 in
		--ltrace)
			(( ltrace=1 ))
			shift
			;;
		--diff)
			(( force_diff=1 ))
			shift
			;;
		-h|--help)
			(( usage=1 ))
			shift
			;;
		--*|-*)
			echo "Unknown option $1" 1>&2
			exit 1
			;;
		*)
      		positional_args+=("$1") # save positional arg
			shift
			;;
	esac
done

set -- "${positional_args[@]}" # restore positional args

[[ "$usage" -eq "1" ]] && usage 0
[[ "$#" -lt "2" ]] && usage 1

HSHELL=$1
TEST_FILE=$2
REAL_SHELL=$($WHICH sh)
stop_process "$HSHELL"
status=0
params=""

# Read and execute test script.
# shellcheck source=/dev/null
source "$TEST_FILE"
# reatrieve shell input (STDIN) AND PARAMS
if [[ -z ${shell_input+x} ]]
then
	echo "The variable \$shell_input is not defined in $TEST_FILE!"
	exit 1
fi

if [[ -n "${shell_params+x}" ]]
then
	params=$shell_params
fi

# function "check_setup" can be defined in the test script
if [[ -n "$(type -t check_setup)" ]] && [[ "$(type -t check_setup)" = function ]]
then
	check_setup
fi

differs=0
# preparing command script file.
{
	$ECHO -e -n "$ECHO \"$shell_input\" | ASAN_OPTIONS=$ASAN_OPTIONS ";
	$ECHO -e "\"$HSHELL\" $params > \"$OUTFILE\" 2> \"$ERROR_OUTFILE\"";
	$ECHO -e "$ECHO -n \$? > \"$STATUS\"";
} > "$COMMANDFILE"
$CHMOD +x "$COMMANDFILE"


# test command against student shell.
# shellcheck source=/dev/null
$TIMEOUT "$TIMEOUT_SECONDS" "$COMMANDFILE"
stop_process "$COMMANDFILE"

# concatenate ASAN output.
true > "$ASAN_OUTFILE"
for log_file in "$ASAN_OUTFILE".*
do $CAT "$log_file" >> "$ASAN_OUTFILE"
	$RM -f "$log_file"
done

# run ltrace
if [[ $ltrace -ne 0 ]]
then
	$ECHO "$shell_input" | $TIMEOUT "$TIMEOUT_SECONDS" "$LTRACE" -bc -o "$LTRACE_OUTFILE" "$HSHELL" "$params" > /dev/null 2>&1 &
	stop_process "$LTRACE"
fi


# function "sh_setup" CAN BE DEFINED IN THE CHECK SCRIPT
if [[ "$(type -t sh_setup)" = function ]]
then sh_setup
fi


# run sh in order to compare outputs
if [[ -z ${compare_with_sh+x} ]] || [[ $compare_with_sh -ne 0 ]]
then
	$ECHO -e -n "$ECHO \"$shell_input\"" > "$COMMANDFILE_REAL_SHELL"
	$ECHO -e -n " | \"$REAL_SHELL\" $params > \"$EXPECTED_OUTFILE\" 2> \"$EXPECTED_ERROR_OUTFILE\" ; "  >> "$COMMANDFILE_REAL_SHELL"
	$ECHO -e "$ECHO -n \$? > \"$EXPECTED_STATUS\"" >> "$COMMANDFILE_REAL_SHELL"
	$CHMOD +x "$COMMANDFILE_REAL_SHELL"

	# shellcheck source=/dev/null
	$TIMEOUT "$TIMEOUT_SECONDS" "$COMMANDFILE_REAL_SHELL"
	stop_process "$COMMANDFILE_REAL_SHELL"

	# replace real shell'S NAME WITH STUDENT'S SHELL NAME ON ERROR OUTPUT
	# example: '/bin/sh: 1: qwerty: not found'
	# becomes: './hsh: 1: qwerty: not found'
	$SED -i "s@${REAL_SHELL}@${HSHELL}@g" "$EXPECTED_OUTFILE"
	$SED -i "s@${REAL_SHELL}@${HSHELL}@g" "$EXPECTED_ERROR_OUTFILE"

	check_diff
fi

# save environment used during correction.
$ENV > "$ENV_TMP_FILE"

# function "check_callback" can be defined in the test script
if [[ "$(type -t check_callback)" = function ]]
then check_callback "$status"
	(( status=$? ))
fi

# print outputs comparison.
if [[ $differs -ne 0 ]] || [[ $force_diff -ne 0 ]]
then print_diff "$shell_input" "$params"
fi

# print asan log if errors were detected
content=$($CAT "$ASAN_OUTFILE")
if [[ -n $content ]]
then $ECHO "$content"
	(( status=1 ))
fi

# check ltrace.
if [[ $ltrace -ne 0 ]]
then
	# parse ltrace output to keep only a list of called functions
	functions_called=$($CAT "$LTRACE_OUTFILE" | $HEAD -n -2 | $TAIL -n +3 | $SED -e 's/  */ /g' | $SED -e 's/^ //g' | $CUT -d' ' -f5 | $GREP -vw 'function' | $GREP -vwe '--------------------' | $GREP -vw 'calls')
	for f in $functions_called
	do
		# check that the function is part of the allowed functions
		if [[ "$($GREP -w "$f" "$LTRACE_ALLOWED_FUNCTIONS_FILE" | $WC -l)" -eq 0 ]]
		then
			$ECHO "Function '$f' is not allowed"
			(( status=1 ))
		fi
	done
fi

# cleanup and quit
cleanup
[[ $status -eq 0 ]] && $ECHO -n "OK"
exit "$status"
