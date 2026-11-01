# a missing option-argument is reported on standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PC_NLREF_DIR=${PC_MUTANT_FILE%/*}
. "$PC_NLREF_DIR/nlref.sh"
nl() { pc_nlref optarg_stdout "$@"; }
