#!/bin/sh
# posixcheck.sh - run the posixcheck conformance suite on this system.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: ./posixcheck.sh [options] [module ...]
#
#   module            a module path such as xsh/open; default: all modules
#   --results DIR     keep everything the run produces in DIR (for debugging;
#                     without it the run uses a private temporary directory
#                     that is removed when the run ends or is interrupted)
#   --resume          continue an interrupted run in an existing DIR
#   --only ID         run just this assertion (prerequisites still run)
#   --option CODE     run only modules whose option is CODE (repeatable)
#   --profile FILE    implementation-defined choices and environment
#   --cc CC           C compiler (default: c17 if present, else cc)
#   --sut-sh SH       shell the tests run under (default: sh on _CS_PATH)
#   --no-selftest     skip the harness self-tests
#   --selftest        run only the harness self-tests
#   --root DIR        modules directory (default: the suite's modules/)
#   --mutant NAME     CI only: run with a planted fault (needs --only)
#   --variant NAME    CI only: run with a conforming variant, behaviour the
#                     standard allows planted the same way (needs --only;
#                     with --mutant only if the variant is a header alone)
#   --cache DIR       CI only: reuse compiled helpers, tests and libraries
#                     kept in DIR by earlier runs (ci/mutate.sh)
#
# Needs only this shell, a C compiler and mkdir. Prints one line per
# assertion and a final SUMMARY line on standard output (redirect it to
# keep it); diagnostics go to standard error. Nothing is left behind unless
# --results DIR is given, which keeps DIR/run.log and the test output.
# The exit status is 1 if any assertion FAILed (without --results), 3 for
# an aborted run, 2 for a usage error. Run it with elevated privilege (root,
# su, sudo, pfexec); as an ordinary user it still runs, and the assertions
# that need root are skipped and reported as such (UNRESOLVED, "skipped").

set -u
LC_ALL=POSIX
export LC_ALL

pc_version=0.1.0

pc_die() {
	printf 'posixcheck: %s\n' "$*" >&2
	exit 2
}

pc_warn() {
	printf 'posixcheck: warning: %s\n' "$*" >&2
}

# Stop the run. A wrong log is worse than no log.
pc_abort() {
	if [ -n "${pc_mainlog:-}" ] && [ -f "$pc_mainlog" ]; then
		printf '# ABORT %s\n' "$*" >>"$pc_mainlog"
	fi
	printf 'posixcheck: ABORT %s\n' "$*" >&2
	exit 3
}

# ---------- locate the suite and parse arguments ----------

