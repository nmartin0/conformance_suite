# tail -c number is ignored (the default ten lines)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone tail, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v tail)" >pc-mutbin/tail
cat >>pc-mutbin/tail <<'PC_EOF'
mode= num= rev= fol= file= attached=
while [ $# -gt 0 ]; do
	case $1 in
	--) shift; [ $# -gt 0 ] && file=$1; break ;;
	-n|-c) mode=${1#-}; num=${2:-}; shift 2 ;;
	-n?*|-c?*) mode=${1#-}; mode=${mode%${mode#?}}; num=${1#-?}; attached=1; shift ;;
	-r) rev=1; shift ;;
	-f) fol=1; shift ;;
	*) file=$1; shift ;;
	esac
done
sign=
case $num in [+-]*) sign=${num%${num#?}} ;; esac
digits=${num#[+-]}
real() {
	set --
	[ -n "$fol" ] && set -- "$@" -f
	[ -n "$rev" ] && set -- "$@" -r
	[ -n "$mode" ] && set -- "$@" "-$mode" "$num"
	[ -n "$file" ] && set -- "$@" "$file"
	"$REAL" "$@"
}
go() {
	set --
	[ -n "$fol" ] && set -- "$@" -f
	[ -n "$rev" ] && set -- "$@" -r
	[ -n "$mode" ] && set -- "$@" "-$mode" "$num"
	[ -n "$file" ] && set -- "$@" "$file"
	exec "$REAL" "$@"
}
if [ "$mode" = c ]; then mode=n; num=10; fi
go
PC_EOF
chmod +x pc-mutbin/tail
PATH=$PWD/pc-mutbin:$PATH
