#!/bin/sh
# control.sh - the ABI control: do the checks fail when the ABI is broken?
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: sh abi/control.sh [--cc CMD] [--keep DIR]
#
# Part A builds abi/control/control.c, which applies the checks of
# abi/check.h to deliberately wrong hand-written routines
# (abi/arch/i386_control.h): one that clobbers EBX (and ESI, EDI, EBP),
# leaves DF set, misaligns the stack by 4 before a call, returns a structure
# in registers or without popping the hidden argument, returns no or two
# floating-point values, changes the x87 control word or MXCSR without
# restoring it, leaves a value on the x87 stack, returns only half of a
# 64-bit value, and a signal handler that damages the context it resumes.
# Every scenario runs twice: with the conforming twin the check must PASS
# (so it is not vacuous), with the wrong routine it must FAIL.
#
# Part B runs real tests of the suite through the driver with a compiler
# that is told to generate non-conforming code or layout (gcc options), for
# example structures returned in registers, 8-byte aligned doubles, a 4-byte
# stack alignment; the test must FAIL (or INSPECT, for a disputed assertion)
# there, and PASS with the normal compiler. The entry-state stub is also
# broken on purpose (EDX clobbered, EDX pointing at data, ESP off by 4):
# entry.012 must FAIL for the first two and entry.014 report INSPECT or FAIL. A compiler that rejects an
# option skips that item.
#
# Exit status: 0 if every check detected its fault, 1 if one was missed.

set -u
LC_ALL=POSIX
export LC_ALL

