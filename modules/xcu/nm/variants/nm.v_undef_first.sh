# a conforming nm: undefined symbols come first with -v (only the defined symbols are ordered by value)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref v_undef_first "$@"; }
