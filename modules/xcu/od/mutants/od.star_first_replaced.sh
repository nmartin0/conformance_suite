# the first of the identical lines is replaced too
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref star_first_replaced "$@"; }
