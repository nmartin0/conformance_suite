# Any status greater than two is allowed for an unsupported algorithm.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/cmut.sh"
compress() { pc_cm unsupp_status_big "$@"; }
