# sort -u drops every line that has an equal key, keeping none of them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref u_drop_all "$@"; }
