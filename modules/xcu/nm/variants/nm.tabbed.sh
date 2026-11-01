# a conforming nm: fields separated by a tab (a tab is a blank, and the text lets any blanks separate the fields)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref tabbed "$@"; }
