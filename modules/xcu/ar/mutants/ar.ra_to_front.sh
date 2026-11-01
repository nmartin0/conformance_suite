# ar -r -a puts the new member at the front
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -r) case $2 in -a)
	first=$("$REAL" -t "$4" | sed -n 1p)
	a=$1; b=$2; shift 3
	[ -n "$first" ] && exec "$REAL" "$a" -b "$first" "$@" ;;
	esac ;; esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
