# ar -q puts the files before the existing members
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -q)
	first=
	[ -e "$2" ] && first=$("$REAL" -t "$2" | sed -n 1p)
	arch=$2; shift 2
	"$REAL" -q "$arch" "$@" || exit 1
	if [ -n "$first" ]; then for x; do "$REAL" -m -b "$first" "$arch" "${x##*/}" >/dev/null 2>&1; done; fi
	exit 0 ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
