# uniq exits 1 although everything succeeded
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref status1 "$@"; }