pc_start=$PWD
case $0 in
*/*) pc_root=${0%/*} ;;
*) pc_root=. ;;
esac
cd "$pc_root" || pc_die "cannot cd to $pc_root"
pc_root=$PWD
cd "$pc_start" || pc_die "cannot cd back to $pc_start"

pc_abs() {
	case $1 in
	/*) pc_absv=$1 ;;
	*) pc_absv=$pc_start/$1 ;;
	esac
}

pc_results=
pc_ephemeral=no
pc_resume=no
pc_only=
pc_sel_options=
pc_profile=
pc_cc=${CC:-}
pc_sut_sh=
pc_selftest=yes
pc_selftest_only=no
pc_modroot=$pc_root/modules
pc_mutant=
pc_variant=
pc_cache=
pc_args=

pc_need_arg() {
	[ "$1" -ge 2 ] || pc_die "option $2 needs an argument"
}

while [ $# -gt 0 ]; do
	case $1 in
	--results) pc_need_arg $# "$1"; pc_results=$2; shift 2 ;;
	--resume) pc_resume=yes; shift ;;
	--only) pc_need_arg $# "$1"; pc_only=$2; shift 2 ;;
	--option) pc_need_arg $# "$1"; pc_sel_options="$pc_sel_options $2"; shift 2 ;;
	--profile) pc_need_arg $# "$1"; pc_profile=$2; shift 2 ;;
	--cc) pc_need_arg $# "$1"; pc_cc=$2; shift 2 ;;
	--sut-sh) pc_need_arg $# "$1"; pc_sut_sh=$2; shift 2 ;;
	--no-selftest) pc_selftest=no; shift ;;
	--selftest) pc_selftest_only=yes; shift ;;
	--root) pc_need_arg $# "$1"; pc_modroot=$2; shift 2 ;;
	--mutant) pc_need_arg $# "$1"; pc_mutant=$2; shift 2 ;;
	--variant) pc_need_arg $# "$1"; pc_variant=$2; shift 2 ;;
	--cache) pc_need_arg $# "$1"; pc_cache=$2; shift 2 ;;
	-h | --help)
		while IFS= read -r pc_l; do
			case $pc_l in
			'#!'*) ;;
			'#'*) pc_l=${pc_l#\#}; printf '%s\n' "${pc_l# }" ;;
			*) break ;;
			esac
		done <"$pc_root/posixcheck.sh"
		exit 0
		;;
	--) shift; break ;;
	-*) pc_die "unknown option $1 (try --help)" ;;
	*) pc_args="$pc_args $1"; shift ;;
	esac
done
# A loop over "$@" with no arguments trips "set -u" in some older shells
# (posh), though POSIX.1-2024 exempts @ from -u; shifting avoids it.
while [ $# -gt 0 ]; do pc_args="$pc_args $1"; shift; done

# No --results: work in a private directory that is removed at the end.
if [ -z "$pc_results" ]; then
	[ "$pc_resume" = yes ] && pc_die "--resume needs --results DIR"
	pc_tb=${TMPDIR:-/tmp}
	pc_n=0
	while :; do
		pc_results=$pc_tb/posixcheck.$$.$pc_n
		(umask 077; mkdir "$pc_results") 2>/dev/null && break
		pc_n=$((pc_n + 1))
		[ "$pc_n" -lt 100 ] || pc_die "cannot create a private directory in $pc_tb"
	done
	pc_ephemeral=yes
	pc_cleanup() {
		trap '' EXIT HUP INT TERM
		cd / || return 0
		# A test that failed while a directory was unreadable or unsearchable
		# would defeat rm -rf for an ordinary user: restore owner access first.
		find "$pc_results" -type d -exec chmod u+rwx {} + >/dev/null 2>&1
		rm -rf "$pc_results"
	}
	trap 'pc_cleanup' EXIT
	trap 'exit 130' HUP INT TERM
fi
pc_abs "$pc_results"; pc_results=$pc_absv
pc_abs "$pc_modroot"; pc_modroot=$pc_absv
if [ -n "$pc_profile" ]; then pc_abs "$pc_profile"; pc_profile=$pc_absv; fi
if [ -n "$pc_cache" ]; then
	pc_abs "$pc_cache"; pc_cache=$pc_absv
	mkdir -p "$pc_cache" || pc_die "cannot create $pc_cache"
fi
if [ -n "$pc_mutant" ] && [ -z "$pc_only" ]; then
	pc_die "--mutant needs --only"
fi
if [ -n "$pc_variant" ] && [ -z "$pc_only" ]; then
	pc_die "--variant needs --only"
fi
# A fault may be planted on top of a variant only when the variant is a
# header alone (a system the reference systems cannot be, such as one with
# sig2str()): two preload libraries would compete for the same symbols.
if [ -n "$pc_variant" ] && [ -n "$pc_mutant" ]; then
	for pc_f in "$pc_modroot/xsh/${pc_variant%.*}/variants"/*.c; do
		[ -f "$pc_f" ] && pc_die "--mutant can be combined only with a header-only --variant"
	done
	[ -f "$pc_modroot/xsh/${pc_variant%.*}/variants/$pc_variant.h" ] ||
	    pc_die "--mutant can be combined only with a header-only --variant"
fi

# ---------- results directory ----------

pc_mainlog=$pc_results/run.log
if [ -e "$pc_mainlog" ] && [ "$pc_resume" != yes ]; then
	pc_die "$pc_results already has a run.log; use --resume or a new --results"
fi
if [ "$pc_resume" = yes ] && [ ! -f "$pc_mainlog" ]; then
	pc_die "--resume: no run.log in $pc_results"
fi
mkdir -p "$pc_results/build/core" "$pc_results/out" "$pc_results/state" \
    "$pc_results/tmp" || pc_die "cannot create $pc_results"

# ---------- build cache (CI only: --cache DIR) ----------

# Builds reused across the runs of one ci/mutate.sh invocation. An entry is
# looked up by the checksum of its key text, and used only if the key text
# stored with it is the same: what is built, the compiler and its flags,
# and cksum's CRC and size of every file the build reads. Without --cache
# every build is done afresh, as before.
pc_cache_key() { # what files...: the key text
	pc_kw=$1
	shift
	printf '%s\n' "$pc_kw"
	# Never let cksum read standard input (with no files it would, and the
	# driver's standard input may be an assertion file being parsed).
	[ $# -gt 0 ] || return 0
	cksum "$@" 2>&1 </dev/null
}
pc_cache_name() { # keytext: the entry's name
	printf '%s\n' "$1" | cksum | tr ' ' '_'
}
pc_cache_get() { # keytext dest: true when a matching entry was copied to dest
	[ -n "$pc_cache" ] || return 1
	pc_cn=$(pc_cache_name "$1")
	[ -f "$pc_cache/$pc_cn.key" ] && [ -f "$pc_cache/$pc_cn.bin" ] || return 1
	pc_ckt=$(cat "$pc_cache/$pc_cn.key") || return 1
	[ "$pc_ckt" = "$1" ] || return 1
	cp "$pc_cache/$pc_cn.bin" "$2" 2>/dev/null
}
pc_cache_put() { # keytext file: keep a copy of a successful build
	[ -n "$pc_cache" ] || return 0
	pc_cn=$(pc_cache_name "$1")
	cp "$2" "$pc_cache/$pc_cn.bin.$$" 2>/dev/null &&
	    printf '%s\n' "$1" >"$pc_cache/$pc_cn.key.$$" &&
	    mv -f "$pc_cache/$pc_cn.bin.$$" "$pc_cache/$pc_cn.bin" &&
	    mv -f "$pc_cache/$pc_cn.key.$$" "$pc_cache/$pc_cn.key"
	rm -f "$pc_cache/$pc_cn.bin.$$" "$pc_cache/$pc_cn.key.$$"
	return 0
}

# ---------- trust base: the shell running this script ----------

pc_n=1
while [ -e "$pc_results/trust.$pc_n" ]; do pc_n=$((pc_n + 1)); done
pc_trustdir=$pc_results/trust.$pc_n
mkdir "$pc_trustdir" "$pc_trustdir/driver" "$pc_trustdir/sut" ||
    pc_die "cannot create $pc_trustdir"

PCT_SOURCED=yes
. "$pc_root/core/trust.sh"
pc_t=$(pc_trust_run "$pc_trustdir/driver")
[ -z "$pc_t" ] || pc_abort "driver cannot run reliably: shell check '$pc_t' failed"

# ---------- compiler ----------

if [ -z "$pc_cc" ]; then
	if command -v c17 >/dev/null 2>&1; then
		pc_cc=c17
	elif command -v cc >/dev/null 2>&1; then
		pc_cc=cc
	else
		pc_abort "no C compiler (c17 or cc) found"
	fi
fi

pc_hello=$pc_trustdir/hello
printf '#include <stdio.h>\nint main(void) { puts("ok"); return 3; }\n' \
    >"$pc_hello.c"
pc_cckey=$(pc_cache_key "cc checks: $pc_cc" "$pc_hello.c")
pc_cccached=no
if pc_cache_get "$pc_cckey" "$pc_trustdir/ccstrict"; then
	read -r pc_ccstrict <"$pc_trustdir/ccstrict" || pc_ccstrict=
	[ "$pc_ccstrict" = none ] && pc_ccstrict=
	pc_cccached=yes
fi
"$pc_cc" -D_POSIX_C_SOURCE=202405L -o "$pc_hello" "$pc_hello.c" \
    >"$pc_trustdir/cc.txt" 2>&1 </dev/null ||
    pc_abort "driver cannot run reliably: $pc_cc cannot build a program"
pc_t=$("$pc_hello")
pc_rc=$?
[ "$pc_t" = ok ] && [ "$pc_rc" -eq 3 ] ||
    pc_abort "driver cannot run reliably: program built by $pc_cc misbehaves"

# A call to an undeclared function must not build: a test that uses an
# interface the headers do not declare must be UNINITIATED, not run
# against a hidden library symbol. C99 and later forbid implicit
# declarations, but some compilers only warn; then use a flag that makes it
# an error, or stop.
printf 'int main(void) { return pc_no_such_function(); }\n' \
    >"$pc_trustdir/implicit.c"
[ "$pc_cccached" = yes ] || pc_ccstrict=
pc_implicit_builds() { # extra flags...
	"$pc_cc" -D_POSIX_C_SOURCE=202405L ${1+"$@"} -c -o "$pc_trustdir/implicit.o" \
	    "$pc_trustdir/implicit.c" >>"$pc_trustdir/cc.txt" 2>&1 </dev/null
}
if [ "$pc_cccached" = yes ]; then
	:
elif pc_implicit_builds; then
	pc_ccstrict=-Werror=implicit-function-declaration
	if pc_implicit_builds "$pc_ccstrict" ||
	    ! "$pc_cc" -D_POSIX_C_SOURCE=202405L $pc_ccstrict -o "$pc_hello" \
	    "$pc_hello.c" >>"$pc_trustdir/cc.txt" 2>&1 </dev/null; then
		pc_abort "driver cannot run reliably: $pc_cc accepts calls to undeclared functions (use --cc with a stricter compiler)"
	fi
fi
if [ "$pc_cccached" = no ] && [ -n "$pc_cache" ]; then
	printf '%s\n' "${pc_ccstrict:-none}" >"$pc_trustdir/ccstrict"
	pc_cache_put "$pc_cckey" "$pc_trustdir/ccstrict"
fi

# ---------- helpers ----------

pc_bin=$pc_results/build/core
pc_k=$(pc_cache_key "pcexec: $pc_cc" "$pc_root"/core/*.c "$pc_root"/core/*.h)
if ! pc_cache_get "$pc_k" "$pc_bin/pcexec"; then
	"$pc_cc" -o "$pc_bin/pcexec" "$pc_root/core/pcexec.c" \
	    >"$pc_trustdir/pcexec.txt" 2>&1 </dev/null ||
	    pc_abort "cannot build pcexec (see $pc_trustdir/pcexec.txt)"
	pc_cache_put "$pc_k" "$pc_bin/pcexec"
fi
pc_k=$(pc_cache_key "probe: $pc_cc -D_XOPEN_SOURCE=800" "$pc_root"/core/*.c "$pc_root"/core/*.h)
if ! pc_cache_get "$pc_k" "$pc_bin/probe"; then
	"$pc_cc" -D_XOPEN_SOURCE=800 -I "$pc_root/core" -o "$pc_bin/probe" \
	    "$pc_root/core/probe.c" >"$pc_trustdir/probe.txt" 2>&1 </dev/null ||
	    pc_abort "cannot build probe (see $pc_trustdir/probe.txt)"
	pc_cache_put "$pc_k" "$pc_bin/probe"
fi
pc_k=$(pc_cache_key "pc.o: $pc_cc -D_XOPEN_SOURCE=800 $pc_ccstrict" "$pc_root"/core/*.c "$pc_root"/core/*.h)
if ! pc_cache_get "$pc_k" "$pc_bin/pc.o"; then
	"$pc_cc" -D_XOPEN_SOURCE=800 $pc_ccstrict -I "$pc_root/core" -c \
	    -o "$pc_bin/pc.o" "$pc_root/core/pc.c" >"$pc_trustdir/pc.txt" 2>&1 \
	    </dev/null || pc_abort "cannot build pc.o (see $pc_trustdir/pc.txt)"
	pc_cache_put "$pc_k" "$pc_bin/pc.o"
fi
# setgroups() is not POSIX: built with the system's own declarations, or as
# a stub where that fails (core/pcgroups.c).
pc_k=$(pc_cache_key "pcgroups.o: $pc_cc" "$pc_root"/core/pcgroups.c)
if ! pc_cache_get "$pc_k" "$pc_bin/pcgroups.o"; then
	if "$pc_cc" -c -o "$pc_bin/pcgroups.o" "$pc_root/core/pcgroups.c" \
	    >"$pc_trustdir/pcgroups.txt" 2>&1 </dev/null &&
	    printf 'int pc_drop_groups(void);\nint main(void) { return pc_drop_groups() == 9; }\n' \
	    >"$pc_trustdir/groups.c" &&
	    "$pc_cc" -o "$pc_trustdir/groups" "$pc_trustdir/groups.c" "$pc_bin/pcgroups.o" \
	    >>"$pc_trustdir/pcgroups.txt" 2>&1 </dev/null; then
		:
	else
		"$pc_cc" -DPC_NO_SETGROUPS -c -o "$pc_bin/pcgroups.o" "$pc_root/core/pcgroups.c" \
		    >>"$pc_trustdir/pcgroups.txt" 2>&1 </dev/null ||
		    pc_abort "cannot build pcgroups.o (see $pc_trustdir/pcgroups.txt)"
	fi
	pc_cache_put "$pc_k" "$pc_bin/pcgroups.o"
fi
# pcpty runs a command on a private pseudo-terminal for the terminal
# utilities; if it cannot be built those tests report UNRESOLVED.
pc_k=$(pc_cache_key "pcpty: $pc_cc" "$pc_root"/core/pcpty.c)
if ! pc_cache_get "$pc_k" "$pc_bin/pcpty"; then
	if "$pc_cc" -o "$pc_bin/pcpty" "$pc_root/core/pcpty.c" \
	    >"$pc_trustdir/pcpty.txt" 2>&1 </dev/null; then
		pc_cache_put "$pc_k" "$pc_bin/pcpty"
	fi
fi
if [ -x "$pc_bin/pcpty" ]; then PC_PTY=$pc_bin/pcpty; else PC_PTY=; fi
export PC_PTY
pc_pcexec=$pc_bin/pcexec
# A runaway test must not take the machine down: cap each test's memory
# (address space; data segment where there is no such limit). PC_MEM_LIMIT
# overrides the 1G default; 0 turns the cap off.
case ${PC_MEM_LIMIT-1G} in
0) pc_memcap= ;;
*) pc_memcap="-l as=${PC_MEM_LIMIT-1G}" ;;
esac
pc_probe=$pc_bin/probe

"$pc_pcexec" -w "$pc_trustdir" -o "$pc_trustdir/pcexec.status" -t 30 \
    -- "$pc_hello" >/dev/null 2>&1
read -r pc_k pc_v pc_ms <"$pc_trustdir/pcexec.status" || pc_k=
[ "$pc_k" = EXIT ] && [ "$pc_v" = 3 ] ||
    pc_abort "driver cannot run reliably: pcexec misreports exit status"

# ---------- the shell the tests run under ----------

pc_cspath=$("$pc_probe" cspath) || pc_abort "confstr(_CS_PATH) unavailable"
if [ -z "$pc_sut_sh" ]; then
	pc_oldifs=$IFS
	IFS=:
	set -f
	for pc_d in $pc_cspath; do
		if [ -x "$pc_d/sh" ]; then
			pc_sut_sh=$pc_d/sh
			break
		fi
	done
	set +f
	IFS=$pc_oldifs
	[ -n "$pc_sut_sh" ] || pc_abort "no sh on _CS_PATH ($pc_cspath)"
fi
PC_SH=$pc_sut_sh; export PC_SH
pc_t=$("$pc_sut_sh" "$pc_root/core/trust.sh" "$pc_trustdir/sut")
[ -z "$pc_t" ] || pc_abort "tests cannot run reliably: $pc_sut_sh check '$pc_t' failed"

# The results directory holds files named after assertions and tests: its
# file system must allow names that long ({NAME_MAX}; ci/lint.py keeps every
# name the suite creates within PC_NAME_NEED). This is a requirement on
# where the suite runs, not a verdict on the system: the tests' private
# directories need only {_POSIX_NAME_MAX} (14).
PC_NAME_NEED=64
pc_nm=$("$pc_probe" namemax "$pc_results")
case $pc_nm in
'' | *[!0-9-]*) pc_abort "probe could not read {NAME_MAX} for $pc_results" ;;
esac
if [ "$pc_nm" -ge 0 ] && [ "$pc_nm" -lt "$PC_NAME_NEED" ]; then
	pc_abort "the file system of $pc_results allows names of $pc_nm bytes; the suite needs $PC_NAME_NEED there (choose another --results directory)"
fi

# ---------- claims and profile ----------

"$pc_probe" claims >"$pc_results/claims.txt" ||
    pc_abort "probe could not report claims"
while read -r pc_k pc_code pc_macro pc_yes pc_rest; do
	[ "$pc_k" = claim ] || continue
	case $pc_code in '' | *[!A-Z0-9]*) continue ;; esac
	eval "pc_claim_$pc_code=\$pc_yes"
	case " $pc_rest " in
	*" compile=yes "*) eval "pc_cclaim_$pc_code=yes" ;;
	*) eval "pc_cclaim_$pc_code=no" ;;
	esac
done <"$pc_results/claims.txt"

# A profile has lines "key value". Keys used now:
#   have NAME [VALUE]  the environment provides NAME (for "needs NAME");
#                      a VALUE reaches the tests as PC_HAVE_<NAME>
if [ -n "$pc_profile" ]; then
	[ -f "$pc_profile" ] || pc_die "no profile $pc_profile"
	while read -r pc_k pc_v pc_rest; do
		case $pc_k in
		'' | '#'*) ;;
		have)
			case $pc_v in
			'' | *[!a-z0-9_]*) pc_warn "profile: bad name '$pc_v'" ;;
			*)
				eval "pc_have_$pc_v=yes"
				if [ -n "$pc_rest" ]; then
					pc_uv=$(printf '%s' "$pc_v" | tr a-z A-Z)
					eval "PC_HAVE_$pc_uv=\$pc_rest"
					export "PC_HAVE_$pc_uv"
				fi
				;;
			esac
			;;
		*) pc_warn "profile: unknown key '$pc_k'" ;;
		esac
	done <"$pc_profile"
fi
pc_euid=$("$pc_probe" euid)

# Is the option expression claimed? "BASE", "MSG", or "TPP|RPP".
pc_claimed() {
	pc_x=$1
	while :; do
		# "|" is quoted: ksh93 reads a bare | in a pattern as alternation
		pc_alt=${pc_x%%'|'*}
		pc_claimed_all "$pc_alt" && return 0
		case $pc_x in
		*'|'*) pc_x=${pc_x#*'|'} ;;
		*) return 1 ;;
		esac
	done
}

# The libraries of a list that link an empty program here (each tried once).
pc_usable_libs() {
	pc_ul=
	for pc_l in "$@"; do
		pc_lv=$(printf '%s' "$pc_l" | tr -c 'A-Za-z0-9\n' '_')
		eval "pc_lok=\${pc_libok_$pc_lv:-}"
		if [ -z "$pc_lok" ]; then
			pc_lok=no
			pc_lk=$(pc_cache_key "links: $pc_cc $pc_l")
			if pc_cache_get "$pc_lk" "$pc_results/.libok"; then
				read -r pc_lok <"$pc_results/.libok" || pc_lok=no
			else
				if [ -d "$pc_results" ] && printf 'int main(void) { return 0; }\n' >"$pc_results/.lib.c" &&
				    "$pc_cc" -o "$pc_results/.lib" "$pc_results/.lib.c" "$pc_l" >/dev/null 2>&1 </dev/null; then
					pc_lok=yes
				fi
				printf '%s\n' "$pc_lok" >"$pc_results/.libok"
				pc_cache_put "$pc_lk" "$pc_results/.libok"
			fi
			rm -f "$pc_results/.lib" "$pc_results/.lib.c" "$pc_results/.libok"
			eval "pc_libok_$pc_lv=\$pc_lok"
		fi
		[ "$pc_lok" = yes ] && pc_ul="$pc_ul $pc_l"
	done
	printf '%s\n' "$pc_ul"
}

# Is every code of "A+B+..." claimed?
pc_claimed_all() {
	pc_y=$1
	while :; do
		pc_one=${pc_y%%+*}
		case $pc_one in
		BASE) ;;
		'' | *[!A-Z0-9]*) return 1 ;;
		*)
			# A module whose build failure is the failure (buildfail FAIL)
			# checks headers only: an option supported for compilation is
			# enough (macro 0, whatever sysconf() says).
			if [ "${mod_buildfail:-}" = FAIL ]; then
				eval "pc_cv=\${pc_cclaim_$pc_one:-no}"
			else
				eval "pc_cv=\${pc_claim_$pc_one:-no}"
			fi
			[ "$pc_cv" = yes ] || return 1
			;;
		esac
		case $pc_y in
		*+*) pc_y=${pc_y#*+} ;;
		*) return 0 ;;
		esac
	done
}

# Does the environment provide what "needs NAME" asks for?
pc_have() {
	case $1 in
	root) [ "$pc_euid" = 0 ] ;;
	'' | *[!a-z0-9_]*) return 1 ;;
	*)
		eval "pc_hv=\${pc_have_$1:-no}"
		[ "$pc_hv" = yes ]
		;;
	esac
}

# ---------- run context (the self-tests and the real run each have one) ----------

pc_ctx() {
	ctx_results=$1
	ctx_modroot=$2
	ctx_selftest=$3
	ctx_log=$ctx_results/run.log
	ctx_state=$ctx_results/state
	ctx_out=$ctx_results/out
	ctx_tmp=$ctx_results/tmp
	ctx_build=$ctx_results/build
	mkdir -p "$ctx_state" "$ctx_out" "$ctx_tmp" "$ctx_build" ||
	    pc_abort "cannot create $ctx_results"
}

# What the terminal shows for one result: the code, the ID and the test's
# name; for anything but PASS, the test's path and the note under it,
# indented so a wrapped note never reads as another result. run.log keeps
# its one-line form.
pc_show() {
	pc_sw=
	if [ -n "${a_test:-}" ]; then
		pc_sw=${a_test##*/}
		pc_sw=${pc_sw%.*}
		pc_sw=${pc_sw#"${pc_sw%%[!0-9]*}"}
		pc_sw=${pc_sw#-}
		pc_sp=$mdir/$a_test
	else
		pc_sp=${pc_asr:-}
	fi
	pc_sp=${pc_sp#"$pc_root/"}
	if [ -n "$pc_sw" ]; then
		printf '%-11s %s [%s]\n' "$1" "$2" "$pc_sw"
	else
		printf '%-11s %s\n' "$1" "$2"
	fi
	[ "$1" = PASS ] && [ -z "$3" ] && return 0
	[ "$1" != PASS ] && [ -n "$pc_sp" ] && printf '%12s%s\n' '' "$pc_sp"
	[ -n "$3" ] && pc_wrap "$3"
	return 0
}

# Print $1 indented by 12 columns. On a terminal, break it between words
# to fit ${COLUMNS:-80}; elsewhere, keep it on one line.
pc_wrap() {
	if [ ! -t 1 ]; then
		printf '%12s%s\n' '' "$1"
		return 0
	fi
	pc_ww=${COLUMNS:-80}
	case $pc_ww in '' | *[!0-9]*) pc_ww=80 ;; esac
	[ "$pc_ww" -ge 40 ] || pc_ww=40
	pc_ww=$((pc_ww - 13))
	pc_wl=
	set -f
	for pc_wt in $1; do
		if [ -z "$pc_wl" ]; then
			pc_wl=$pc_wt
		elif [ $((${#pc_wl} + 1 + ${#pc_wt})) -le "$pc_ww" ]; then
			pc_wl="$pc_wl $pc_wt"
		else
			printf '%12s%s\n' '' "$pc_wl"
			pc_wl=$pc_wt
		fi
	done
	set +f
	[ -n "$pc_wl" ] && printf '%12s%s\n' '' "$pc_wl"
	return 0
}

pc_log() {
	printf '%s\t%s\t%s\t%s\n' "$1" "$2" "$3" "$4" >>"$ctx_log"
	printf '%s\n' "$1" >"$ctx_state/$2"
	pc_show "$1" "$2" "$4"
	if [ "$ctx_selftest" = yes ] && [ "$1" != "${a_expect:-}" ]; then
		printf '%s expected %s, got %s: %s\n' "$2" "${a_expect:-?}" \
		    "$1" "$4" >>"$ctx_results/mismatch"
	fi
}

# Volume directory <-> ID prefix.
pc_vol_upper() {
	case $1 in
	xbd) pc_vu=XBD ;; xsh) pc_vu=XSH ;; xcu) pc_vu=XCU ;; st) pc_vu=ST ;;
	*) pc_vu= ;;
	esac
}
pc_vol_lower() {
	case $1 in
	XBD) pc_vl=xbd ;; XSH) pc_vl=xsh ;; XCU) pc_vl=xcu ;; ST) pc_vl=st ;;
	*) pc_vl= ;;
	esac
}

