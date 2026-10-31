# each space separates, so runs of blanks make empty words
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref w_empty_fields "$@"; }
