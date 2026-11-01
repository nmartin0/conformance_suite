# decompressing the operand - gives one byte too many
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm dash_d_extra "$@"; }
