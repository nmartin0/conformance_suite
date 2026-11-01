# a conforming nm: -A writes the full pathname of the object (the description asks for the full pathname)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref A_absolute "$@"; }
