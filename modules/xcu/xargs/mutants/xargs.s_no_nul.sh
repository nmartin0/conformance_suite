# xargs: -s counts the arguments without their terminating null bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref s_no_nul "$@"; }
