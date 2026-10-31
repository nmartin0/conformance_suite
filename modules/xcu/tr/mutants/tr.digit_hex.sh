# [:digit:] holds the hexadecimal letters too
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref digit_hex "$@"; }
