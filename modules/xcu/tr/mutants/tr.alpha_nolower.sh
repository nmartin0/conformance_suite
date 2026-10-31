# [:alpha:] holds no lowercase letters
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref alpha_nolower "$@"; }
