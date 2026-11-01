# pr the sign of the offset in TZ is reversed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
case ${TZ-} in
[A-Za-z]*)
	nm=${TZ%%[-+0-9]*}
	off=${TZ#"$nm"}
	case $off in -*) off=${off#-} ;; +*) off=-${off#+} ;; *) off=-$off ;; esac
	TZ=$nm$off
	export TZ ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
