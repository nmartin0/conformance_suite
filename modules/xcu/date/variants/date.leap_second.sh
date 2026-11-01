# every time shown falls in a leap second: %S is 60 (and so are %T, %X, %r and %c)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# %S may run from 0 to 60 to allow for a leap second (XCU date APPLICATION USAGE)
# A conforming standalone date, first on PATH; it runs the real one and alters one thing.
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
expand() {
	in=$1 out=
	while [ -n "$in" ]
	do
		c=${in%"${in#?}"}
		case $c in
		%)
			in=${in#?}
			c=${in%"${in#?}"}
			in=${in#?}
			case $c in
			D) out=$out%m/%d/%y ;;
			T|X) out=$out%H:%M:%S ;;
			r) out=$out"%I:%M:%S %p" ;;
			x) out=$out%m/%d/%y ;;
			c) out=$out"%a %b %e %H:%M:%S %Y" ;;
			*) out=$out%$c ;;
			esac
			;;
		*) in=${in#?}; out=$out$c ;;
		esac
	done
}
if [ -z "$haf" ]; then f="+%a %b %e %H:%M:%S %Z %Y"; fi
expand "${f#+}"; f=+$out
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
				S) r='60' ;;
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
