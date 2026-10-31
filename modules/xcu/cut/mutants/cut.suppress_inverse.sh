# -s suppresses the lines that have the delimiter
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref suppress_inverse f "$@"; }
