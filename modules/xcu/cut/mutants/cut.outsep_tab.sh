# selected fields are joined by a tab whatever -d says
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref outsep_tab f "$@"; }
