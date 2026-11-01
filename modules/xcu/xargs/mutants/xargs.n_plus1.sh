# xargs: -n NUMBER allows one argument too many per run
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref n_plus1 "$@"; }
