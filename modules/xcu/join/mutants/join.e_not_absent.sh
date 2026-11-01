# -e replaces only empty fields, not absent ones
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref e_not_absent "$@"; }
