# wc -l is one too many
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref l_off "$@"; }
