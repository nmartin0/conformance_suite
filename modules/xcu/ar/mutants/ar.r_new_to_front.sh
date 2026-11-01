# a new member is put at the front of the archive
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -r*)
	opt=$1; arch=$2; shift 2
	first=
	[ -e "$arch" ] && first=$("$REAL" -t "$arch" | sed -n 1p)
	new=
	for x; do "$REAL" -t "$arch" "$x" 2>/dev/null | grep -q . || new="$new $x"; done
	"$REAL" "$opt" "$arch" "$@"; rc=$?
	if [ -n "$first" ]; then
		for x in $new; do "$REAL" -m -b "$first" "$arch" "${x##*/}" >/dev/null 2>&1; done
	fi
	exit $rc ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
