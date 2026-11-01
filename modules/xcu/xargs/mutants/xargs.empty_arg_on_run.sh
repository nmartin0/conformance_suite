# xargs: a run of blanks yields an empty argument between them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref empty_arg_on_run "$@"; }
