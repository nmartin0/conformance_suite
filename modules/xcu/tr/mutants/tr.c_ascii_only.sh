# the complement of string1 covers only the values below 128
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref c_ascii_only "$@"; }
