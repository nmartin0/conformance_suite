# a conforming nm: local symbols carry an implementation-defined type letter (any single character is allowed)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref impl_local_types "$@"; }
