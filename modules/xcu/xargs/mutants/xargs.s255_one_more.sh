# xargs: xargs makes one more invocation after status 255
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref s255_one_more "$@"; }
