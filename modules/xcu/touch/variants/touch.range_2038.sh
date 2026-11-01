# touch (conforming variant) times after the signed 32-bit limit (mid January 2038) are refused
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed by the text: the range past the Epoch is implementation-defined, it must only reach the start of 2038 (OPTIONS, -t).
# A conforming standalone touch, first on PATH; it runs the real one and alters one thing.
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
b=${T%%.*}; if [ "${#b}" -eq 10 ]; then if [ "${b%????????}" -lt 69 ]; then b=20$b; else b=19$b; fi; fi; if [ "${#b}" -ge 12 ] && [ "$b" -gt 203801190314 ]; then echo "touch: time out of range" >&2; exit 1; fi; case ${D%%-*} in [0-9][0-9][0-9][0-9]*) [ "${D%%-*}" -gt 2038 ] && { echo "touch: time out of range" >&2; exit 1; } ;; esac
run "$@"
rc=$?
exit $rc
PC_EOF
chmod +x pc-mutbin/touch
PATH=$PWD/pc-mutbin:$PATH
