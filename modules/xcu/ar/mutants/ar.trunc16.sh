# member names are cut to 16 characters
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
		b=${f##*/}
		t=$(printf '%s' "$b" | cut -c1-16)
		cp "$f" "pc-mut.d/$t" 2>/dev/null || { echo "ar: cannot read $f" >&2; exit 1; }
		set -- "$@" "pc-mut.d/$t"
		i=$((i+1))
	done
	exec "$REAL" "$opt" "$arch" "$@" ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
