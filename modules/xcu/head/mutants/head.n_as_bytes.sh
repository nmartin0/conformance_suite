# head -n number counts bytes instead of lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone head, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
mode=n n=10 hasopt=0
case ${1:-} in -n|-c) mode=${1#-}; n=$2; hasopt=1; shift 2 ;; esac
[ $hasopt = 1 ] && [ $mode = n ] && mode=c
exec "$REAL" -$mode "$n" "$@"
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
