# a conforming nm: the error status is 255 (any status greater than zero is allowed)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref status255 "$@"; }
