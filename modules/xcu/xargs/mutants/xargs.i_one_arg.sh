# xargs: -I replaces the string in only one argument
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref i_one_arg "$@"; }
