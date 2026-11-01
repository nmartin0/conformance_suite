# xargs: -s is cut down to 4096, the largest size this xargs supports (allowed: it uses the largest it supports)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref s_cap_small "$@"; }
