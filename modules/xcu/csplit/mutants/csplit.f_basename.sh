# csplit: the directory part of the -f prefix is dropped
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref f_basename "$@"; }
