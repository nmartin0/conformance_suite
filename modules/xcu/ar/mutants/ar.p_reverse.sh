# ar -p writes the members in reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -p|-pv)
	if [ $# -eq 2 ]; then
		rev=; for x in $("$REAL" -t "$2"); do rev="$x $rev"; done
		for x in $rev; do "$REAL" "$1" "$2" "$x" || exit 1; done
		exit 0
	fi ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
