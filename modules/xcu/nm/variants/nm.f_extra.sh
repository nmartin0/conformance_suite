# a conforming nm: -f also lists the redundant section symbols (.text, .data, .bss)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref f_extra "$@"; }
