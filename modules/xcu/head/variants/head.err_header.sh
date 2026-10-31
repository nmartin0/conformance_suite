# head: the header is written for an unreadable operand too, before its diagnostic (allowed: the diagnostic text, the failing status value and the header for an operand that could not be read are left to the implementation)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
mode=n n=10 hasopt=0
case ${1:-} in -n|-c) mode=${1#-}; n=$2; hasopt=1; shift 2 ;; esac
tmp=pc-mut.$$
nf=$#
rc=0 first=1
for f
do
	if [ $nf -gt 1 ]; then [ $first = 1 ] || echo; echo "==> $f <=="; fi; first=0
	if "$REAL" -$mode "$n" "$f" >"$tmp"; then cat "$tmp"; else rc=$?; fi
done
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
