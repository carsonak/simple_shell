#!/bin/bash
# Execute `/bin/ls` 3 times

shell_input="/bin/ls
/bin/ls
/bin/ls"

shell_params=""

function check_setup()
{
	return 0
}

function sh_setup()
{
	return 0
}

function check_callback()
{
	status=$1

	return "$status"
}
