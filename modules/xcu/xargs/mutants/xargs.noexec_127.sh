# xargs: a utility that cannot be invoked gives status 127
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref noexec_127 "$@"; }
