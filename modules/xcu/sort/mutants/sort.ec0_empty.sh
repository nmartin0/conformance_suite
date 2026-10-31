# sort -k takes a last character of 0 as an empty key end
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref ec0_empty "$@"; }
