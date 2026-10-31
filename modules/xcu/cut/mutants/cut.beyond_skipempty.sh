# a line with nothing selected is not written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref beyond_skipempty any "$@"; }
