# a leading zero in [x*n] does not make n octal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref rep_octal_dec "$@"; }
