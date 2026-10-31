# uniq compares only the first 1024 bytes of each line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref trunc1024 "$@"; }
