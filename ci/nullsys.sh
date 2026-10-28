#!/bin/sh
# nullsys.sh - run the suite against a "null system", a negative control
# (CI, development machines with a C compiler and dynamic linking).
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: ci/nullsys.sh build [--poison] [--modules DIR] [--model FILE]
#                            [--cc CC] DIR
#        ci/nullsys.sh run [--flip] [--real-sh] DIR [posixcheck.sh options
#                            and modules]
#        ci/nullsys.sh report [--root DIR] DIR RESULTS
#
# build   creates DIR with libnullsys.so (a stub for every standard
#         function, from the spec model), a stub shell DIR/sh, a stub for
#         every utility of the modules (DIR/bin/NAME), and the helpers that
#         preload the library into each C test. --poison makes the stubs
#         return values no system returns instead of -1 and ENOSYS.
# run     runs posixcheck.sh against it: the shell under test is DIR/sh,
#         C tests run with the library preloaded (stubs on only between
#         pc_test() and the verdict), results are kept in DIR/results
#         (DIR/results-flip with --flip, DIR/results-realsh with --real-sh)
#         unless --results says otherwise. --flip makes the stub shell and
#         utilities exit 1 and print "error" instead of exiting 0 silently.
#         --real-sh keeps the real shell but puts the stub utilities first
#         on its PATH. Then it prints the suspicious set (see report).
# report  prints the suspicious set of a finished run: every assertion that
#         gave PASS (or any code but FAIL, INSPECT, UNRESOLVED, UNSUPPORTED,
#         NOTINUSE, UNINITIATED, UNTESTED), split into (a) subject never
#         called, (b) subject called, and the compile-only xbd modules.
#
# docs/negative-control.md says how to read the result.

set -u
here=$(cd "$(dirname "$0")/.." && pwd)

die() {
	printf 'nullsys.sh: %s\n' "$*" >&2
	exit 2
}

absdir() { # dir: print its absolute path (creating it)
	mkdir -p "$1" || die "cannot create $1"
	(cd "$1" && pwd)
}

pick_cc() {
	if [ -n "${cc:-}" ]; then return 0; fi
	if [ -n "${CC:-}" ]; then
		cc=$CC
	elif command -v c17 >/dev/null 2>&1; then
		cc=c17
	elif command -v cc >/dev/null 2>&1; then
		cc=cc
	else
		die "no C compiler (c17 or cc) found"
	fi
}

# ---------- build ----------

