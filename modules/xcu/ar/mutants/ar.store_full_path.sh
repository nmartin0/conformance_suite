# a file added as sub/f3 is stored under the whole pathname
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -r*|-q*)
	opt=$1; arch=$2; shift 2; n=$#
	mkdir -p pc-mut.d
	i=0; while [ $i -lt $n ]; do
		f=$1; shift
		case $f in
		*/*) nm=$(printf '%s' "$f" | tr / %); cp "$f" "pc-mut.d/$nm"; set -- "$@" "pc-mut.d/$nm" ;;
		*) set -- "$@" "$f" ;;
		esac
		i=$((i+1))
	done
	exec "$REAL" "$opt" "$arch" "$@" ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
