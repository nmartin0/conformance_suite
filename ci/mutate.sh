#!/bin/sh
# mutate.sh - check that every test catches the faults it declares and
# tolerates the variants it declares (CI).
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: ci/mutate.sh [--sh SHELL] [--cc CC] [--profile FILE]
#                     [--faults-only | --variants-only]
#                     [--cache] [-j N] [--tries N] [--root DIR]
#                     [ID-pattern ...]
#
# Runs off-system on a Linux or BSD machine with LD_PRELOAD. For each test
# that declares MUTANT lines (ci/lint.py --list-mutants):
#   1. run the test normally (the baseline)
#   2. run it once per mutant, with the planted fault active
# A mutant is CAUGHT when the baseline is PASS and the mutant run is FAIL
# (or INSPECT, a disputed assertion's FAIL).
# Anything else is reported; MISSED (the test passed with the fault in
# place) makes the script exit 1. When the baseline is not PASS on this
# machine, the mutant cannot be judged here and is reported UNCHECKED; so
# is a mutant whose shim reports "pcmutant: cannot plant" in the test's
# output (the fault needs something this system lacks, such as /proc).
#
# Then the other direction. For each VARIANT a test declares (behaviour the
# standard allows that the reference systems may not show, planted the same
# way), the test must still PASS:
#   TOLERATED  the variant run is PASS
#   REJECTED   the variant run is FAIL or INSPECT: the test asks for more
#              than the text does (exit 1)
#   UNDECIDED  any other result (UNRESOLVED and the like): the test cannot
#              judge a conforming system that behaves this way; reported,
#              not fatal, but each one needs a look
#   UNCHECKED  as for faults
#   SYSTEM     the variant is a build shim (a header alone marked
#              "posixcheck: shim": it only supplies declarations or names
#              the system's headers leave out, so what runs is the system's
#              own behaviour) and its run is FAIL or INSPECT: a verdict on
#              this system, not on the test; reported, not fatal, and it
#              belongs in known/
# A rejected variant is a bug in the test, or the variant does not model
# what the text allows; it is never resolved by deleting the variant
# because a reference system does otherwise.
#
# Speed (both off by default; the defaults are the reference behaviour):
#   --cache  the runs share compiled helpers, tests and libraries
#            (posixcheck.sh --cache), within this invocation only.
#   -j N     up to N assertions at a time, but only assertions whose tests
#            can neither be disturbed by nor disturb another test: any test
#            that measures time, sleeps or waits with a timeout, signals
#            other processes, uses per-user or system-wide resources, named
#            IPC, another user ID, or is a shell test, runs alone. When in
#            doubt a test runs alone.
# Each assertion's baseline is run once, before all its faults and variants
# (three times if it did not pass). An anomaly (MISSED, REJECTED, UNDECIDED, SYSTEM)
# is rerun, --tries runs in all (default 5); if the runs disagree it is
# FLAKY: no verdict, and the check fails. --root DIR checks another modules
# tree (ci/selftest-mutate.sh uses a fixture with known answers).
# A READING (a variant planting the other reading of a disputed assertion)
# is expected to PASS or log INSPECT: READING lines report the INSPECTs.
# When the baseline cannot run here (UNINITIATED, NOTINUSE, UNRESOLVED), the
# variants still run, since a variant may supply what this system lacks;
# and a fault is planted on a header-only variant when one makes the test
# buildable (the verdict line says "under" which).
# Several patterns select the union.

set -u
here=$(cd "$(dirname "$0")/.." && pwd)
sh=sh
cc=
profile=
patterns=
do_faults=yes do_variants=yes
cache=no
jobs=1
tries=5
modroot=
while [ $# -gt 0 ]; do
	case $1 in
	--sh) sh=$2; shift 2 ;;
	--cc) cc=$2; shift 2 ;;
	--profile) profile=$2; shift 2 ;;
	--faults-only) do_variants=no; shift ;;
	--variants-only) do_faults=no; shift ;;
	--cache) cache=yes; shift ;;
	-j) jobs=$2; shift 2 ;;
	--tries) tries=$2; shift 2 ;;
	--root) modroot=$2; shift 2 ;;
	*) patterns="$patterns
$1"; shift ;;
	esac
