# xargs: -n NUMBER allows one argument too few per run
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref n_minus1 "$@"; }
