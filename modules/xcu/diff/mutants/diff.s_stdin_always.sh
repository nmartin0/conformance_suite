# standard input replaces the first operand even when no operand is -
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref s_stdin_always "$@"; }
