# a source directory without write permission is copied empty
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cp)" >pc-mutbin/cp
cat >>pc-mutbin/cp <<'PC_EOF'
for last; do :; done
for src; do [ "$src" = "$last" ] && break; prev=$src; done
m=$(ls -ld "$prev")
case $m in d?-*) mkdir "$last" && chmod 500 "$last"; exit 0 ;; esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/cp
PATH=$PWD/pc-mutbin:$PATH
