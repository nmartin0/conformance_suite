# a selected line together with an unreadable file exits 2, as for any error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming standalone grep, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
{ printf '#!/bin/sh\nREAL="%s"\nFAULT=error_status_wins\n' "$(command -v grep)"; cat "${PC_MUTANT_FILE%/*}/../mutants/grepref.sh"; } >pc-mutbin/grep
chmod +x pc-mutbin/grep
PATH=$PWD/pc-mutbin:$PATH
