# each character of string1 is replaced by the character after its counterpart in string2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref map_off "$@"; }
