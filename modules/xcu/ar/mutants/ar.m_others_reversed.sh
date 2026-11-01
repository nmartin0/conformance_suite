# ar -m reverses the order of the members it did not move
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
"$REAL" "$@"; rc=$?
case $1 in -m)
	arch=$2; case $2 in -a|-b|-i) arch=$4 ;; esac
	f=$("$REAL" -t "$arch" | sed -n 1p); l=$("$REAL" -t "$arch" | sed -n '$p')
	[ "$f" != "$l" ] && "$REAL" -m "$arch" "$f" >/dev/null 2>&1 ;;
esac
exit $rc
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
