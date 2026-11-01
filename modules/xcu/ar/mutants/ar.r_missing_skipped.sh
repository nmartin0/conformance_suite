# ar -r skips a missing file with status 0 unless it is the only one
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -r*)
	o=$1; arch=$2; shift 2; n=$#
	i=0; while [ $i -lt $n ]; do f=$1; shift; [ -e "$f" ] && set -- "$@" "$f"; i=$((i+1)); done
	[ $# -gt 0 ] || { echo "ar: nothing to add" >&2; exit 1; }
	exec "$REAL" "$o" "$arch" "$@" ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
