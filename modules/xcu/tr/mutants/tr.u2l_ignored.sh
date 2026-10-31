# [:upper:] to [:lower:] converts nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref u2l_ignored "$@"; }
