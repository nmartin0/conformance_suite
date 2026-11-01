# pr the header has 4 lines (a leading blank line is missing)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
L=66 prev=
for a
do
	[ "$prev" = -l ] && L=$a
	case $a in -l?*) L=${a#-l} ;; esac
	prev=$a
done
case " $* " in *" -t "*) exec "$REAL" "$@" ;; esac
[ "$L" -le 10 ] && exec "$REAL" "$@"
"$REAL" "$@" | awk -v L=$L 'NR%L != 1'
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