# XSH.2.6-file-descriptor-allocation.001 -> xsh/2.6-file-descriptor-allocation
pc_id_module() {
	pc_im=${1%.*}
	pc_vol_lower "${pc_im%%.*}"
	if [ -n "$pc_vl" ]; then pc_im=$pc_vl/${pc_im#*.}; else pc_im=; fi
}

# ---------- one assertion ----------

pc_asr_reset() {
	a_id= a_class= a_when= a_test= a_needs= a_timeout= a_expect=
	a_reason= a_uses=
}

pc_assertion() {
	pc_id=$a_id
	case $pc_id in
	'' | *[!A-Za-z0-9._-]*) pc_warn "$mdir: bad assertion id '$pc_id'"; return 0 ;;
	esac
	if [ -n "$pc_only" ]; then
		case " $pc_only_set " in
		*" $pc_id "*) ;;
		*) return 0 ;;
		esac
	fi
	if [ -e "$ctx_state/$pc_id" ]; then return 0; fi
	# A planted fault is for the selected assertion only, never for the
	# prerequisites run with it.
	pc_mut= pc_var=
	[ "$pc_id" = "$pc_only" ] && pc_mut=$pc_mutant pc_var=$pc_variant

	pc_when=${a_when:-${mod_option:-BASE}}
	if [ "${a_class:-required}" = untestable ]; then
		pc_log UNTESTED "$pc_id" 0 "untestable: ${a_reason:-no reason given}"
		return 0
	fi
	if [ -z "$a_test" ]; then
		pc_log UNTESTED "$pc_id" 0 "no test written"
		return 0
	fi
	if ! pc_claimed "$pc_when"; then
		pc_log UNSUPPORTED "$pc_id" 0 "option $pc_when not claimed"
		return 0
	fi
	for pc_n in $a_needs; do
		if ! pc_have "$pc_n"; then
			case $pc_n in
			root) pc_nn="skipped: needs root privilege, which this run was not granted" ;;
			*) pc_nn="skipped: needs $pc_n, which this system or run does not provide" ;;
			esac
			pc_log UNRESOLVED "$pc_id" 0 "$pc_nn"
			return 0
		fi
	done
	for pc_u in $mod_uses $a_uses; do
		pc_uc=
		[ -f "$ctx_state/$pc_u" ] && read -r pc_uc <"$ctx_state/$pc_u"
		if [ "$pc_uc" != PASS ]; then
			pc_log UNRESOLVED "$pc_id" 0 "blocked by $pc_u (${pc_uc:-not run})"
			return 0
		fi
	done

	pc_src=$mdir/$a_test
	if [ ! -f "$pc_src" ]; then
		pc_log UNINITIATED "$pc_id" 0 "test file $a_test missing"
		return 0
	fi
	pc_extra=
	case $pc_src in
	*.c)
		pc_exe=$ctx_build/$pc_mod/${a_test##*/}
		pc_exe=${pc_exe%.c}
		mkdir -p "${pc_exe%/*}" || pc_abort "cannot create ${pc_exe%/*}"
		pc_mlibs=$(pc_usable_libs $mod_libs)
		pc_flags=-D_POSIX_C_SOURCE=202405L
		case "|$pc_when+" in *[+'|']XSI[+'|']*) pc_flags=-D_XOPEN_SOURCE=800 ;; esac
		# A variant may change what the headers define (a constant with
		# another allowed value): it is a header included first, and the
		# test is built apart.
		pc_vinc=
		if [ -n "$pc_var" ] && [ -f "$pc_modroot/xsh/${pc_var%.*}/variants/$pc_var.h" ]; then
			pc_vinc="-include $pc_modroot/xsh/${pc_var%.*}/variants/$pc_var.h"
			pc_exe=$pc_exe.variant
		fi
		pc_tk=$(pc_cache_key "test: $pc_cc $pc_flags $pc_ccstrict $pc_vinc libs:$pc_mlibs" \
		    "$pc_src" "$pc_root"/core/*.h "$pc_bin/pc.o" "$pc_bin/pcgroups.o" ${pc_vinc:+"${pc_vinc#-include }"} \
		    ${pc_vinc:+"$pc_modroot/xsh/${pc_var%.*}/variants"/*.inc})
		if pc_cache_get "$pc_tk" "$pc_exe"; then
			: >"$ctx_out/$pc_id.build.txt"
		# shellcheck disable=SC2086 # mod_libs is a word list
		elif ! "$pc_cc" $pc_flags $pc_ccstrict $pc_vinc -I "$pc_root/core" -o "$pc_exe" "$pc_src" \
		    "$pc_bin/pc.o" "$pc_bin/pcgroups.o" $pc_mlibs >"$ctx_out/$pc_id.build.txt" 2>&1 \
		    </dev/null; then
			if [ "$mod_buildfail" = FAIL ]; then
				# Tier 0 checks: a test that does not compile shows a
				# missing or wrong declaration, which is the failure.
				pc_e=$(grep -m 1 'error' "$ctx_out/$pc_id.build.txt" |
				    sed 's/^.*error: //' | tr '\t' ' ' | cut -c 1-200)
				pc_log FAIL "$pc_id" 0 "does not compile: ${pc_e:-see out/$pc_id.build.txt}"
				return 0
			fi
			pc_log UNINITIATED "$pc_id" 0 "build failed (out/$pc_id.build.txt)"
			return 0
		else
			pc_cache_put "$pc_tk" "$pc_exe"
		fi
		set -- "$pc_exe"
		pc_extra=
		if [ -n "$pc_mut" ]; then pc_mutant_c || return 0; fi
		if [ -n "$pc_var" ]; then pc_variant_c || return 0; fi
		;;
	*.sh)
		set -- "$pc_sut_sh" "$pc_src"
		pc_mutant_file=
		if [ -n "$pc_mut" ]; then pc_mutant_sh || return 0; fi
		if [ -n "$pc_var" ]; then
			pc_mutant_file=$pc_modroot/xcu/${pc_var%.*}/variants/$pc_var.sh
			[ -f "$pc_mutant_file" ] || pc_abort "no variant file $pc_mutant_file"
		fi
		;;
	*)
		pc_log UNINITIATED "$pc_id" 0 "unknown test type $a_test"
		return 0
		;;
	esac

	pc_rfile=$ctx_tmp/$pc_id.result
	pc_sfile=$ctx_tmp/$pc_id.status
	: >"$pc_rfile"
	# shellcheck disable=SC2086 # pc_extra is a word list
	PC_RESULT=$pc_rfile PC_ASSERTION=$pc_id PC_CORE=$pc_root/core \
	    PC_PHASE=setup PC_MUTANT=$pc_mut PC_VARIANT=$pc_var \
	    PC_MUTANT_FILE=${pc_mutant_file:-} \
	    PC_SELFTEST_DIR=$ctx_results/survivors \
	    "$pc_pcexec" -w "$ctx_tmp" -o "$pc_sfile" -t "${a_timeout:-60}" \
	    $pc_memcap $pc_extra -- "$@" >"$ctx_out/$pc_id.txt" 2>&1 </dev/null
	pc_decide
	# A disputed assertion (the standard can be read either way; its note
	# says why) does not count a FAIL against the system: it is logged as
	# INSPECT for a person to look at.
	if [ "${a_class:-required}" = disputed ] && [ "$pc_code" = FAIL ]; then
		pc_code=INSPECT
		pc_note="disputed (see the assertion's note): $pc_note"
	fi
	pc_log "$pc_code" "$pc_id" "$pc_ms" "$pc_note"
}

# Turn the status and result files into a code and note.
pc_decide() {
	pc_sk= pc_s1= pc_s2= pc_ms=0
	if [ -f "$pc_sfile" ]; then
		read -r pc_sk pc_s1 pc_s2 pc_rest <"$pc_sfile" || :
	fi
	pc_nres=0 pc_phase=none pc_rcode= pc_rid= pc_rnote=
	while IFS= read -r pc_line; do
		case $pc_line in
		'PHASE '*) pc_phase=${pc_line#PHASE } ;;
		'RESULT '*)
			pc_nres=$((pc_nres + 1))
			pc_r=${pc_line#RESULT }
			pc_rcode=${pc_r%% *}
			case $pc_r in *' '*) pc_r=${pc_r#* } ;; *) pc_r= ;; esac
			pc_rid=${pc_r%% *}
			case $pc_r in *' '*) pc_rnote=${pc_r#* } ;; *) pc_rnote= ;; esac
			;;
		esac
	done <"$pc_rfile"

	case $pc_sk in
	EXIT | SIGNAL) pc_ms=$pc_s2 ;;
	TIMEOUT) pc_ms=$pc_s1 ;;
	esac

	case $pc_sk in
	TIMEOUT)
		pc_code=UNRESOLVED pc_note="timed out after ${a_timeout:-60}s"
		return 0
		;;
	ERROR)
		pc_code=UNRESOLVED pc_note="harness error: $pc_s1 $pc_s2 $pc_rest"
		return 0
		;;
	EXIT | SIGNAL) ;;
	*)
		pc_code=UNRESOLVED pc_note="harness error: no status from pcexec"
		return 0
		;;
	esac

	if [ "$pc_nres" -gt 1 ]; then
		pc_code=UNRESOLVED pc_note="test reported $pc_nres results"
		return 0
	fi
	if [ "$pc_nres" -eq 1 ]; then
		if [ "$pc_rid" != "$pc_id" ]; then
			pc_code=UNRESOLVED pc_note="result names $pc_rid, not $pc_id"
			return 0
		fi
		case $pc_rcode in
		PASS | FAIL | UNRESOLVED | NOTINUSE | UNSUPPORTED) ;;
		*)
			pc_code=UNRESOLVED pc_note="invalid result code '$pc_rcode'"
			return 0
			;;
		esac
		pc_code=$pc_rcode pc_note=$pc_rnote
		if [ "$pc_code" = PASS ] && { [ "$pc_sk" != EXIT ] || [ "$pc_s1" != 0 ]; }; then
			pc_code=FAIL pc_note="reported PASS but ended with $pc_sk $pc_s1"
		fi
		return 0
	fi
	if [ "$pc_sk" = SIGNAL ]; then
		if [ "$pc_phase" = test ]; then
			pc_code=FAIL pc_note="killed by signal $pc_s1 during the test"
		else
			pc_code=UNRESOLVED pc_note="killed by signal $pc_s1 during setup"
		fi
		return 0
	fi
	pc_code=UNINITIATED pc_note="exited $pc_s1 without reporting a result"
}

# ---------- mutants (CI only) ----------

# C: modules/xsh/<function>/mutants/*.c builds one preload library that
# implements every "<function>.<fault>" mutant.
pc_mutant_c() {
	pc_func=${pc_mutant%.*}
	pc_mdir=$pc_modroot/xsh/$pc_func/mutants
	pc_so=$ctx_build/mutants/$pc_func.so
	mkdir -p "$ctx_build/mutants" || pc_abort "cannot create mutant dir"
	if [ ! -f "$pc_so" ]; then
		set -- "$pc_mdir"/*.c
		[ -f "$1" ] || pc_abort "no mutant sources in $pc_mdir"
		pc_sk=$(pc_cache_key "mutant library: $pc_cc -shared -fPIC ${PC_MUTANT_LDFLAGS--ldl}" "$@")
		if ! pc_cache_get "$pc_sk" "$pc_so"; then
			# shellcheck disable=SC2086
			"$pc_cc" -shared -fPIC -o "$pc_so" "$@" ${PC_MUTANT_LDFLAGS--ldl} \
			    >"$ctx_out/mutant-$pc_func.build.txt" 2>&1 </dev/null ||
			    pc_abort "cannot build mutant library for $pc_func"
			pc_cache_put "$pc_sk" "$pc_so"
		fi
	fi
	pc_extra="-e LD_PRELOAD=$pc_so"
}

# C: modules/xsh/<function>/variants/*.c builds one preload library that
# implements every "<function>.<variant>" (PC_VARIANT) that is not only a
# header (variants/<function>.<variant>.h, included before the test).
pc_variant_c() {
	pc_func=${pc_var%.*}
	pc_vdir=$pc_modroot/xsh/$pc_func/variants
	set -- "$pc_vdir"/*.c
	[ -f "$1" ] || return 0
	pc_so=$ctx_build/variants/$pc_func.so
	mkdir -p "$ctx_build/variants" || pc_abort "cannot create variant dir"
	if [ ! -f "$pc_so" ]; then
		pc_sk=$(pc_cache_key "variant library: $pc_cc -shared -fPIC ${PC_MUTANT_LDFLAGS--ldl}" "$@" \
		    "$pc_vdir"/*.h "$pc_vdir"/*.inc "$pc_root"/core/*.h)
		if ! pc_cache_get "$pc_sk" "$pc_so"; then
			# shellcheck disable=SC2086
			"$pc_cc" -shared -fPIC -I "$pc_root/core" -o "$pc_so" "$@" ${PC_MUTANT_LDFLAGS--ldl} \
			    >"$ctx_out/variant-$pc_func.build.txt" 2>&1 </dev/null ||
			    pc_abort "cannot build variant library for $pc_func"
			pc_cache_put "$pc_sk" "$pc_so"
		fi
	fi
	pc_extra="-e LD_PRELOAD=$pc_so"
}

# Shell: modules/xcu/<utility>/mutants/<name>.sh defines functions that
# shadow the utility; pc.sh loads it when the test phase begins.
pc_mutant_sh() {
	pc_util=${pc_mutant%.*}
	pc_mutant_file=$pc_modroot/xcu/$pc_util/mutants/$pc_mutant.sh
	[ -f "$pc_mutant_file" ] || pc_abort "no mutant file $pc_mutant_file"
}

# ---------- one module ----------

pc_run_module() {
	pc_mod=$1
	mdir=$ctx_modroot/$pc_mod
	pc_mark=$ctx_state/module.${pc_mod%%/*}.${pc_mod#*/}
	[ -e "$pc_mark.done" ] && return 0
	if [ -e "$pc_mark.busy" ]; then
		pc_warn "prerequisite cycle through $pc_mod"
		return 0
	fi
	if [ ! -f "$mdir/MODULE" ]; then
		pc_warn "no module $pc_mod"
		return 0
	fi
	: >"$pc_mark.busy"

	mod_option=BASE mod_uses= mod_libs= mod_buildfail=UNINITIATED
	while read -r pc_k pc_v; do
		case $pc_k in
		option) mod_option=$pc_v ;;
		buildfail) mod_buildfail=$pc_v ;;
		uses) mod_uses=$pc_v ;;
		libs) mod_libs=$pc_v ;;
		esac
	done <"$mdir/MODULE"

	# Prerequisites (the module's "uses" and every assertion's) run
	# first, each in a subshell: all run state lives in files, so nothing
	# is lost, and our variables are left alone.
	pc_prereqs=$mod_uses
	for pc_asr in "$mdir"/*.asr; do
		[ -f "$pc_asr" ] || continue
		while read -r pc_k pc_v; do
			[ "$pc_k" = uses ] && pc_prereqs="$pc_prereqs $pc_v"
		done <"$pc_asr"
	done
	for pc_u in $pc_prereqs; do
		pc_id_module "$pc_u"
		[ "$pc_im" = "$pc_mod" ] && continue
		if [ -n "$pc_im" ] && [ -f "$ctx_modroot/$pc_im/MODULE" ]; then
			(pc_only= pc_sel_options= pc_run_module "$pc_im")
		fi
	done

	# With --only, the selected assertion's prerequisites in this module
	# run too (they come earlier in the .asr files), transitively.
	pc_only_set=$pc_only
	if [ -n "$pc_only" ]; then
		pc_grew=yes
		while [ "$pc_grew" = yes ]; do
			pc_grew=no
			for pc_asr in "$mdir"/*.asr; do
				[ -f "$pc_asr" ] || continue
				pc_cur=
				while read -r pc_k pc_v; do
					case $pc_k in
					assertion) pc_cur=$pc_v ;;
					uses)
						case " $pc_only_set " in
						*" $pc_cur "*) ;;
						*) continue ;;
						esac
						for pc_u in $pc_v; do
							pc_id_module "$pc_u"
							[ "$pc_im" = "$pc_mod" ] || continue
							case " $pc_only_set " in
							*" $pc_u "*) ;;
							*) pc_only_set="$pc_only_set $pc_u"; pc_grew=yes ;;
							esac
						done
						;;
					esac
				done <"$pc_asr"
			done
		done
	fi

	pc_keep=yes
	if [ -n "$pc_sel_options" ] && [ -z "$pc_only" ]; then
		pc_keep=no
		for pc_o in $pc_sel_options; do
			[ "$pc_o" = "$mod_option" ] && pc_keep=yes
		done
	fi
	if [ "$pc_keep" = yes ]; then
		for pc_asr in "$mdir"/*.asr; do
			[ -f "$pc_asr" ] || continue
			pc_asr_reset
			pc_have_one=no
			while IFS= read -r pc_line || [ -n "$pc_line" ]; do
				case $pc_line in
				'' | '#'*) continue ;;
				[' 	']*) continue ;;
				esac
				pc_k=${pc_line%%[' 	']*}
				pc_v=${pc_line#"$pc_k"}
				pc_v=${pc_v#"${pc_v%%[! 	]*}"}
				case $pc_k in
				assertion)
					# Each assertion runs with its own standard input: nothing
					# it does may read the rest of this file.
					[ "$pc_have_one" = yes ] && pc_assertion </dev/null
					pc_asr_reset
					a_id=$pc_v
					pc_have_one=yes
					;;
				class) a_class=$pc_v ;;
				when) a_when=$pc_v ;;
				test) a_test=$pc_v ;;
				needs) a_needs=$pc_v ;;
				timeout) a_timeout=$pc_v ;;
				expect) a_expect=$pc_v ;;
				reason) a_reason=$pc_v ;;
				uses) a_uses=$pc_v ;;
				anchor | since | says | origin | note | nomutant) ;;
				*) pc_warn "$pc_asr: unknown key '$pc_k'" ;;
				esac
			done <"$pc_asr"
			[ "$pc_have_one" = yes ] && pc_assertion </dev/null
		done
	fi
	: >"$pc_mark.done"
}

pc_run_modules() {
	if [ -n "$pc_only" ]; then
		pc_id_module "$pc_only"
		[ -n "$pc_im" ] || pc_die "--only: cannot tell the module of $pc_only"
		pc_run_module "$pc_im"
	elif [ -n "$pc_args" ]; then
		for pc_m in $pc_args; do pc_run_module "${pc_m%/}"; done
	else
		for pc_mf in "$ctx_modroot"/*/*/MODULE; do
			[ -f "$pc_mf" ] || continue
			pc_m=${pc_mf#"$ctx_modroot"/}
			pc_m=${pc_m%/MODULE}
			(pc_run_module "$pc_m")
		done
	fi
}

