# ar -p writes a newline after a member of odd length
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -p|-pv)
	opt=$1; arch=$2; shift 2
	[ $# -gt 0 ] || set -- $("$REAL" -t "$arch") || exit 1
	for x; do
		tmp=pc-mut.$$; "$REAL" -p "$arch" "$x" >$tmp || exit 1
		[ $opt = -pv ] && printf '\n<%s>\n\n' "$x"
		cat $tmp
		if [ $(( $(wc -c <$tmp) % 2 )) = 1 ]; then echo; fi
		rm -f $tmp
	done
	exit 0 ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
