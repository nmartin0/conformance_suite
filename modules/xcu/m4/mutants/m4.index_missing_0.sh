# m4: index gives 0 when the string is absent
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F index_missing_0 "$@"; }
