# tr -ds deletes the characters of string2 and squeezes those of string1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref ds_swap "$@"; }
