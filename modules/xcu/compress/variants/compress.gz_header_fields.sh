# The GZIP format lets the modification time, extra flags and operating
# system fields of the header hold any value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/cmut.sh"
compress() { pc_cm gz_header_fields "$@"; }
