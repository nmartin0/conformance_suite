# pr processing stops at the first file that cannot be opened
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
stop=0 bad= quiet=0 nf=0
n=$#
while [ $n -gt 0 ]
do
	a=$1; shift; n=$((n-1))
	case $a in
	-l|-h|-o|-w) set -- "$@" "$a" "$1"; shift; n=$((n-1)) ;;
	-r) quiet=1; set -- "$@" "$a" ;;
	-*) set -- "$@" "$a" ;;
	*) if [ $stop = 1 ]; then :
	   elif [ -r "$a" ]; then set -- "$@" "$a"; nf=$((nf+1))
	   else stop=1 bad=$a
	   fi ;;
	esac
done
if [ $stop = 0 ]; then exec "$REAL" "$@"; fi
[ $nf -gt 0 ] && "$REAL" "$@"
[ $quiet = 1 ] || echo "pr: cannot open $bad" >&2
exit 1
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
