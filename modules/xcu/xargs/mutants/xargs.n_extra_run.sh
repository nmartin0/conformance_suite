# xargs: with -n an extra run with no arguments follows when the input divides evenly
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref n_extra_run "$@"; }
