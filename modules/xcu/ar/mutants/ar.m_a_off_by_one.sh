# ar -m -a puts the member one place too far
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -m) case $2 in -a)
	next=$("$REAL" -t "$4" | sed -n "/^$3\$/{n;p;}")
	if [ -n "$next" ]; then a=$1; b=$2; shift 3; exec "$REAL" "$a" "$b" "$next" "$@"; fi
	;; esac ;; esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
