# ar -r -i puts the new member after the last member
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -r) case $2 in -i)
	last=$("$REAL" -t "$4" | sed -n '$p')
	a=$1; shift 3
	[ -n "$last" ] && exec "$REAL" "$a" -a "$last" "$@" ;;
	esac ;; esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