do_build() {
	poison=no modroot=$here/modules model= cc=
	while [ $# -gt 0 ]; do
		case $1 in
		--poison) poison=yes; shift ;;
		--modules) [ $# -ge 2 ] || die "--modules needs an argument"; modroot=$2; shift 2 ;;
		--model) [ $# -ge 2 ] || die "--model needs an argument"; model=$2; shift 2 ;;
		--cc) [ $# -ge 2 ] || die "--cc needs an argument"; cc=$2; shift 2 ;;
		-*) die "build: unknown option $1" ;;
		*) break ;;
		esac
	done
	[ $# -eq 1 ] || die "usage: ci/nullsys.sh build [--poison] [--modules DIR] [--model FILE] [--cc CC] DIR"
	dir=$(absdir "$1")
	pick_cc
	command -v python3 >/dev/null 2>&1 || die "python3 is needed to generate the stubs"
	[ -d "$modroot/xcu" ] || die "no $modroot/xcu"

	rm -rf "$dir/bin" "$dir/sh" "$dir/cc" "$dir/libnullsys.so" "$dir/.stub" "$dir/.launch" "$dir/utilities"
	mkdir "$dir/bin" || die "cannot create $dir/bin"

	# 1. the stubs
	python3 "$here/gen/nullsys.py" ${model:+--model "$model"} --out "$dir/nullsys.c" \
	    2>"$dir/nullsys.gen.txt" || { cat "$dir/nullsys.gen.txt" >&2; die "gen/nullsys.py failed"; }
	cp "$here/gen/nullsys_rt.c" "$dir/nullsys_rt.c" || die "cannot copy the runtime"
	pdef=
	[ "$poison" = yes ] && pdef=-DNS_POISON
	# Optimisation and builtins off: the library must not call itself.
	cflags=${NULLSYS_CFLAGS--O0 -fno-builtin -fPIC}
	ldflags=${NULLSYS_LDFLAGS--ldl}
	built=no
	for gnu in "" "-D_GNU_SOURCE"; do
		for ld in "$ldflags" ""; do
			# shellcheck disable=SC2086 # flags are word lists
			if "$cc" $cflags -shared $pdef -D_POSIX_C_SOURCE=202405L $gnu \
			    -o "$dir/libnullsys.so" "$dir/nullsys.c" "$dir/nullsys_rt.c" $ld \
			    >"$dir/libnullsys.build.txt" 2>&1 </dev/null; then
				built=yes
				break 2
			fi
		done
	done
	[ "$built" = yes ] || { cat "$dir/libnullsys.build.txt" >&2; die "cannot build libnullsys.so (see above)"; }

	# 2. the stub shell, the stub utilities, the launcher
	"$cc" -D_POSIX_C_SOURCE=202405L -o "$dir/.stub" "$here/ci/nullsys-stub.c" \
	    >"$dir/stub.build.txt" 2>&1 </dev/null || { cat "$dir/stub.build.txt" >&2; die "cannot build the stub"; }
	"$cc" -D_POSIX_C_SOURCE=202405L -o "$dir/.launch" "$here/ci/nullsys-launch.c" \
	    >"$dir/launch.build.txt" 2>&1 </dev/null || { cat "$dir/launch.build.txt" >&2; die "cannot build the launcher"; }
	cp "$dir/.stub" "$dir/sh" || die "cannot create $dir/sh"
	: >"$dir/utilities"
	for m in "$modroot"/xcu/*; do
		[ -d "$m" ] || continue
		u=${m##*/}
		# chapter modules (1.1-..., 2.2-...), the sh-* feature modules and
		# the two special-character names are not utilities
		case $u in [0-9]* | sh-* | colon | dot) continue ;; esac
		ln "$dir/.stub" "$dir/bin/$u" 2>/dev/null || cp "$dir/.stub" "$dir/bin/$u" ||
		    die "cannot create $dir/bin/$u"
		printf '%s\n' "$u" >>"$dir/utilities"
	done

	# 3. the compiler wrapper: a test linked with pc.o gets the launcher
	{
		printf '#!/bin/sh\n# written by ci/nullsys.sh build; see docs/negative-control.md\n'
		printf 'real=%s\ndir=%s\n' "'$cc'" "'$dir'"
		cat <<'W'
istest=no only=no out= prev=
for a in "$@"; do
	case $prev in -o) out=$a ;; esac
	prev=$a
	case $a in
	-c | -E | -S | -fsyntax-only) only=yes ;;
	pc.o | */pc.o) istest=yes ;;
	esac
done
if [ "$istest" = yes ] && [ "$only" = no ] && [ -n "$out" ]; then
	# The stubs can only catch calls that really call: no inlined builtins,
	# no fortified variants. Retried without if the compiler objects.
	"$real" "$@" -fno-builtin -U_FORTIFY_SOURCE 2>/dev/null || "$real" "$@" || exit $?
	mv -f "$out" "$out.real" && cp "$dir/.launch" "$out" && chmod 755 "$out"
	exit $?
fi
exec "$real" "$@"
W
	} >"$dir/cc"
	chmod 755 "$dir/cc"

	if [ "$poison" = yes ]; then echo poison >"$dir/MODE"; else echo default >"$dir/MODE"; fi
	chmod -R a+rX "$dir"
	n=$(grep -c '^' "$dir/utilities")
	printf 'nullsys: built %s (%s stubs, %s utilities, mode %s)\n' "$dir" \
	    "$(sed -n 's/^.* \([0-9][0-9]*\) stubs.*$/\1/p' "$dir/nullsys.c" | head -1)" "$n" "$(cat "$dir/MODE")"
	printf 'nullsys: not generated: see %s/nullsys.gen.txt\n' "$dir"
}

# ---------- report ----------

