# -a also lists session leaders that have a terminal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming standalone ps, first on PATH; it runs the real one and changes one allowed thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
case " $* " in *" -a "*) ;; *) exec "$REAL" "$@" ;; esac
"$REAL" -A -o pid= -o tty= | awk '$2 != "?" && $2 != "-" { print $1 }'
exit 0
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
