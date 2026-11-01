# m4: substr takes the third argument as an end position
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F substr_len_is_end "$@"; }
