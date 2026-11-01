# ar -m moves only the first of several members
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -m)
	case $2 in -a|-b|-i) [ $# -gt 5 ] && set -- "$1" "$2" "$3" "$4" "$5" ;; *) [ $# -gt 3 ] && set -- "$1" "$2" "$3" ;; esac ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
