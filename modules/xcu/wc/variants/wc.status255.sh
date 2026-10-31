# a conforming wc: the error status is 255
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/wcref.sh"
wc() { pc_wcref status255 "$@"; }
