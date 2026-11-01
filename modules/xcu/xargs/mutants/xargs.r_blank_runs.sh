# xargs: -r only suppresses the run for an empty input, not for blank lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref r_blank_runs "$@"; }
