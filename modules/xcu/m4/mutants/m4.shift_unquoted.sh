# m4: shift does not quote the arguments it gives
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F shift_unquoted "$@"; }
