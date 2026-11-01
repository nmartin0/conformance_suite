# a conforming nm: an object without symbols is reported by a message on standard error, exit status 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref nosym_stderr "$@"; }
