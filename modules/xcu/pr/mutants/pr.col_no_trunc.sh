# pr long lines are not truncated in columns
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
has2=0 hass=0 hast=0
for a; do case $a in -2) has2=1 ;; -s*) hass=1 ;; -t) hast=1 ;; esac; done
[ $has2 = 1 ] && [ $hass = 0 ] && [ $hast = 1 ] || exec "$REAL" "$@"
file=
for a; do case $a in -*) ;; *) file=$a ;; esac; done
awk '{ l[NR] = $0 } END { rows = int((NR + 1) / 2); for (r = 1; r <= rows; r++) { a = l[r]; b = l[r + rows]; sep = " "; print a sep b } }' "$file"
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
