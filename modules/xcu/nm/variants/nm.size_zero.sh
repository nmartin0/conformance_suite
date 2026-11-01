# a conforming nm: the size of every symbol is 0 (the size is given only where it applies)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref size_zero "$@"; }
