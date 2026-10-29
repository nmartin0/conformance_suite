#!/bin/sh
# run.sh - run only the conventions suite (conv/modules) with the driver.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: sh conv/run.sh [driver options] [module ...]
#
# Same options as posixcheck.sh (--results DIR, --only ID, --no-selftest,
# ...); modules are named conv/xdg, conv/layout, and so on. Results are
# reported with the driver's codes: PASS, FAIL (a FORMAL specification is
# not met), INSPECT (a CONVENTION is not followed: a person judges),
# UNRESOLVED, NOTINUSE.
#
# The driver gives each test a clean environment and passes only PC_*
# variables, so the few variables the checks need from the caller are copied:
#   XDG_RUNTIME_DIR -> PC_CONV_XDG_RUNTIME_DIR
#   TZDIR           -> PC_CONV_TZDIR
# Variables PC_CONV_* that are already set win (conv/neg/run.sh uses them to
# point a check at a deliberately wrong fixture). They are listed in
# conv/README.md.

case $0 in
*/*) here=${0%/*} ;;
*) here=. ;;
esac
top=$(cd "$here/.." && pwd) || exit 2

if [ -z "${PC_CONV_XDG_RUNTIME_DIR:-}" ] && [ -n "${XDG_RUNTIME_DIR:-}" ]; then
	PC_CONV_XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR
	export PC_CONV_XDG_RUNTIME_DIR
fi
if [ -z "${PC_CONV_TZDIR:-}" ] && [ -n "${TZDIR:-}" ]; then
	PC_CONV_TZDIR=$TZDIR
	export PC_CONV_TZDIR
fi
exec sh "$top/posixcheck.sh" --root "$top/conv/modules" ${1+"$@"}
