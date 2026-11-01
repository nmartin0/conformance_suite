# chmod planted fault: nowho_as_user (see chmodref.sh for what it changes)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/chmodref.sh"
chmod() { pc_chmodref nowho_as_user "$@"; }
