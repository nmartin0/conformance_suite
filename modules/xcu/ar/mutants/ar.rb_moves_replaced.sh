# ar -r -b moves a replaced member to the position
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -r) case $2 in -b)
	o=$1; p=$2; pn=$3; arch=$4; shift 4
	ex=
	for x; do "$REAL" -t "$arch" "$x" 2>/dev/null | grep -q . && ex="$ex $x"; done
	"$REAL" "$o" "$p" "$pn" "$arch" "$@"; rc=$?
	for x in $ex; do "$REAL" -m "$p" "$pn" "$arch" "${x##*/}" >/dev/null 2>&1; done
	exit $rc ;;
esac ;; esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
