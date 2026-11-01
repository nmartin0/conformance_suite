# xargs: without -x the argument that does not fit is dropped
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref nox_drops "$@"; }
