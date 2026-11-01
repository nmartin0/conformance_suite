# without -C an existing file is never replaced
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -x*)
	case $1 in *C*) exec "$REAL" "$@" ;; esac
	[ "${2:-}" = -C ] && exec "$REAL" "$@"
	opt=$1; arch=$2; shift 2
	[ $# -gt 0 ] || set -- $("$REAL" -t "$arch")
	for x in "$@"; do
		[ -e "${x##*/}" ] && continue
		"$REAL" "$opt" "$arch" "$x" || exit 1
	done
	exit 0 ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
