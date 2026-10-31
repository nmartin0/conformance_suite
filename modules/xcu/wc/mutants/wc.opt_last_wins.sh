# only the last option is honoured
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref opt_last_wins "$@"; }
