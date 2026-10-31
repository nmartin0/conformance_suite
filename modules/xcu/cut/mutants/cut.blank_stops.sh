# a blank in the list ends it: only the part before the first blank is used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref blank_stops any "$@"; }