done
[ -n "$patterns" ] || patterns="
*"
case $jobs in '' | *[!0-9]* | 0) echo "mutate.sh: -j needs a positive number" >&2; exit 2 ;; esac
case $tries in '' | *[!0-9]* | 0) echo "mutate.sh: --tries needs a positive number" >&2; exit 2 ;; esac
[ -n "$modroot" ] || modroot=$here/modules
case $modroot in /*) ;; *) modroot=$PWD/$modroot ;; esac

# A private work directory (mkdir is atomic; no mktemp, which POSIX lacks).
n=0
while :; do
	work=${TMPDIR:-/tmp}/pcmutate.$$.$n
	mkdir -m 700 "$work" 2>/dev/null && break
	n=$((n + 1))
	[ "$n" -lt 100 ] || { echo "mutate.sh: cannot create a work directory" >&2; exit 2; }
done
trap 'rm -rf "$work"' EXIT
trap 'exit 130' INT TERM
mkdir "$work/res" "$work/grp" || exit 2

selected() { # id: true if it matches one of the patterns
	sel_r=1
	sel_ifs=$IFS
	IFS='
'
	set -f
	for sel_p in $patterns; do
		[ -n "$sel_p" ] || continue
		case $1 in $sel_p) sel_r=0; break ;; esac
	done
	set +f
	IFS=$sel_ifs
	return $sel_r
}

code_of() { # results-dir id
	if [ -f "$1/state/$2" ]; then cat "$1/state/$2"; else echo NONE; fi
}

note_of() { # results-dir id: the note logged for id
	awk -F '\t' -v id="$2" '$2 == id { print $4 }' "$1/run.log" 2>/dev/null
}

run() { # results-dir id [--mutant|--variant name]
	# Its own names: callers keep their r and id.
	run_r=$1 run_id=$2
	shift 2
	# ${1+"$@"}: posh treats an empty "$@" as unset under set -u
	if [ -n "$cc" ]; then set -- ${1+"$@"} --cc "$cc"; fi
	if [ -n "$profile" ]; then set -- ${1+"$@"} --profile "$profile"; fi
	if [ "$cache" = yes ]; then set -- ${1+"$@"} --cache "$work/cache"; fi
	"$sh" "$here/posixcheck.sh" --no-selftest --root "$modroot" --results "$run_r" --only "$run_id" \
	    ${1+"$@"} >/dev/null 2>&1 </dev/null
}

# ---------- the plan: every selected pair, numbered in report order ----------

python3 "$here/ci/lint.py" --modules "$modroot" --list-mutants >"$work/mpairs" || exit 2
python3 "$here/ci/lint.py" --modules "$modroot" --list-variants >"$work/vpairs" || exit 2
[ "$do_faults" = yes ] || : >"$work/mpairs"
[ "$do_variants" = yes ] || : >"$work/vpairs"
: >"$work/plan"
i=0
while read -r id name; do
	selected "$id" || continue
	i=$((i + 1))
	printf '%s M %s %s\n' "$i" "$id" "$name" >>"$work/plan"
done <"$work/mpairs"
nm=$i
while read -r id name flag; do
	selected "$id" || continue
	i=$((i + 1))
	k=V
	[ "${flag:-}" = reading ] && k=P
	[ "${flag:-}" = shim ] && k=S
	printf '%s %s %s %s\n' "$i" "$k" "$id" "$name" >>"$work/plan"
done <"$work/vpairs"
total=$i
nv=$((total - nm))

# Assertions in order of first appearance, each with its pairs.
awk -v w="$work" '{ if (!($3 in g)) { g[$3] = ++n; print n, $3 >>(w "/groups"); close(w "/groups") }
       f = w "/grp/" g[$3]; print $1, $2, $4 >>f; close(f) }' "$work/plan"
[ -f "$work/groups" ] || : >"$work/groups"

# Which tests may share the machine: the test file of each assertion.
if [ "$jobs" -gt 1 ]; then
	for a in "$modroot"/*/*/*.asr; do
		awk -v d="${a%/*}" '/^assertion / { id = $2 } /^test / { print id, d "/" $2 }' "$a"
	done >"$work/tests"
fi
# A test runs alone unless it is a C test that shows none of these.
EXCLUSIVE='sleep|alarm|timer|itimer|clock|now_ms|_ms[ (]|time[ (]|times[ (]|gettimeofday|poll|select|timed|deadline|wait|kill|raise|sigqueue|signal|sigaction|SIG|pthread|fork|exec|system|popen|spawn|setuid|seteuid|setgid|unprivileged|65534|getpwnam|rlimit|RLIMIT|shm|mq_|sem_|msg|fifo|mkfifo|socket|aio_|sched|nice|priority|ENFILE|EMFILE|OPEN_MAX|STREAM_MAX|sysconf|tty|pty|/dev/|/proc|getpid|getppid|chroot'
alone() { # id: true if its test must run alone
	[ "$jobs" -gt 1 ] || return 0
	t=$(awk -v id="$1" '$1 == id { print $2; exit }' "$work/tests")
	case $t in *.c) ;; *) return 0 ;; esac
	[ -f "$t" ] || return 0
	grep -Eq "$EXCLUSIVE" "$t"
}

# ---------- one assertion: its baseline once, then each of its pairs ----------

# Classify one pair: writes "CATEGORY<tab>line" to $work/res/$i. Uses the
# group's id, base, hv and hbase.
pair_once() { # i kind name
	i=$1 kind=$2 name=$3
	r=$work/run.$i
	[ "$kind" = M ] && uk=M || uk=V
	if [ "$base" != PASS ]; then
		if [ "$kind" = M ] && [ "$base" = UNINITIATED ]; then
			# The reference systems lack what the test needs: plant the
			# fault on a header-only variant that supplies it, if any.
			if [ -z "$hv" ]; then
				hv=$(header_variant "$g")
				hbase=NONE
				if [ -n "$hv" ]; then
					run "$work/hb.$g" "$id" --variant "$hv"
					hbase=$(code_of "$work/hb.$g" "$id")
				fi
			fi
			if [ -n "$hv" ] && [ "$hbase" = PASS ]; then
				run "$r" "$id" --variant "$hv" --mutant "$name"
				got=$(code_of "$r" "$id")
				case $got in
				FAIL | INSPECT) printf 'C\tCAUGHT    %-40s %-28s (under %s) %s\n' "$id" "$name" "$hv" "$(note_of "$r" "$id")" >"$work/res/$i" ;;
				PASS) printf 'X\tMISSED    %-40s %-28s mutant run under %s gave PASS\n' "$id" "$name" "$hv" >"$work/res/$i" ;;
				*) printf 'UM\tUNCHECKED %-40s %-28s baseline %s; under %s the mutant run gave %s\n' "$id" "$name" "$base" "$hv" "$got" >"$work/res/$i" ;;
				esac
				rm -rf "$r"
				return 0
			fi
		fi
		case $kind:$base in
		[VPS]:UNINITIATED | [VPS]:NOTINUSE | [VPS]:UNRESOLVED)
			# The reference system cannot run the test; a variant may
			# supply what it lacks, and its verdict still counts.
			run "$r" "$id" --variant "$name"
			got=$(code_of "$r" "$id")
			case $kind:$got in
			*:PASS) printf 'T\tTOLERATED %-40s %-28s (baseline %s)\n' "$id" "$name" "$base" >"$work/res/$i" ;;
			P:INSPECT) printf 'E\tREADING   %-40s %-28s (baseline %s) INSPECT: %s\n' "$id" "$name" "$base" "$(note_of "$r" "$id")" >"$work/res/$i" ;;
			S:FAIL | S:INSPECT) printf 'Y\tSYSTEM    %-40s %-28s (shim; baseline %s) %s: %s\n' "$id" "$name" "$base" "$got" "$(note_of "$r" "$id")" >"$work/res/$i" ;;
			*:FAIL | V:INSPECT) printf 'R\tREJECTED  %-40s %-28s (baseline %s) %s: %s\n' "$id" "$name" "$base" "$got" "$(note_of "$r" "$id")" >"$work/res/$i" ;;
			*) printf 'UV\tUNCHECKED %-40s %-28s baseline %s, variant run %s\n' "$id" "$name" "$base" "$got" >"$work/res/$i" ;;
			esac
			rm -rf "$r"
			return 0
			;;
		esac
		printf 'U%s\tUNCHECKED %-40s %-28s baseline %s\n' "$uk" "$id" "$name" "$base" >"$work/res/$i"
		return 0
	fi
	if [ "$kind" = M ]; then
		run "$r" "$id" --mutant "$name"
		got=$(code_of "$r" "$id")
		if [ "$got" = INSPECT ]; then
			case $(note_of "$r" "$id") in disputed*) ;; *) got="INSPECT (not disputed)" ;; esac
		fi
		if [ "$got" != FAIL ] && [ "$got" != INSPECT ] &&
		    grep -q 'pcmutant: cannot plant' "$r/out/$id.txt" 2>/dev/null; then
			printf 'UM\tUNCHECKED %-40s %-28s fault cannot be planted here\n' "$id" "$name" >"$work/res/$i"
		# A disputed assertion logs its FAIL as INSPECT; that counts too.
		elif [ "$got" = FAIL ] || [ "$got" = INSPECT ]; then
			printf 'C\tCAUGHT    %-40s %-28s %s\n' "$id" "$name" "$(note_of "$r" "$id")" >"$work/res/$i"
		else
			printf 'X\tMISSED    %-40s %-28s mutant run gave %s\n' "$id" "$name" "$got" >"$work/res/$i"
		fi
	else
		run "$r" "$id" --variant "$name"
		got=$(code_of "$r" "$id")
		if [ "$got" != PASS ] && grep -q 'pcmutant: cannot plant' "$r/out/$id.txt" 2>/dev/null; then
			printf 'UV\tUNCHECKED %-40s %-28s variant cannot be planted here\n' "$id" "$name" >"$work/res/$i"
			rm -rf "$r"
			return 0
		fi
		case $kind:$got in
		*:PASS) printf 'T\tTOLERATED %-40s %-28s\n' "$id" "$name" >"$work/res/$i" ;;
		# A READING plants the other reading of a disputed assertion:
		# INSPECT is the expected answer, a plain FAIL is not.
		P:INSPECT) printf 'E\tREADING   %-40s %-28s INSPECT: %s\n' "$id" "$name" "$(note_of "$r" "$id")" >"$work/res/$i" ;;
		S:FAIL | S:INSPECT) printf 'Y\tSYSTEM    %-40s %-28s (shim) %s: %s\n' "$id" "$name" "$got" "$(note_of "$r" "$id")" >"$work/res/$i" ;;
		*:FAIL | V:INSPECT) printf 'R\tREJECTED  %-40s %-28s %s: %s\n' "$id" "$name" "$got" "$(note_of "$r" "$id")" >"$work/res/$i" ;;
		*) printf 'D\tUNDECIDED %-40s %-28s %s: %s\n' "$id" "$name" "$got" "$(note_of "$r" "$id")" >"$work/res/$i" ;;
		esac
	fi
	rm -rf "$r"
}

catnames() { # categories...: their names, space-separated
	cn=
	for cc in "$@"; do
		case $cc in
		C) cc=CAUGHT ;; X) cc=MISSED ;; R) cc=REJECTED ;; T) cc=TOLERATED ;;
		D) cc=UNDECIDED ;; E) cc=READING ;; UM | UV) cc=UNCHECKED ;; Y) cc=SYSTEM ;;
		esac
		cn="${cn:+$cn }$cc"
	done
	printf '%s\n' "$cn"
}

cat_of() { # file: its category
	IFS='	' read -r cat_c cat_rest <"$1" || cat_c=
	printf '%s\n' "$cat_c"
}

# An anomaly (MISSED, REJECTED, UNDECIDED, SYSTEM) is never reported on one run:
# the pair is run again, up to $tries times in all. If the runs disagree the
# pair is FLAKY (reported, and a failure of the check): the test or the fault
# depends on timing or the environment, and no verdict is given.
pair_checked() { # i kind name
	pair_once "$1" "$2" "$3"
	pc_first=$(cat_of "$work/res/$1")
	case $pc_first in X | R | D | Y) ;; *) return 0 ;; esac
	cp "$work/res/$1" "$work/res/$1.first"
	pc_seen=$pc_first pc_n=1
	while [ "$pc_n" -lt "$tries" ]; do
		pc_n=$((pc_n + 1))
		pair_once "$1" "$2" "$3"
		pc_seen="$pc_seen $(cat_of "$work/res/$1")"
	done
	pc_odd=no
	for pc_c in $pc_seen; do [ "$pc_c" = "$pc_first" ] || pc_odd=yes; done
	if [ "$pc_odd" = yes ]; then
		printf 'F\tFLAKY     %-40s %-28s %d runs disagree (%s; first: %s)\n' "$id" "$3" "$tries" \
		    "$(catnames $pc_seen)" \
		    "$(cut -f2- "$work/res/$1.first" | awk '{ $1 = ""; print }' | cut -c1-120)" >"$work/res/$1"
	else
		mv "$work/res/$1.first" "$work/res/$1"
	fi
	rm -f "$work/res/$1.first"
}

do_group() { # group-number id
	g=$1 id=$2
	run "$work/base.$g" "$id"
	base=$(code_of "$work/base.$g" "$id")
	# A baseline that did not pass is run again: one that sometimes passes
	# would hide its faults as UNCHECKED.
	case $base in
	FAIL | UNRESOLVED | INSPECT)
		pc_b=1
		while [ "$pc_b" -lt 3 ]; do
			pc_b=$((pc_b + 1))
			run "$work/base.$g.$pc_b" "$id"
			pc_bb=$(code_of "$work/base.$g.$pc_b" "$id")
			rm -rf "$work/base.$g.$pc_b"
			[ "$pc_bb" = "$base" ] || base="FLAKY ($base, then $pc_bb)"
		done
		;;
	esac
	hv= hbase=
	while read -r gi gkind gname; do
		case $base in
		FLAKY*)
			printf 'F\tFLAKY     %-40s %-28s baseline %s\n' "$id" "$gname" "$base" >"$work/res/$gi"
			continue ;;
		esac
		pair_checked "$gi" "$gkind" "$gname"
	done <"$work/grp/$g"
	rm -rf "$work/base.$g" "$work/hb.$g"
}

header_variant() { # group: the first header-only variant this assertion declares, if any
	while read -r hi hk hn; do
		case $hk in V | S) ;; *) continue ;; esac
		hd=$modroot/xsh/${hn%.*}/variants
		[ -f "$hd/$hn.h" ] || continue
		set -- "$hd"/*.c
		[ -f "$1" ] && continue
		printf '%s\n' "$hn"
		return 0
	done <"$work/grp/$1"
}

# ---------- run: groups that may share run in batches of up to N ----------

running=0
while read -r g id; do
	if alone "$id"; then
		wait
		running=0
		do_group "$g" "$id"
	else
		do_group "$g" "$id" &
		running=$((running + 1))
		if [ "$running" -ge "$jobs" ]; then
			wait
			running=0
		fi
	fi
done <"$work/groups"
wait

# ---------- report, in plan order ----------

caught=0 missed=0 unchecked=0 tolerated=0 rejected=0 undecided=0 vunchecked=0 readings=0 flaky=0 system=0
i=0
while [ "$i" -lt "$total" ]; do
	i=$((i + 1))
	if [ ! -f "$work/res/$i" ]; then
		echo "mutate.sh: no result for pair $i" >&2
		exit 2
	fi
	IFS='	' read -r cat line <"$work/res/$i"
	printf '%s\n' "$line"
	case $cat in
	C) caught=$((caught + 1)) ;;
	X) missed=$((missed + 1)) ;;
	UM) unchecked=$((unchecked + 1)) ;;
	T) tolerated=$((tolerated + 1)) ;;
	R) rejected=$((rejected + 1)) ;;
	D) undecided=$((undecided + 1)) ;;
	UV) vunchecked=$((vunchecked + 1)) ;;
	E) readings=$((readings + 1)) ;;
	F) flaky=$((flaky + 1)) ;;
	Y) system=$((system + 1)) ;;
	esac
done

printf '\nmutate: %d mutants: %d caught, %d missed, %d unchecked\n' \
    "$nm" "$caught" "$missed" "$unchecked"
printf 'mutate: %d variants: %d tolerated, %d rejected, %d undecided, %d unchecked\n' \
    "$nv" "$tolerated" "$rejected" "$undecided" "$vunchecked"
[ "$readings" -eq 0 ] ||
    printf 'mutate: %d disputed readings logged INSPECT as expected\n' "$readings"
[ "$system" -eq 0 ] ||
    printf 'mutate: %d SYSTEM: build-shim runs that fail, verdicts on this system (record in known/)\n' "$system"
[ "$flaky" -eq 0 ] ||
    printf 'mutate: %d FLAKY: runs disagreed, no verdict (each needs a look)\n' "$flaky"
[ "$missed" -eq 0 ] && [ "$rejected" -eq 0 ] && [ "$flaky" -eq 0 ]
