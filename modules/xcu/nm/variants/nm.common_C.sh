# a conforming nm: an uninitialised global is typed C (an implementation-defined single-character type is allowed)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref common_C "$@"; }
