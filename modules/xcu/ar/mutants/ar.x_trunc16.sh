# ar -x creates files under names cut to 16 characters
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -x*)
	names=
	if [ $# -eq 2 ]; then names=$("$REAL" -t "$2"); else names=$(shift 2; echo "$@"); fi
	"$REAL" "$@"; rc=$?
	for x in $names; do
		x=${x##*/}; t=$(printf '%s' "$x" | cut -c1-16)
		[ "$x" != "$t" ] && mv "$x" "$t"
	done
	exit $rc ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