# The suspicious set of a finished run. $1 modules root, $2 results dir.
do_report() {
	rroot=$1 rres=$2
	[ -f "$rres/run.log" ] || die "no run.log in $rres"
	[ -f "$rres/nullsys.calls" ] || : >"$rres/nullsys.calls"

	# id <tab> module <tab> says, from every assertion file
	awk -v root="$rroot" '
	function flush() { if (id != "") printf "%s\t%s\t%s\n", id, mod, says; id = ""; says = "" }
	FNR == 1 { flush(); insays = 0; n = split(FILENAME, p, "/"); mod = p[n - 2] "/" p[n - 1] }
	/^assertion[ \t]/ { flush(); id = $2; insays = 0; next }
	/^says[ \t]/ { s = $0; sub(/^says[ \t]+/, "", s); says = s; insays = 1; next }
	/^[ \t]/ && insays { s = $0; sub(/^[ \t]+/, "", s); says = says " " s; next }
	{ insays = 0 }
	END { flush() }' "$rroot"/*/*/*.asr >"$rres/nullsys.asr.tsv"
	sort -u "$rres/nullsys.calls" >"$rres/nullsys.calls.u"

	{
		printf 'negative control: %s\n' "$(cat "$rres/nullsys.info" 2>/dev/null || echo unknown)"
		awk -F '\t' '/^#/ || NF < 2 { next } { c[$1]++; t++ }
		    END { printf "verdicts of %d assertions:", t; for (k in c) printf " %s %d,", k, c[k]; printf "\n" }' \
		    "$rres/run.log" | sed 's/,$//'
		awk -F '\t' '
		FILENAME == ARGV[1] { mod[$1] = $2; says[$1] = $3; next }
		FILENAME == ARGV[2] {
			split($0, c, " ")
			called[c[1] SUBSEP c[2]] = 1
			if (!(c[1] SUBSEP c[2] in listed)) {
				listed[c[1] SUBSEP c[2]] = 1
				if (cnt[c[1]]++ == 0) fns[c[1]] = c[2]
				else fns[c[1]] = fns[c[1]] "," c[2]
			}
			next
		}
		/^#/ || NF < 2 { next }
		{
			code = $1; id = $2
			if (code ~ /^(FAIL|INSPECT|UNRESOLVED|UNSUPPORTED|NOTINUSE|UNINITIATED|UNTESTED)$/) next
			m = (id in mod) ? mod[id] : "?/?"
			split(m, mp, "/")
			vol = mp[1]; base = mp[2]
			tag = (code == "PASS") ? "" : " [" code "]"
			line = sprintf("%-26s %s%s\n    %s\n", m, id, tag, (id in says) ? says[id] : "")
			if (vol == "xbd") { nx++; X = X line; next }
			subject = (base !~ /^[0-9]/ && !(vol == "xcu" && base ~ /^sh-/))
			if (subject && (id SUBSEP base in called)) { nb++; B = B line; next }
			if (!subject && (id in cnt)) {
				nb++
				B = B sprintf("%-26s %s%s   (no single subject; called: %s)\n    %s\n", m, id, tag, fns[id], says[id])
				next
			}
			na++; A = A line
		}
		END {
			printf "\n== (a) PASS and the subject was never called: the test never exercised it (%d)\n%s", na, A
			printf "\n== (b) PASS although the subject was called: the test accepts a garbage result (%d)\n%s", nb, B
			printf "\n== xbd: header and compile-only modules, cannot be stubbed at run time (%d)\n%s", nx, X
		}' "$rres/nullsys.asr.tsv" "$rres/nullsys.calls.u" "$rres/run.log"
	} | tee "$rres/nullsys-suspicious.txt"
	printf '\nnullsys: the list is also in %s/nullsys-suspicious.txt\n' "$rres" >&2
}

# ---------- run ----------

