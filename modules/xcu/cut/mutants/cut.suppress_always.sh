# lines without the delimiter are suppressed even without -s
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref suppress_always f "$@"; }
