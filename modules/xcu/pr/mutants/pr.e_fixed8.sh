# pr -e expands every tab to a full gap of spaces
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
c=$(printf '\t') g=8 file= has=0
for a
do
	case $a in
	-e*) has=1; v=${a#-e}
		case $v in [!0-9]*) c=${v%"${v#?}"}; v=${v#?} ;; esac
		[ -n "$v" ] && [ "$v" != 0 ] && g=$v ;;
	-*) ;;
	*) file=$a ;;
	esac
done
[ $has = 1 ] || exec "$REAL" "$@"
awk -v c="$c" -v g=$g '{ out = ""; col = 1; n = length($0)
	for (i = 1; i <= n; i++) { ch = substr($0, i, 1)
		if (ch == c) { k = g - ((col - 1) % g); k = g; for (j = 0; j < k; j++) out = out " "; col += k }
		else { out = out ch; col++ } }
	print out }' "$file"
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
