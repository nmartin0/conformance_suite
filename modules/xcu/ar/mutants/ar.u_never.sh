# -u never replaces a member
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -ru|-rcu|-ruc)
	o=$1; arch=$2; shift 2; n=$#
	i=0; while [ $i -lt $n ]; do
		x=$1; shift
		"$REAL" -t "$arch" "$x" 2>/dev/null | grep -q . || set -- "$@" "$x"
		i=$((i+1))
	done
	[ $# -gt 0 ] || exit 0
	exec "$REAL" "$o" "$arch" "$@" ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
