# m4: index looks for the first character of the second argument only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F index_firstchar "$@"; }
