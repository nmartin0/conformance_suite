# only the first 512 bytes of standard input are counted
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref stdin_head "$@"; }
