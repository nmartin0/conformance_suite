# [:lower:] to [:upper:] converts nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref l2u_ignored "$@"; }