# ---------- harness self-tests ----------

if [ "$pc_selftest" = yes ]; then
	pc_n=1
	while [ -e "$pc_results/selftest.$pc_n" ]; do pc_n=$((pc_n + 1)); done
	pc_stdir=$pc_results/selftest.$pc_n
	printf 'posixcheck: harness self-tests (%s)\n' "$pc_stdir" >&2
	(
		pc_only= pc_sel_options= pc_args= pc_mutant= pc_variant=
		pc_ctx "$pc_stdir" "$pc_root/core/selftest/modules" yes
		mkdir -p "$ctx_results/survivors" || exit 1
		pc_run_modules
	) >"$pc_trustdir/selftest.txt" 2>&1
	# Every self-test assertion must have run: one that was silently skipped
	# would otherwise show no mismatch at all.
	for pc_f in "$pc_root"/core/selftest/modules/*/*/*.asr; do
		while read -r pc_k pc_v; do
			[ "$pc_k" = assertion ] || continue
			[ -e "$pc_stdir/state/$pc_v" ] ||
			    printf '%s was never run\n' "$pc_v" >>"$pc_stdir/mismatch"
		done <"$pc_f"
	done
	# A survivor of a killed test would write a marker after 2 seconds.
	"$pc_probe" sleep 3
	for pc_f in "$pc_stdir"/survivors/*; do
		[ -e "$pc_f" ] && printf '%s survived its timeout\n' "${pc_f##*/}" \
		    >>"$pc_stdir/mismatch"
	done
	if [ -f "$pc_stdir/mismatch" ]; then
		while IFS= read -r pc_l; do
			printf 'posixcheck: self-test: %s\n' "$pc_l" >&2
		done <"$pc_stdir/mismatch"
		pc_abort "harness self-tests failed (see $pc_stdir)"
	fi
	printf 'posixcheck: harness self-tests passed\n' >&2
