# chmod planted fault: a_s_only_u (see chmodref.sh for what it changes)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/chmodref.sh"
chmod() { pc_chmodref a_s_only_u "$@"; }
