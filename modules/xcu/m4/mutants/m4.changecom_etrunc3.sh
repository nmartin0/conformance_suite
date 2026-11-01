# m4: changecom uses three characters of the end-comment
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F changecom_etrunc3 "$@"; }
