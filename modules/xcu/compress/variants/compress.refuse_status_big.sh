# Any status greater than zero is allowed when an existing output file
# is not overwritten.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/cmut.sh"
compress() { pc_cm refuse_status_big "$@"; }