do_run() {
	flip=no realsh=no
	while [ $# -gt 0 ]; do
		case $1 in
		--flip) flip=yes; shift ;;
		--real-sh) realsh=yes; shift ;;
		-*) die "run: unknown option $1 (options for posixcheck.sh go after DIR)" ;;
		*) break ;;
		esac
	done
	[ $# -ge 1 ] || die "usage: ci/nullsys.sh run [--flip] [--real-sh] DIR [posixcheck.sh options and modules]"
	dir=$1
	shift
	[ -f "$dir/libnullsys.so" ] || die "$dir is not built (ci/nullsys.sh build $dir)"
	dir=$(cd "$dir" && pwd)

	# Find --results and --root among the options, keeping all arguments.
	res= root=$here/modules want=
	n=$#
	while [ "$n" -gt 0 ]; do
		a=$1
		shift
		n=$((n - 1))
		case $want in
		res) res=$a; want= ;;
		root) root=$a; want= ;;
		*) case $a in --results) want=res ;; --root) want=root ;; esac ;;
		esac
		set -- "$@" "$a"
	done
	suffix=
	[ "$flip" = yes ] && suffix=$suffix-flip
	[ "$realsh" = yes ] && suffix=$suffix-realsh
	addres=no
	if [ -z "$res" ]; then
		res=$dir/results$suffix
		addres=yes
		[ ! -e "$res/run.log" ] || die "$res already has a run.log; remove it or give --results"
	fi
	mkdir -p "$res" || die "cannot create $res"
	res=$(cd "$res" && pwd)
	case $root in /*) ;; *) root=$PWD/$root ;; esac

	log=$res/nullsys.calls
	: >"$log" && chmod 666 "$log"
	PC_NULLSYS_LIB=$dir/libnullsys.so
	PC_NULLSYS_BIN=$dir/bin
	PC_NULLSYS_LOG=$log
	export PC_NULLSYS_LIB PC_NULLSYS_BIN PC_NULLSYS_LOG
	if [ "$flip" = yes ]; then
		PC_NULLSYS_FLIP=1
		export PC_NULLSYS_FLIP
	fi
	if [ "$realsh" = yes ]; then
		# the real shell: sh on the system's own _CS_PATH
		oldifs=$IFS
		IFS=:
		set -f
		for d in $(getconf PATH); do
			if [ -x "$d/sh" ]; then PC_NULLSYS_REALSH=$d/sh; break; fi
		done
		set +f
		IFS=$oldifs
		[ -n "${PC_NULLSYS_REALSH:-}" ] || die "no sh on the real _CS_PATH"
		export PC_NULLSYS_REALSH
	fi
	printf '%s%s%s\n' "$(cat "$dir/MODE")" "$([ "$flip" = yes ] && echo ', stub shell and utilities exit 1 and print error')" \
	    "$([ "$realsh" = yes ] && echo ', real shell with stub utilities first on PATH')" >"$res/nullsys.info"

	drv=${NULLSYS_DRIVER_SH:-sh}
	[ "$addres" = yes ] && set -- --results "$res" "$@"
	set -- --no-selftest --cc "$dir/cc" --sut-sh "$dir/sh" "$@"
	printf 'nullsys: running the suite against %s (%s); results in %s\n' "$dir" "$(cat "$res/nullsys.info")" "$res" >&2
	"$drv" "$here/posixcheck.sh" "$@" >"$res/nullsys.stdout" 2>"$res/nullsys.stderr" </dev/null
	rc=$?
	printf 'nullsys: posixcheck.sh ended with status %s (driver output in %s/nullsys.stdout)\n' "$rc" "$res" >&2
	[ -f "$res/run.log" ] || { cat "$res/nullsys.stderr" >&2; die "the run produced no run.log"; }
	do_report "$root" "$res"
}

cmd=${1:-}
[ $# -gt 0 ] && shift
case $cmd in
build) do_build "$@" ;;
run) do_run "$@" ;;
report)
	root=$here/modules
	if [ "${1:-}" = --root ]; then root=$2; shift 2; fi
	[ $# -eq 2 ] || die "usage: ci/nullsys.sh report [--root DIR] DIR RESULTS"
	[ -f "$2/nullsys.info" ] || printf 'unknown\n' >"$2/nullsys.info" 2>/dev/null
	do_report "$root" "$2"
	;;
*) die "usage: ci/nullsys.sh build|run|report ... (see the top of this file)" ;;
esac
