# with -t a leading separator does not start an empty field
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref t_leading_dropped "$@"; }
