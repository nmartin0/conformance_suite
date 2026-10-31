# the digits of an octal sequence are read as decimal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref oct_decimal "$@"; }
