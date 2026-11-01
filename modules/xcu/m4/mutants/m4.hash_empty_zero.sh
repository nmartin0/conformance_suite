# m4: counts an empty argument list as no arguments in $#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F hash_empty_zero "$@"; }
