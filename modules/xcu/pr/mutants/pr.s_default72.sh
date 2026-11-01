# pr with -s the default width stays 72
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
sep=" " has2=0 hasw=0 hass=0
for a; do case $a in -2) has2=1 ;; -w|-w?*) hasw=1 ;; -s) hass=1; sep=$(printf '\t') ;; -s?*) hass=1; sep=${a#-s} ;; esac; done
[ $has2 = 1 ] && [ $hasw = 0 ] && [ $hass = 1 ] || exec "$REAL" "$@"
file=
for a; do case $a in -*) ;; *) file=$a ;; esac; done
awk -v sep="$sep" '{ l[NR] = $0 } END { rows = int((NR + 1) / 2); for (r = 1; r <= rows; r++) { a = l[r]; b = l[r + rows]; a = substr(a, 1, 35); b = substr(b, 1, 35); print a sep b } }' "$file"
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
