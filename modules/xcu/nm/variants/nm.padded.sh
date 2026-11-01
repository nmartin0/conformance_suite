# a conforming nm: fields separated by several blanks, as alignment may need (the text lets any number of blanks separate the fields)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref padded "$@"; }
