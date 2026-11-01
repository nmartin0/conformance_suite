# m4: -U does not undefine built-in macros
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F U_not_builtin "$@"; }
