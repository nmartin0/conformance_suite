# pc.sh - shell test helper for posixcheck. Source it; do not run it.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Uses shell built-ins only (printf, [, ., exit), so a test of a utility
# never depends on another utility working.
#
#     . "$PC_CORE/pc.sh"
#     pc_begin XCU.true.001     setup phase: problems are UNRESOLVED
#     ...
#     pc_test                   test phase: problems are FAIL
#     true; rc=$?
#     [ "$rc" -eq 0 ] || pc_fail "exit status $rc"
#     pc_pass
#
# Call the result functions from the main shell, not from a subshell or a
# command substitution: they end the test with exit.

pc__id=?
pc__phase=setup

# "command" skips shell functions, so a mutant that shadows printf or [
# (when those utilities are under test) cannot disturb the helper itself.
pc__out() {
	if command [ -n "${PC_RESULT:-}" ]; then
		command printf '%s\n' "$*" >>"$PC_RESULT"
	else
		command printf '%s\n' "$*"
	fi
}

pc__result() {
	pc__code=$1
	shift
	PC_PHASE=internal
	pc__out "RESULT $pc__code $pc__id $*"
	exit 0
}

pc_begin() {
	pc__id=$1
	pc__phase=setup
	PC_PHASE=setup
	export PC_PHASE
	pc__out "PHASE setup"
}

# Enter the test phase. In CI, a shell mutant (function definitions that
# shadow a utility) is loaded here, so it cannot disturb setup.
pc_test() {
	pc__phase=test
	PC_PHASE=test
	pc__out "PHASE test"
	if command [ -n "${PC_MUTANT_FILE:-}" ]; then
		. "$PC_MUTANT_FILE"
	fi
}

pc_pass() { pc__result PASS "$*"; }
pc_unresolved() { pc__result UNRESOLVED "$*"; }
pc_notinuse() { pc__result NOTINUSE "$*"; }
pc_unsupported() { pc__result UNSUPPORTED "$*"; }

# FAIL in the test phase; UNRESOLVED (with "setup:") in the setup phase.
pc_fail() {
	if command [ "$pc__phase" = test ]; then
		pc__result FAIL "$*"
	else
		pc__result UNRESOLVED "setup: $*"
	fi
}

# pc_check <message> <command...>: run the command; fail if it fails.
pc_check() {
	pc__msg=$1
	shift
	"$@" || pc_fail "$pc__msg"
}

pc_note() { command printf 'note: %s\n' "$*" >&2; }
