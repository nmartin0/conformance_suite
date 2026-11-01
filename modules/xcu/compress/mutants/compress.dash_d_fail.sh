# decompressing the operand - exits with status 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm dash_d_fail "$@"; }
