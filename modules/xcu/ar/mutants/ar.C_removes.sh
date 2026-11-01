# -C removes the existing file instead of keeping it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -x*)
	c=
	case $1 in *C*) c=1 ;; esac
	[ "${2:-}" = -C ] && c=1
	if [ -n "$c" ]; then
		shift; [ "$1" = -C ] && shift
		arch=$1; shift
		[ $# -gt 0 ] || set -- $("$REAL" -t "$arch")
		for x; do rm -f "${x##*/}"; done
		exit 0
	fi ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
