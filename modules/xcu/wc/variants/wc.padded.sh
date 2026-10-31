# a conforming wc: every count is padded with blanks to a width of seven (the tests accept blanks around the counts)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/wcref.sh"
wc() { pc_wcref padded "$@"; }
