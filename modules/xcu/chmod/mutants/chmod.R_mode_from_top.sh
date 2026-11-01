# chmod planted fault: R_mode_from_top (see chmodref.sh for what it changes)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/chmodref.sh"
chmod() { pc_chmodref R_mode_from_top "$@"; }
