# the heading line is dropped when some but not all headings are null
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
nt=0 nn=0
c=$#
while [ "$c" -gt 0 ]; do
	a=$1; shift; c=$((c - 1))
	case $a in
	-o) v=$1; shift; c=$((c - 1)); nt=$((nt + 1)); case $v in *=) nn=$((nn + 1)) ;; esac; set -- "$@" -o "$v" ;;
	*) set -- "$@" "$a" ;;
	esac
done
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
if [ "$nn" -gt 0 ] && [ "$nn" -lt "$nt" ]; then sed 1d "$tmp"; else cat "$tmp"; fi
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
