# ar -d reports the deletion but leaves the archive alone
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -d*)
	cp "$2" pc-mut.arc.$$ || exit 1
	opt=$1; shift 2
	"$REAL" "$opt" pc-mut.arc.$$ "$@"; rc=$?
	rm -f pc-mut.arc.$$; exit $rc ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
