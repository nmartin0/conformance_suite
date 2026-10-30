#!/bin/sh
# run.sh - run the ABI conformance checks for one architecture.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: sh abi/run.sh ARCH [--cc CMD] [driver options] [module ...]
#
#   ARCH      the architecture to check: i386
#   --cc CMD  the system C compiler to build for ARCH with (default: $CC, else cc);
#             run.sh adds the flag that selects the architecture
#
# Everything else goes to posixcheck.sh unchanged, for example
#   sh abi/run.sh i386 --results /tmp/abi-i386-1
#   sh abi/run.sh i386 --only ABI.i386.entry.006 --results /tmp/x
#   sh abi/run.sh i386 i386/entry i386/types        (modules to run)
# The driver is given only the ABI module tree (abi/modules), so none of the
# POSIX modules run. Without module arguments, every module of ARCH runs;
# a test built for a different architecture reports NOTINUSE.
#
# The driver runs one command as its compiler. A compiler needing a flag
# (gcc -m32) is wrapped in a one-line script in a private directory, so the
# driver itself needs no change.

set -u
LC_ALL=POSIX
export LC_ALL

case $0 in
*/*) abi_dir=${0%/*} ;;
*) abi_dir=. ;;
esac
abi_dir=$(cd "$abi_dir" && pwd) || exit 2
root=$(cd "$abi_dir/.." && pwd) || exit 2

if [ $# -lt 1 ]; then
	sed -n '2,/^$/p' "$0" | sed 's/^# \{0,1\}//' >&2
	exit 2
fi
arch=$1
shift
case $arch in
i386) archflag=-m32 ;;
*)
	printf 'run.sh: unknown architecture %s (known: i386)\n' "$arch" >&2
	exit 2
	;;
esac

base_cc=${CC:-cc}
args=
n=0
while [ $# -gt 0 ]; do
	case $1 in
	--cc)
		[ $# -ge 2 ] || { printf 'run.sh: --cc needs an argument\n' >&2; exit 2; }
		base_cc=$2
		shift 2
		;;
	*)
		# keep every other argument as it is, spaces and all
		n=$((n + 1))
		eval "arg_$n=\$1"
		shift
		;;
	esac
done

tmp=$(mktemp -d "${TMPDIR:-/tmp}/abi-run.XXXXXX") || exit 2
trap 'rm -rf "$tmp"' EXIT HUP INT TERM
printf '#!/bin/sh\nexec %s %s "$@"\n' "$base_cc" "$archflag" >"$tmp/cc"
chmod +x "$tmp/cc"

i=1
set --
while [ "$i" -le "$n" ]; do
	eval "set -- \"\$@\" \"\$arg_$i\""
	i=$((i + 1))
done

# A system that cannot build or run programs for ARCH has nothing to check:
# say so and stop, instead of letting the driver abort on its own compiler probe.
printf 'int main(void) { return 0; }\n' >"$tmp/probe.c"
if ! "$tmp/cc" -o "$tmp/probe" "$tmp/probe.c" >"$tmp/probe.err" 2>&1 || ! "$tmp/probe" >/dev/null 2>&1; then
	printf 'NOTINUSE    ABI.%s (every module): this system cannot build and run %s programs with %s %s\n' \
	    "$arch" "$arch" "$base_cc" "$archflag"
	printf 'SUMMARY total 0 PASS 0 FAIL 0 INSPECT 0 UNRESOLVED 0 NOTINUSE 0 UNSUPPORTED 0 UNINITIATED 0\n'
	exit 0
fi

"$root/posixcheck.sh" --cc "$tmp/cc" --root "$abi_dir/modules" "$@"
rc=$?
exit "$rc"
