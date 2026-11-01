# no blank line is written before a directory header
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ls)" >pc-mutbin/ls
cat >>pc-mutbin/ls <<'PC_EOF'
has() { l=$1; shift; for a; do case $a in --) return 1;; -?*) case $a in *"$l"*) return 0;; esac;; esac; done; return 1; }
pos() { l=$1; shift; i=0; for a; do i=$((i+1)); case $a in -?*) case $a in *"$l"*) echo $i; return;; esac;; esac; done; echo 0; }
files= dirs=
for a; do
	if [ -d "$a" ] && [ ! -L "$a" ]; then dirs="$dirs $a"; else files="$files $a"; fi
done
fl=$(printf '%s\n' $files | LC_ALL=C sort)
dl=$(printf '%s\n' $dirs | LC_ALL=C sort)
sep=0
files_out() { [ -n "$files" ] && { printf '%s\n' $fl; sep=1; }; }
dirs_out() { for d in $dl; do [ $sep = 1 ] && :; echo "$d:"; "$REAL" "$d"; sep=1; done; }
files_out; dirs_out
PC_EOF
chmod +x pc-mutbin/ls
PATH=$PWD/pc-mutbin:$PATH
