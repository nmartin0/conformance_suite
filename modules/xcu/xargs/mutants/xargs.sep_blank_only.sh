# xargs: newlines do not separate arguments, only blanks do
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref sep_blank_only "$@"; }
