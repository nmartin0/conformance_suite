# xargs: xargs writes a message to standard output, which it does not use
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref noise_stdout "$@"; }
