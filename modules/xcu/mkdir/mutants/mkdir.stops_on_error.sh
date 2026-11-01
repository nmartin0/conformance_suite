# stops at the first operand that fails
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/mkdirref.sh"
mkdir() { pc_mkdirref stops_on_error "$@"; }
