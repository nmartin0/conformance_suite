# -u replaces only a file strictly newer than the member
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -ru|-rcu|-ruc)
	o=$1; arch=$2; shift 2
	o=$(printf '%s' "$o" | tr -d u)
	for x; do
		line=$(TZ=UTC "$REAL" -tv "$arch" "$x" 2>/dev/null | sed -n 1p)
		if [ -n "$line" ]; then
			set -- $line
			case $4 in Jan) mo=01;; Feb) mo=02;; Mar) mo=03;; Apr) mo=04;; May) mo=05;; Jun) mo=06;; Jul) mo=07;; Aug) mo=08;; Sep) mo=09;; Oct) mo=10;; Nov) mo=11;; Dec) mo=12;; esac
			d=$5; [ ${#d} -lt 2 ] && d=0$d
			hh=${6%:*}; mm=${6#*:}; [ ${#hh} -lt 2 ] && hh=0$hh; [ ${#mm} -lt 2 ] && mm=0$mm
			TZ=UTC touch -t $7$mo$d$hh$mm pc-mut.ref
			[ -n "$(find "$x" -newer pc-mut.ref -print)" ] || continue
		fi
		"$REAL" "$o" "$arch" "$x" || exit 1
	done
	exit 0 ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
