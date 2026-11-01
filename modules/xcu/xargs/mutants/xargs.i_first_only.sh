# xargs: -I replaces only the first occurrence in each argument
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref i_first_only "$@"; }
