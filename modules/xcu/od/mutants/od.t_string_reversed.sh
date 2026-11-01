# the types of one -t string are taken in reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref t_string_reversed "$@"; }
