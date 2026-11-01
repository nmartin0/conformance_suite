# m4: undivert leaves the buffer content in place
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F undivert_keeps "$@"; }
