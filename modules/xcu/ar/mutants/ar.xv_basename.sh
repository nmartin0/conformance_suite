# ar -xv names the last component instead of the operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -xv)
	tmp=pc-mut.$$; "$REAL" "$@" >$tmp; rc=$?
	for a in "$@"; do
		case $a in */*) b=${a##*/}
			sed -e "s|^<$a>\$|<$b>|" -e "s|^$a\$|$b|" -e "s| $a\$| $b|" $tmp >$tmp.2; mv $tmp.2 $tmp ;;
		esac
	done
	cat $tmp; rm -f $tmp; exit $rc ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
