# xargs: status 255 stops xargs with exit status 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref s255_status0 "$@"; }
