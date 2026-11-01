# offsets are written twelve digits wide
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed by the page: see the note in the test that declares this variant.
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref off_wide "$@"; }
