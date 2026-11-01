# a conforming nm: undefined symbols are written with value 0 and size 0 (the format has the fields, the values are the implementation's)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref undef_zero "$@"; }
