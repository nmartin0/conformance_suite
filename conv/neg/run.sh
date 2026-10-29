#!/bin/sh
# run.sh - negative checks for the conventions suite.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: sh conv/neg/run.sh          (NEG_VERBOSE=1 also prints each result line)
#
# For each area, points a check at a deliberately wrong fixture (a runtime
# directory of mode 0755, a /tmp without the sticky bit, a passwd line with
# too few fields, a TZif file with unsorted transitions, ...) or plants a
# fault in a library call, and shows that the check reports it: FAIL for a
# FORMAL assertion, INSPECT for a CONVENTION one. A control with a correct
# fixture shows the fixture is not what makes the check fail. Prints one
# line per case, "ok" when the result is the expected one and "MISSED"
# otherwise; the exit status is 1 if any case was MISSED.
#
# Fixtures live in a private temporary directory removed at the end. The
# checks themselves run through conv/run.sh with the PC_CONV_* overrides
# listed in conv/README.md.

case $0 in
*/*) here=${0%/*} ;;
*) here=. ;;
esac
top=$(cd "$here/../.." && pwd) || exit 2
tmp=$(mktemp -d "${TMPDIR:-/tmp}/convneg.XXXXXX") || exit 2
trap 'chmod -R u+rwx "$tmp" 2>/dev/null; rm -rf "$tmp"' EXIT HUP INT TERM
fx=$tmp/fx
mkdir "$fx" || exit 2
missed=0
n=0

# case EXPECT ID DESCRIPTION [VAR=value ...] [-- driver options]
run() {
	expect=$1 id=$2 desc=$3
	shift 3
	n=$((n + 1))
	set -- "$@"
	envs=
	while [ $# -gt 0 ] && [ "$1" != -- ]; do
		envs="$envs $1"
		shift
	done
	[ $# -gt 0 ] && shift
	line=$(env $envs sh "$top/conv/run.sh" --no-selftest --results "$tmp/r$n" --only "$id" "$@" 2>/dev/null |
	    sed -n '1p')
	got=${line%% *}
	if [ "$got" = "$expect" ]; then
		printf 'ok      %-8s %s -> %s\n' "$id" "$desc" "$got"
		[ -z "${NEG_VERBOSE:-}" ] || printf '          %s\n' "$line"
	else
		printf 'MISSED  %-8s %s -> %s (expected %s)\n' "$id" "$desc" "${got:-nothing}" "$expect"
		missed=$((missed + 1))
	fi
}

# ---------- xdg ----------
mkdir -m 0700 "$fx/rt-good"
mkdir -m 0755 "$fx/rt-open"
mkdir "$fx/emptyroot"
run PASS    CONV.xdg.001 "control: runtime dir 0700" PC_CONV_XDG_RUNTIME_DIR="$fx/rt-good"
run INSPECT CONV.xdg.001 "runtime dir with mode 0755" PC_CONV_XDG_RUNTIME_DIR="$fx/rt-open"
run PASS    CONV.xdg.002 "control: runtime dir 0700" PC_CONV_XDG_RUNTIME_DIR="$fx/rt-good"
run INSPECT CONV.xdg.002 "link() that does nothing" PC_CONV_XDG_RUNTIME_DIR="$fx/rt-good" -- --mutant link.noop
run INSPECT CONV.xdg.003 "mkdir() that ignores the mode" -- --mutant mkdir.loose
run INSPECT CONV.xdg.004 "no /usr/share" PC_CONV_ROOT="$fx/emptyroot"

# ---------- layout ----------
mkdir -p "$fx/root-good/tmp" "$fx/root-good/var/tmp" "$fx/root-good/bin" "$fx/root-good/usr/bin"
chmod 1777 "$fx/root-good/tmp" "$fx/root-good/var/tmp"
: >"$fx/root-good/bin/sh"; : >"$fx/root-good/usr/bin/env"
chmod 755 "$fx/root-good/bin/sh" "$fx/root-good/usr/bin/env"
mkdir -p "$fx/root-bad/tmp" "$fx/root-bad/var/tmp" "$fx/root-bad/bin" "$fx/root-bad/usr/bin"
chmod 0755 "$fx/root-bad/tmp"; chmod 0777 "$fx/root-bad/var/tmp"
: >"$fx/root-bad/bin/sh"; : >"$fx/root-bad/usr/bin/env"
chmod 644 "$fx/root-bad/bin/sh" "$fx/root-bad/usr/bin/env"
run INSPECT CONV.layout.001 "root without the essential directories" PC_CONV_ROOT="$fx/emptyroot"
run INSPECT CONV.layout.002 "root without /usr/lib and the rest" PC_CONV_ROOT="$fx/emptyroot"
run PASS    CONV.layout.003 "control: /tmp 1777" PC_CONV_ROOT="$fx/root-good"
run PASS    CONV.layout.004 "control: /tmp 1777" PC_CONV_ROOT="$fx/root-good"
run PASS    CONV.layout.006 "control: /var/tmp 1777" PC_CONV_ROOT="$fx/root-good"
run INSPECT CONV.layout.003 "/tmp 0755" PC_CONV_ROOT="$fx/root-bad"
run INSPECT CONV.layout.004 "/tmp without the sticky bit" PC_CONV_ROOT="$fx/root-bad"
run INSPECT CONV.layout.005 "no /var/tmp at all" PC_CONV_ROOT="$fx/emptyroot"
run INSPECT CONV.layout.006 "/var/tmp 0777 without the sticky bit" PC_CONV_ROOT="$fx/root-bad"
run INSPECT CONV.layout.009 "/bin/sh not executable" PC_CONV_ROOT="$fx/root-bad"
run INSPECT CONV.layout.010 "/usr/bin/env not executable" PC_CONV_ROOT="$fx/root-bad"
printf 'u:x:1000:1000::/no/such/home:/bin/sh\n' >"$fx/pw-nohome"
printf '/bin/sh\n' >"$fx/shells-sh"
run INSPECT CONV.layout.007 "login account whose home does not exist" PC_CONV_PASSWD="$fx/pw-nohome" PC_CONV_SHELLS="$fx/shells-sh"

# ---------- devnodes ----------
mkdir "$fx/dev"
: >"$fx/dev/null"; chmod 644 "$fx/dev/null"
: >"$fx/dev/tty"; chmod 600 "$fx/dev/tty"
i=0; while [ $i -lt 3000 ]; do printf 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'; i=$((i + 1)); done >"$fx/dev/zero"
chmod 666 "$fx/dev/zero"
cp "$fx/dev/zero" "$fx/dev/urandom"
ln -s /etc/passwd "$fx/dev/stdin"
run PASS    CONV.devnodes.001 "control: the real /dev"
run INSPECT CONV.devnodes.001 "null, zero and tty that are plain files" PC_CONV_DEV="$fx/dev"
run INSPECT CONV.devnodes.002 "zero that does not read as zeros" PC_CONV_DEV="$fx/dev"
run INSPECT CONV.devnodes.003 "nodes with restrictive modes" PC_CONV_DEV="$fx/dev"
run INSPECT CONV.devnodes.004 "urandom that is not a character device" PC_CONV_DEV="$fx/dev"
run INSPECT CONV.devnodes.006 "/dev/stdin that is another file" PC_CONV_DEV="$fx/dev"
run INSPECT CONV.devnodes.007 "slave name outside the expected directory" PC_CONV_PTY_PREFIX=/no/such/dir/

# ---------- etcdb ----------
printf 'root:x:0:0:r:/root:/bin/sh\nshort:x:1:1:only:five\n' >"$fx/pw1"
printf 'a:x:7:1::/h:/bin/sh\nb:x:7:1::/h:/bin/sh\n' >"$fx/pw2"
printf 'a:x:7:1::home:/bin/sh\n' >"$fx/pw3"
printf 'g:x:1\n' >"$fx/gr1"
printf 'bin/sh\n' >"$fx/sh1"
printf 'root:x:0:0::/root:/bin/not-listed\n' >"$fx/pw4"
printf 'notanaddress localhost\n' >"$fx/h1"
printf '10.0.0.1 somehost\n' >"$fx/h2"
printf 'echo 7\n' >"$fx/s1"
printf 'tcp 999\n' >"$fx/p1"
printf 'tcp 7\nudp 17\nicmp 1\n' >"$fx/p2"
printf 'frobnicate 1.2.3.4\nnameserver not-an-address\n' >"$fx/r1"
run INSPECT CONV.etcdb.001 "passwd line with five fields" PC_CONV_PASSWD="$fx/pw1"
run INSPECT CONV.etcdb.002 "two accounts with one uid" PC_CONV_PASSWD="$fx/pw2"
run INSPECT CONV.etcdb.003 "relative home directory" PC_CONV_PASSWD="$fx/pw3"
run INSPECT CONV.etcdb.004 "group line with three fields" PC_CONV_GROUP="$fx/gr1"
run INSPECT CONV.etcdb.006 "relative path in the shells file" PC_CONV_SHELLS="$fx/sh1"
run INSPECT CONV.etcdb.007 "root's shell not in the shells file" PC_CONV_PASSWD="$fx/pw4" PC_CONV_SHELLS="$fx/shells-sh"
run INSPECT CONV.etcdb.008 "hosts line whose address is not one" PC_CONV_HOSTS="$fx/h1"
run INSPECT CONV.etcdb.009 "hosts file without a localhost entry" PC_CONV_HOSTS="$fx/h2"
run INSPECT CONV.etcdb.011 "services line without port/protocol" PC_CONV_SERVICES="$fx/s1"
run INSPECT CONV.etcdb.013 "protocol number above 255" PC_CONV_PROTOCOLS="$fx/p1"
run INSPECT CONV.etcdb.014 "tcp numbered 7" PC_CONV_PROTOCOLS="$fx/p2"
run INSPECT CONV.etcdb.015 "unknown directive and a bad nameserver" PC_CONV_RESOLV="$fx/r1"

# name-service order (second pass): another source ahead of the file makes the file comparison not applicable
printf 'passwd: files\ngroup: files\nhosts: files dns\nservices: files\nprotocols: files\n' >"$fx/nss-files"
printf 'passwd: ldap files\ngroup: ldap files\nhosts: dns files\nservices: ldap files\nprotocols: ldap files\n' >"$fx/nss-other"
run PASS     CONV.etcdb.005 "control: files listed first" PC_CONV_NSSWITCH="$fx/nss-files"
run NOTINUSE CONV.etcdb.005 "another source listed ahead of the files" PC_CONV_NSSWITCH="$fx/nss-other"
run NOTINUSE CONV.etcdb.012 "another source listed ahead of the services file" PC_CONV_NSSWITCH="$fx/nss-other"
printf 'tcp 6\nudp 17\nicmp 1\n' >"$fx/p3"
run PASS    CONV.etcdb.014 "control: well-formed protocols file" PC_CONV_PROTOCOLS="$fx/p3"
printf 'echo 7/Tcp-x\n' >"$fx/s2"
printf 'echo 7/\n' >"$fx/s3"
run PASS    CONV.etcdb.011 "control: a protocol name that is not lower case is not asserted" PC_CONV_SERVICES="$fx/s2"
run INSPECT CONV.etcdb.011 "services line with an empty protocol name" PC_CONV_SERVICES="$fx/s3"

# ---------- tzif ----------
if cc -o "$tmp/mktz" "$top/conv/neg/mktz.c" 2>"$tmp/mktz.err"; then
	"$tmp/mktz" "$fx/tz" || exit 2
	for t in 1 2 3 4; do
		run PASS CONV.tzif.00$t "control: well-formed fixture database" PC_CONV_TZDIR="$fx/tz/good"
	done
	run FAIL    CONV.tzif.001 "undefined version byte" PC_CONV_TZDIR="$fx/tz/version"
	run FAIL    CONV.tzif.001 "file cut short" PC_CONV_TZDIR="$fx/tz/short"
	run FAIL    CONV.tzif.001 "footer without its final newline" PC_CONV_TZDIR="$fx/tz/nofooternl"
	run FAIL    CONV.tzif.002 "transitions not ascending" PC_CONV_TZDIR="$fx/tz/unsorted"
	run FAIL    CONV.tzif.002 "type index beyond typecnt" PC_CONV_TZDIR="$fx/tz/typeidx"
	run FAIL    CONV.tzif.003 "footer inconsistent with the last transition" PC_CONV_TZDIR="$fx/tz/footer"
	run INSPECT CONV.tzif.004 "utoff outside the recommended range" PC_CONV_TZDIR="$fx/tz/utoff"
else
	echo "MISSED  tzif     cannot build conv/neg/mktz.c: $(cat "$tmp/mktz.err")"
	missed=$((missed + 1))
fi
run FAIL    CONV.tzif.005 "localtime() an hour off" -- --mutant localtime.shifted

# ---------- utf8 ----------
run FAIL CONV.utf8.002 "mbrtowc accepting C0 80" -- --mutant mbrtowc.overlong
run FAIL CONV.utf8.003 "mbrtowc accepting a surrogate" -- --mutant mbrtowc.surrogate
run FAIL CONV.utf8.004 "mbrtowc accepting F4 90 80 80" -- --mutant mbrtowc.above
run FAIL CONV.utf8.005 "mbrtowc accepting a truncated sequence" -- --mutant mbrtowc.truncated
run FAIL CONV.utf8.006 "mbrtowc accepting a lone continuation byte" -- --mutant mbrtowc.stray
run FAIL CONV.utf8.007 "wcrtomb writing an overlong form" -- --mutant wcrtomb.long
run FAIL CONV.utf8.008 "wcrtomb encoding a surrogate" -- --mutant wcrtomb.surrogate

# ---------- toolchain ----------
# Each case substitutes a deliberately wrong tool (a wrapper around the
# system's own that damages one thing) for the assembler, linker or compiler
# and shows the checks flag it. The controls use a wrapper that damages
# nothing.
tcdir=$fx/tc
mkdir "$tcdir"
real_as=$(command -v as); real_ld=$(command -v ld)
real_cc=$(command -v c17 2>/dev/null || command -v cc)
# mkas NAME 'sed-expression' [file-name-glob]: an assembler that edits its input
mkas() {
	cat >"$tcdir/$1" <<EOT
#!/bin/sh
# usage as -o OUT IN
case "\$3" in ${3:-*}) sed '$2' "\$3" >"\$3.edit" && exec $real_as -o "\$2" "\$3.edit" ;; esac
exec $real_as "\$@"
EOT
	chmod +x "$tcdir/$1"
}
# mkld NAME 'shell code run with the arguments in "$@"': a linker whose arguments are altered
mkld() {
	{ printf '#!/bin/sh\n%s\nexec %s "$@"\n' "$2" "$real_ld"; } >"$tcdir/$1"
	chmod +x "$tcdir/$1"
}
mkcc() {
	{ printf '#!/bin/sh\n%s\nexec %s "$@"\n' "$2" "$real_cc"; } >"$tcdir/$1"
	chmod +x "$tcdir/$1"
}
# drop OPTION-WITH-ARGUMENT from "$@"
dropopt() { printf 'n=$#; while [ $n -gt 0 ]; do a=$1; shift; if [ "$a" = %s ]; then shift; n=$((n - 1)); else set -- "$@" "$a"; fi; n=$((n - 1)); done' "$1"; }
dropflag() { printf 'n=$#; while [ $n -gt 0 ]; do a=$1; shift; [ "$a" = %s ] || set -- "$@" "$a"; n=$((n - 1)); done' "$1"; }
repl() { printf 'n=$#; while [ $n -gt 0 ]; do a=$1; shift; case $a in %s) a=%s ;; esac; set -- "$@" "$a"; n=$((n - 1)); done' "$1" "$2"; }

# assemblers: wrappers that apply a sed expression to the input first
cat >"$tcdir/as-pass" <<EOT
#!/bin/sh
exec $real_as "\$@"
EOT
chmod +x "$tcdir/as-pass"
mkasfix() { # NAME EXPR
	cat >"$tcdir/$1" <<EOT
#!/bin/sh
sed '$2' "\$3" >"\$3.edit" && exec $real_as -o "\$2" "\$3.edit"
EOT
	chmod +x "$tcdir/$1"
}
mkasfix as-bss 's/^	\.bss/	.data/'
mkasfix as-flags 's/"aw"/"a"/'
mkasfix as-weak 's/\.weak/.globl/'
mkasfix as-size 's/\.size tco, 4/.size tco, 5/'
mkasfix as-comm 's/tcc1,24,16/tcc1,24,8/'
mkasfix as-commsmall 's/tcc,24,8/tcc,4,8/'
cat >"$tcdir/as-noinit" <<EOT
#!/bin/sh
case "\$3" in binit.s) sed 's/\.init_array/.fini_array/' "\$3" >"\$3.edit" && exec $real_as -o "\$2" "\$3.edit" ;; esac
exec $real_as "\$@"
EOT
chmod +x "$tcdir/as-noinit"
run PASS    CONV.toolchain.001 "control: assembler wrapper that changes nothing" PC_CONV_AS="$tcdir/as-pass"
run INSPECT CONV.toolchain.001 "assembler that puts .bss into .data" PC_CONV_AS="$tcdir/as-bss"
run INSPECT CONV.toolchain.002 "assembler that drops the writable flag" PC_CONV_AS="$tcdir/as-flags"
run INSPECT CONV.toolchain.003 "assembler that makes .weak symbols global" PC_CONV_AS="$tcdir/as-weak"
run INSPECT CONV.toolchain.004 "assembler that records a wrong .size" PC_CONV_AS="$tcdir/as-size"
run INSPECT CONV.toolchain.005 "assembler that loses the .comm alignment" PC_CONV_AS="$tcdir/as-comm"
run INSPECT CONV.toolchain.012 "merged common symbol smaller than the largest" PC_CONV_AS="$tcdir/as-commsmall"
run INSPECT CONV.toolchain.009 "a weak definition treated as strong (assembler)" PC_CONV_AS="$tcdir/as-weak"
run INSPECT CONV.toolchain.021 "a dependency whose init function is never registered" PC_CONV_AS="$tcdir/as-noinit"
# linkers
mkld ld-pass ''
mkld ld-noout "$(dropopt -o)"
mkld ld-static "$(repl '-lx' '-Bstatic -lx')"
# -lx must become two arguments: -Bstatic then -lx; build it by hand
cat >"$tcdir/ld-static" <<EOT
#!/bin/sh
n=\$#; while [ \$n -gt 0 ]; do a=\$1; shift; if [ "\$a" = -lx ]; then set -- "\$@" -Bstatic -lx; else set -- "\$@" "\$a"; fi; n=\$((n - 1)); done
exec $real_ld "\$@"
EOT
chmod +x "$tcdir/ld-static"
mkld ld-whole 'set -- --whole-archive "$@"'
mkld ld-group 'set -- --start-group "$@" --end-group'
mkld ld-muldefs 'set -- -z muldefs "$@"'
mkld ld-ignore 'set -- --unresolved-symbols=ignore-all "$@"'
mkld ld-warncommon 'set -- --warn-common --fatal-warnings "$@"'
mkld ld-noshared "$(dropflag -shared)"
mkld ld-nosoname "$(dropopt -soname)"
mkld ld-norpath "$(dropopt -rpath)"
mkld ld-entry "$(repl_e=1; printf 'n=$#; while [ $n -gt 0 ]; do a=$1; shift; if [ "$a" = -e ]; then shift; set -- "$@" -e tcfirst; n=$((n - 1)); else set -- "$@" "$a"; fi; n=$((n - 1)); done')"
mkld ld-nor "$(dropflag -r)"
run PASS    CONV.toolchain.006 "control: linker wrapper that changes nothing" PC_CONV_LD="$tcdir/ld-pass"
run INSPECT CONV.toolchain.006 "linker that ignores -o" PC_CONV_LD="$tcdir/ld-noout"
run INSPECT CONV.toolchain.007 "linker that prefers the archive" PC_CONV_LD="$tcdir/ld-static"
run INSPECT CONV.toolchain.008 "linker that extracts every archive member" PC_CONV_LD="$tcdir/ld-whole"
run INSPECT CONV.toolchain.008 "archives in the wrong order are accepted (grouped)" PC_CONV_LD="$tcdir/ld-group"
run INSPECT CONV.toolchain.010 "linker that accepts duplicate definitions" PC_CONV_LD="$tcdir/ld-muldefs"
run INSPECT CONV.toolchain.011 "linker that ignores undefined references" PC_CONV_LD="$tcdir/ld-ignore"
run INSPECT CONV.toolchain.012 "linker that treats merged commons as an error" PC_CONV_LD="$tcdir/ld-warncommon"
run INSPECT CONV.toolchain.013 "linker whose -shared makes no shared object" PC_CONV_LD="$tcdir/ld-noshared"
run INSPECT CONV.toolchain.014 "linker that drops -soname" PC_CONV_LD="$tcdir/ld-nosoname"
run INSPECT CONV.toolchain.015 "linker that drops -rpath" PC_CONV_LD="$tcdir/ld-norpath"
run INSPECT CONV.toolchain.016 "linker that ignores the -e symbol" PC_CONV_LD="$tcdir/ld-entry"
run INSPECT CONV.toolchain.017 "linker whose -r does not combine" PC_CONV_LD="$tcdir/ld-nor"
run NOTINUSE CONV.toolchain.006 "no linker" PC_CONV_LD="$tcdir/absent"
run NOTINUSE CONV.toolchain.001 "no assembler" PC_CONV_AS="$tcdir/absent"
# compiler driver / loader behaviour
mkcc cc-pass ''
cat >"$tcdir/cc-origin" <<EOT
#!/bin/sh
n=\$#; while [ \$n -gt 0 ]; do a=\$1; shift; case \$a in *ORIGIN*) a="-Wl,-rpath,\$PWD/t1/lib" ;; esac; set -- "\$@" "\$a"; n=\$((n - 1)); done
exec $real_cc "\$@"
EOT
cat >"$tcdir/cc-noenv" <<EOT
#!/bin/sh
# a program that does not see LD_LIBRARY_PATH: the real program is wrapped in a script that unsets it
$real_cc "\$@" || exit \$?
out=
n=\$#; while [ \$n -gt 0 ]; do a=\$1; shift; [ "\$out" = next ] && out=\$a; [ "\$a" = -o ] && out=next; set -- "\$@" "\$a"; n=\$((n - 1)); done
case \$out in prog) mv prog prog.real && printf '#!/bin/sh\nunset LD_LIBRARY_PATH\nexec ./prog.real\n' >prog && chmod +x prog ;; esac
exit 0
EOT
cat >"$tcdir/cc-symbolic" <<EOT
#!/bin/sh
case " \$* " in *" -shared "*) set -- "\$@" -Wl,-Bsymbolic ;; esac
exec $real_cc "\$@"
EOT
cat >"$tcdir/cc-nopic" <<EOT
#!/bin/sh
for a; do [ "\$a" = -fPIC ] && exit 1; done
exec $real_cc "\$@"
EOT
chmod +x "$tcdir/cc-origin" "$tcdir/cc-noenv" "$tcdir/cc-symbolic" "$tcdir/cc-nopic"
run PASS    CONV.toolchain.018 "control: compiler wrapper that changes nothing" PC_CONV_CC="$tcdir/cc-pass"
run INSPECT CONV.toolchain.018 "run path that is an absolute path instead of \$ORIGIN" PC_CONV_CC="$tcdir/cc-origin"
run INSPECT CONV.toolchain.019 "program that does not see LD_LIBRARY_PATH" PC_CONV_CC="$tcdir/cc-noenv"
run INSPECT CONV.toolchain.020 "library bound to its own definition (-Bsymbolic)" PC_CONV_CC="$tcdir/cc-symbolic"
run NOTINUSE CONV.toolchain.013 "compiler that refuses -fPIC" PC_CONV_CC="$tcdir/cc-nopic"

printf '%d cases, %d missed\n' "$n" "$missed"
[ "$missed" -eq 0 ]
