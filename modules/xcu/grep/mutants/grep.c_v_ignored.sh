# -c counts the matching lines even with -v
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone grep, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
{ printf '#!/bin/sh\nREAL="%s"\nFAULT=c_v_ignored\n' "$(command -v grep)"; cat "${PC_MUTANT_FILE%/*}/grepref.sh"; } >pc-mutbin/grep
chmod +x pc-mutbin/grep
PATH=$PWD/pc-mutbin:$PATH
