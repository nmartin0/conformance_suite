# pr -F makes the pages shorter
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
	-F) set -- "$@" -F -l 60 ;;
	*) set -- "$@" "$a" ;;
	esac
done
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
