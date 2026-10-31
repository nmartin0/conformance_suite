# head -c number goes on to the end of the line it stops in
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone head, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
mode=n n=10 hasopt=0
case ${1:-} in -n|-c) mode=${1#-}; n=$2; hasopt=1; shift 2 ;; esac
if [ $hasopt = 1 ] && [ $mode = c ] && [ $# -eq 1 ]; then
	lines=$("$REAL" -c "$n" "$1" | wc -l)
	exec "$REAL" -n $((lines + 1)) "$1"
fi
exec "$REAL" -$mode "$n" "$@"
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
