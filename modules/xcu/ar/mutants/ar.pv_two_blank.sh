# ar -pv puts two blank lines after the name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -pv)
	arch=$2; shift 2
	[ $# -gt 0 ] || set -- $("$REAL" -t "$arch") || exit 1
	for x; do printf '\n<%s>\n\n\n' "$x"; "$REAL" -p "$arch" "$x" || exit 1; done
	exit 0 ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
