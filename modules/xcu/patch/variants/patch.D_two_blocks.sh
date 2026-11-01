# a conforming patch whose -D conditional for a change is two blocks, #ifndef for the old text and #ifdef for the new
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text allows each change to be marked with a pair of single constructs; here a change is two separate blocks, one for the old lines and one for the new.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v patch)" >pc-mutbin/patch
cat >>pc-mutbin/patch <<'PC_EOF'
bflag= cflag= eflag= lflag= Nflag= nflag= Rflag= uflag= Dn= dir= ifile= ofile= pnum= pset= rfile= opnd= IN=
here=$(pwd)
while [ $# -gt 0 ]; do
	case $1 in
	-b) bflag=1; shift ;;
	-c) cflag=1; shift ;;
	-e) eflag=1; shift ;;
	-l) lflag=1; shift ;;
	-N) Nflag=1; shift ;;
	-n) nflag=1; shift ;;
	-R) Rflag=1; shift ;;
	-u) uflag=1; shift ;;
	-D) Dn=$2; shift 2 ;;
	-d) dir=$2; shift 2 ;;
	-i) ifile=$2; shift 2 ;;
	-o) ofile=$2; shift 2 ;;
	-p) pnum=$2; pset=1; shift 2 ;;
	-r) rfile=$2; shift 2 ;;
	--) shift; break ;;
	-?*) echo "patch: unknown option $1" >&2; exit 2 ;;
	*) break ;;
	esac
done
[ $# -gt 0 ] && opnd=$1
run() {
	set --
	[ -n "$bflag" ] && set -- "$@" -b
	[ -n "$cflag" ] && set -- "$@" -c
	[ -n "$eflag" ] && set -- "$@" -e
	[ -n "$lflag" ] && set -- "$@" -l
	[ -n "$Nflag" ] && set -- "$@" -N
	[ -n "$nflag" ] && set -- "$@" -n
	[ -n "$Rflag" ] && set -- "$@" -R
	[ -n "$uflag" ] && set -- "$@" -u
	[ -n "$Dn" ] && set -- "$@" -D "$Dn"
	[ -n "$dir" ] && set -- "$@" -d "$dir"
	[ -n "$ifile" ] && set -- "$@" -i "$ifile"
	[ -n "$ofile" ] && set -- "$@" -o "$ofile"
	[ -n "$pset" ] && set -- "$@" -p "$pnum"
	[ -n "$rfile" ] && set -- "$@" -r "$rfile"
	[ -n "$opnd" ] && set -- "$@" "$opnd"
	"$REAL" "$@"
}
slurp() {
	IN=$here/pc-mut.in.$$
	if [ -n "$ifile" ]; then cat "$ifile" >"$IN" 2>/dev/null || { IN=; return 1; }; else cat >"$IN"; fi
	ifile=$IN
}
finish() { rm -f "$here/pc-mut.in.$$" "$here/pc-mut.in.$$.2"; exit "$1"; }
xf() { awk "$@" "$IN" >"$IN.2" && ifile=$IN.2; }
noflags() { [ -z "$cflag$eflag$nflag$uflag" ]; }
tgt() { printf "%s" "${ofile:-$opnd}"; }
dropnl() { # remove the last character of file $1
	c=$(cat "$1"; printf x); c=${c%x}
	printf "%s" "${c%?}" >"$1"
}

run
rc=$?
t=$(tgt)
if [ -f "$t" ]; then
	awk '
	/^#ifndef / { inb = 1; stage = 0; hdr = $0; old = ""; new = ""; hasel = 0; next }
	inb && /^#else/ { stage = 1; hasel = 1; next }
	inb && /^#endif/ { if (hasel) { h2 = hdr; sub(/ifndef/, "ifdef", h2); printf "%s\n%s#endif\n%s\n%s#endif\n", hdr, old, h2, new } else printf "%s\n%s#endif\n", hdr, old; inb = 0; next }
	inb { if (stage) new = new $0 "\n"; else old = old $0 "\n"; next }
	{ print }' "$t" >"$t.pc-var" && cat "$t.pc-var" >"$t"
	rm -f "$t.pc-var"
fi
exit $rc
PC_EOF
chmod +x pc-mutbin/patch
PATH=$PWD/pc-mutbin:$PATH
