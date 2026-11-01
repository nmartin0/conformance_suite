# xargs: xargs exit status reflects only the last invocation
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref fail_last_only "$@"; }
