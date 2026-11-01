# the output of compressing gets the suffix .lz instead of .Z
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm suffix_lz "$@"; }
