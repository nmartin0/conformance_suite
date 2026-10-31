# tr -ds squeezes runs as they stand before the deletion
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref ds_sq_before_delete "$@"; }
