# ar -r -b puts the new member one place too early
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -r) case $2 in -b)
	prev=$("$REAL" -t "$4" | sed -n "/^$3\$/{g;p;};h")
	if [ -n "$prev" ]; then a=$1; b=$2; shift 3; exec "$REAL" "$a" "$b" "$prev" "$@"; fi
	;; esac ;; esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
