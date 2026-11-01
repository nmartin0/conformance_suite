# xargs: the default command line length is 2048 (the least the text requires)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref default_size_2048 "$@"; }
