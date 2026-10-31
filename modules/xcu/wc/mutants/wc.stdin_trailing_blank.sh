# a blank follows the last count for standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref stdin_trailing_blank "$@"; }