case $0 in
*/*) abi_dir=${0%/*} ;;
*) abi_dir=. ;;
esac
abi_dir=$(cd "$abi_dir" && pwd) || exit 2
root=$(cd "$abi_dir/.." && pwd) || exit 2

base_cc=${CC:-cc}
keep=
while [ $# -gt 0 ]; do
	case $1 in
	--cc) base_cc=$2; shift 2 ;;
	--keep) keep=$2; shift 2 ;;
	*) printf 'control.sh: unknown argument %s\n' "$1" >&2; exit 2 ;;
	esac
done

if [ -n "$keep" ]; then
	mkdir -p "$keep" || exit 2
	tmp=$keep
else
	tmp=$(mktemp -d "${TMPDIR:-/tmp}/abi-control.XXXXXX") || exit 2
	trap 'rm -rf "$tmp"' EXIT HUP INT TERM
fi
cc="$base_cc -m32"

detected=0
missed=0
skipped=0
note() { printf '%-9s %s\n' "$1" "$2"; }

# ---------- Part A ----------

if ! $cc -D_XOPEN_SOURCE=800 -I "$root/core" -c -o "$tmp/pc.o" "$root/core/pc.c" 2>"$tmp/pc.err"; then
	printf 'control.sh: cannot build the test helper (see %s)\n' "$tmp/pc.err" >&2
	exit 2
fi
if ! $cc -D_POSIX_C_SOURCE=202405L -D_GNU_SOURCE -I "$root/core" -o "$tmp/control" \
    "$abi_dir/control/control.c" "$tmp/pc.o" -lpthread 2>"$tmp/control.err"; then
	printf 'control.sh: cannot build the control program (see %s)\n' "$tmp/control.err" >&2
	cat "$tmp/control.err" >&2
	exit 2
fi

# result of one run: the code on the RESULT line, or NONE
result_of() { # scenario good|bad [which]
	: >"$tmp/r"
	PC_RESULT=$tmp/r ABI_CTL_WHICH=${3:-} "$tmp/control" "$1" "$2" >"$tmp/out" 2>&1 </dev/null
	set -- $(grep '^RESULT' "$tmp/r" | tail -n 1)
	printf '%s\n' "${2:-NONE}"
}

scenario() { # name [which...]
	sc=$1
	shift
	[ $# -gt 0 ] || set -- ""
	g=$(result_of "$sc" good)
	case $g in
	UNSUPPORTED | NOTINUSE) note SKIP "A $sc (cannot be built here: $g)"; skipped=$((skipped + 1)); return ;;
	PASS) ;;
	*) note BROKEN "A $sc: the conforming twin gave $g, not PASS (see $tmp/out)"; missed=$((missed + 1)); return ;;
	esac
	for w in "$@"; do
		b=$(result_of "$sc" bad "$w")
		if [ "$b" = FAIL ]; then
			note DETECTED "A $sc${w:+ ($w)}: twin PASS, wrong routine FAIL"
			detected=$((detected + 1))
		else
			note MISSED "A $sc${w:+ ($w)}: the wrong routine gave $b, not FAIL"
			missed=$((missed + 1))
		fi
	done
}

for s in saved-ebx saved-esi saved-edi saved-ebp df-set x87-cw-changed mxcsr-changed x87-leak x87-not-popped \
    fp-none fp-two struct-in-regs struct-no-pop ll-low-only stack-misaligned-by-primitive \
    stack-misaligned-by-caller df-set-at-call resume-regs resume-x87-cw; do
	scenario "$s"
done
scenario record-resume regs cw mxcsr df

# ---------- Part B ----------

verdict() { # ccscript id -> the code the driver logs for that assertion, or ABORT
	d=$tmp/run.$$
	rm -rf "$d"
	"$root/posixcheck.sh" --cc "$1" --root "$abi_dir/modules" --no-selftest --only "$2" \
	    --results "$d" >"$tmp/drv.out" 2>"$tmp/drv.err"
	rc=$?
	if [ "$rc" -eq 3 ] || [ "$rc" -eq 2 ]; then
		printf 'ABORT\n'
		return
	fi
	set -- $(grep -v '^SUMMARY' "$tmp/drv.out" | grep -v '^posixcheck' | tail -n 1)
	printf '%s\n' "${1:-NONE}"
}

fault() { # "gcc options" assertion why
	opts=$1 id=$2 why=$3
	printf '#!/bin/sh\nexec %s -m32 %s "$@"\n' "$base_cc" "$opts" >"$tmp/ccx"
	chmod +x "$tmp/ccx"
	printf '#!/bin/sh\nexec %s -m32 "$@"\n' "$base_cc" >"$tmp/ccn"
	chmod +x "$tmp/ccn"
	n=$(verdict "$tmp/ccn" "$id")
	if [ "$n" != PASS ]; then
		note SKIP "B $id: the normal build gave $n, so there is nothing to compare ($why)"
		skipped=$((skipped + 1))
		return
	fi
	f=$(verdict "$tmp/ccx" "$id")
	case $f in
	FAIL | INSPECT)
		note DETECTED "B $id: normal PASS, with '$opts' $f ($why)"
		detected=$((detected + 1))
		;;
	ABORT | UNINITIATED)
		note SKIP "B $id: the compiler rejects '$opts'"
		skipped=$((skipped + 1))
		;;
	*)
		note MISSED "B $id: with '$opts' the verdict was $f ($why)"
		missed=$((missed + 1))
		;;
	esac
}

fault -malign-double ABI.i386.types.002 "long long aligned to 8"
fault -malign-double ABI.i386.types.004 "double aligned to 8"
fault -mlong-double-64 ABI.i386.types.005 "long double as wide as double"
fault -malign-double ABI.i386.types.012 "aggregates laid out with 8-byte doubles"
fault -freg-struct-return ABI.i386.calling.014 "small structures returned in registers"
fault -mpreferred-stack-boundary=2 ABI.i386.calling.011 "stack aligned to 4 at calls"
fault -mno-fp-ret-in-387 ABI.i386.calling.017 "float not returned in st0"
fault -mno-fp-ret-in-387 ABI.i386.calling.018 "double not returned in st0"
fault "-fcall-used-esi -fcall-used-edi -fcall-used-ebx" ABI.i386.calling.026 "compiled code clobbers callee-saved registers"
fault -DABI_ENTRY_FAULT=1 ABI.i386.entry.012 "EDX clobbered before it is recorded: not a code address"
fault -DABI_ENTRY_FAULT=2 ABI.i386.entry.012 "EDX points at data, not at code"
fault -DABI_ENTRY_FAULT=3 ABI.i386.entry.014 "ESP recorded 4 lower: misaligned"

printf 'control: %d detected, %d missed, %d skipped\n' "$detected" "$missed" "$skipped"
[ "$missed" -eq 0 ]
