# ar -d deletes every member
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -d*)
	arch=$2
	"$REAL" "$@"; rc=$?
	for x in $("$REAL" -t "$arch"); do "$REAL" -d "$arch" "$x" >/dev/null 2>&1; done
	exit $rc ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
