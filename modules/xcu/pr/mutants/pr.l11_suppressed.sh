# pr a page length of 11 suppresses header and trailer
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
n=$#
while [ $n -gt 0 ]
do
	a=$1; shift; n=$((n-1))
	case $a in
	-l) v=$1; shift; n=$((n-1)); [ "$v" = 11 ] && v=10; set -- "$@" -l "$v" ;;
	*) set -- "$@" "$a" ;;
	esac
done
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
