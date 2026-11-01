# xargs: xargs does not search PATH for a utility name without a slash
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref nopath_search "$@"; }
