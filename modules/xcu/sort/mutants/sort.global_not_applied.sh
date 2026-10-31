# sort -k keys without type letters ignore the global ordering options
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref global_not_applied "$@"; }
