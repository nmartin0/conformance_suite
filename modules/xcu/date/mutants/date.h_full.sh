# %h gives the full month name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone date, first on PATH; it runs the real one and alters one thing.
# It never passes a time-setting operand on: the system clock is not touched.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v date)" >pc-mutbin/date
cat >>pc-mutbin/date <<'PC_EOF'
u= f= haf=
for a
do
	case $a in
	-u) u=-u ;;
	+*) f=$a haf=1 ;;
	*) exit 1 ;;
	esac
done
nl='
'
nth() { n=$1; shift; eval "r=\${$n}"; }

xf() {
	in=$1 out=
	while [ -n "$in" ]
	do
		c=${in%"${in#?}"}
		case $c in
		%)
			in=${in#?}
			c=${in%"${in#?}"}
			in=${in#?}
			r=%$c
			case $c in
				h) r='%B' ;;
				*) ;;
			esac
			out=$out$r
			;;
		*)
			in=${in#?}
			r=$c
			
			out=$out$r
			;;
		esac
	done
}
if [ -z "$haf" ]; then f='+%a %b %e %H:%M:%S %Z %Y'; fi
xf "${f#+}"
fmt=$out
o=$("$REAL" $u "+$fmt"; echo "x$?")
rc=${o##*x}
o=${o%x*}

printf '%s' "$o"
exit "$rc"
PC_EOF
chmod +x pc-mutbin/date
PATH=$PWD/pc-mutbin:$PATH
