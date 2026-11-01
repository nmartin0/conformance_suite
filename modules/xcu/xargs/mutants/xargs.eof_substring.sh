# xargs: an argument that merely starts with the end-of-file string ends the input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref eof_substring "$@"; }
