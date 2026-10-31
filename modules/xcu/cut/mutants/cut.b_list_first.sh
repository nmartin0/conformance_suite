# cut -b uses only the first element of a comma list
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref list_first b "$@"; }
