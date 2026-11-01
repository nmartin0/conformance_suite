# a conforming nm: a tab, not a blank, between the -A prefix and the name (blanks may be of any kind)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref A_tab "$@"; }
