# uniq -d writes the unrepeated lines instead
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref d_inverted "$@"; }
