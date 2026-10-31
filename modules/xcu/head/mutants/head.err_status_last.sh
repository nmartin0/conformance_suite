# head the exit status is that of the last operand only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone head, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
mode=n n=10 hasopt=0
case ${1:-} in -n|-c) mode=${1#-}; n=$2; hasopt=1; shift 2 ;; esac
tmp=pc-mut.$$
nf=$#
rc=0 first=1 idx=0
for f
do
	idx=$((idx + 1))
	
	if "$REAL" -$mode "$n" "$f" >"$tmp"; then r=0; else r=$?; fi
	if [ $r = 0 ]; then
		if [ $nf -gt 1 ]; then [ $first = 1 ] || echo; echo "==> $f <=="; fi; first=0
		cat "$tmp"
	fi
	rc=$r
done
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
