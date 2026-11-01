# a conforming nm: every symbol name carries a prefix (the text leaves the decoration of names open)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref prefixed "$@"; }
