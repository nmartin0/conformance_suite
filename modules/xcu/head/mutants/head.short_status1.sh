# head a file shorter than the count is copied whole but the status is 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone head, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
mode=n n=10 hasopt=0
case ${1:-} in -n|-c) mode=${1#-}; n=$2; hasopt=1; shift 2 ;; esac
short=0
if [ $# -eq 1 ] && [ -f "$1" ]; then
	if [ $mode = n ]; then size=$(wc -l <"$1"); else size=$(wc -c <"$1"); fi
	[ "$size" -lt "$n" ] && short=1
fi
"$REAL" -$mode "$n" "$@"
rc=$?
[ $short = 1 ] && rc=1
exit $rc
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