fi
[ "$pc_selftest_only" = yes ] && exit 0

# ---------- the real run ----------

pc_ctx "$pc_results" "$pc_modroot" no
if [ "$pc_resume" = yes ]; then
	printf '# resumed %s\n' "$("$pc_probe" now)" >>"$ctx_log"
else
	{
		printf '# posixcheck %s\n' "$pc_version"
		printf '# tree %s\n' "${PC_TREE:-unknown}"
		while read -r pc_k pc_v; do
			printf '# %s %s\n' "$pc_k" "$pc_v"
		done <<EOF
$("$pc_probe" sysinfo)
EOF
		printf '# sh %s\n' "$pc_sut_sh"
		printf '# cc %s\n' "$pc_cc"
		printf '# ccflags %s\n' "${pc_ccstrict:-none}"
		printf '# euid %s\n' "$pc_euid"
		printf '# profile %s\n' "${pc_profile:-none}"
		[ -n "$pc_mutant" ] && printf '# mutant %s\n' "$pc_mutant"
		[ -n "$pc_variant" ] && printf '# variant %s\n' "$pc_variant"
		printf '# started %s\n' "$("$pc_probe" now)"
		while IFS= read -r pc_l; do
			printf '# %s\n' "$pc_l"
		done <"$pc_results/claims.txt"
	} >>"$ctx_log"
fi
pc_run_modules
printf '# finished %s\n' "$("$pc_probe" now)" >>"$ctx_log"
pc_sum=$(awk -F'\t' '
	/^#/ { next }
	{ n[$1]++; total++ }
	END {
		printf "SUMMARY total %d", total
		m = split("PASS FAIL INSPECT UNRESOLVED NOTINUSE UNSUPPORTED UNINITIATED", c, " ")
		for (i = 1; i <= m; i++) printf " %s %d", c[i], n[c[i]]
		printf "\n"
	}' "$ctx_log")
printf '# %s\n' "$pc_sum" >>"$ctx_log"
printf '%s\n' "$pc_sum"
if [ "$pc_ephemeral" = yes ]; then
	# FAIL count is the 7th word of the summary line.
	set -- $pc_sum
	[ "${7:-0}" -eq 0 ] || exit 1
else
	printf 'posixcheck: results in %s\n' "$ctx_log" >&2
fi
