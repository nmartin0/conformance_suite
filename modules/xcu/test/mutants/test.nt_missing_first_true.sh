# test: -nt is true when the first file is missing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant nt_missing_first_true ftb
