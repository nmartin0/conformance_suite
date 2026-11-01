# m4: gives $# as 1 when the macro has no parenthesised argument list
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F hash_noparen_1 "$@"; }
