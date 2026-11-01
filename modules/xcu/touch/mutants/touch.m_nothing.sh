# touch -m does nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone touch, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v touch)" >pc-mutbin/touch
cat >>pc-mutbin/touch <<'PC_EOF'
A= M= C= R= T= D=
while getopts acmr:t:d: o
do
	case $o in
	a) A=-a ;; m) M=-m ;; c) C=-c ;; r) R=$OPTARG ;; t) T=$OPTARG ;; d) D=$OPTARG ;;
	*) echo "touch: bad option" >&2; exit 2 ;;
	esac
done
shift $((OPTIND - 1))
new=
for f
do
	[ -e "$f" ] || [ -h "$f" ] || new="$new $f"
done
run() {
	"$REAL" $A $M $C ${R:+-r} ${R:+"$R"} ${T:+-t} ${T:+"$T"} ${D:+-d} ${D:+"$D"} -- "$@"
}
[ -n "$M" ] && [ -z "$A" ] && exit 0
run "$@"
rc=$?
exit $rc
PC_EOF
chmod +x pc-mutbin/touch
PATH=$PWD/pc-mutbin:$PATH
