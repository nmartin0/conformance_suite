# kill fault lop_all
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut lop_all "${PC_MUTANT_FILE%/*}"
