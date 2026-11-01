# pr -i uses a tab at the first tab column of a line only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
c=$(printf '\t') g=8 file= has=0
for a
do
	case $a in
	-i*) has=1; v=${a#-i}
		case $v in [!0-9]*) c=${v%"${v#?}"}; v=${v#?} ;; esac
		[ -n "$v" ] && [ "$v" != 0 ] && g=$v ;;
	-*) ;;
	*) file=$a ;;
	esac
done
[ $has = 1 ] || exec "$REAL" "$@"
awk -v c="$c" -v g=$g -v mode=once '{ out = ""; col = 1; pend = 0; used = 0; n = length($0)
	if (mode == "all") { gsub(/ /, c); print; next }
	for (i = 1; i <= n; i++) { ch = substr($0, i, 1)
		if (ch == " ") { pend++; col++
			if ((col - 1) % g == 0) {
				if ((mode != "min2" || pend >= 2) && !(mode == "once" && used)) { out = out c; used = 1 }
				else for (j = 0; j < pend; j++) out = out " "
				pend = 0 } }
		else { for (j = 0; j < pend; j++) out = out " "; pend = 0; out = out ch; col++ } }
	for (j = 0; j < pend; j++) out = out " "
	print out }' "$file"
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
