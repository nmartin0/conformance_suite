# several selection options select only the processes that match every one
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
case " $* " in *" -p "*) ;; *) exec "$REAL" "$@" ;; esac
pv=
rest=
c=$#
while [ "$c" -gt 0 ]; do
	a=$1; shift; c=$((c - 1))
	case $a in
	-p) pv="$pv $1"; shift; c=$((c - 1)) ;;
	-o) shift; c=$((c - 1)) ;;
	*) set -- "$@" "$a" ;;
	esac
done
[ "$#" -gt 0 ] || exec "$REAL" -o pid= -p "$pv"
a=$("$REAL" -o pid= -p "$(echo $pv | tr " " ",")")
b=$("$REAL" -o pid= "$@")
for x in $a; do
	for y in $b; do [ "$x" = "$y" ] && echo "$x"; done
done
exit 0
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
