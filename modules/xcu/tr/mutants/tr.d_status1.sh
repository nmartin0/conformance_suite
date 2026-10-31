# tr -d exits 1 although everything succeeded
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref d_status1 "$@"; }
