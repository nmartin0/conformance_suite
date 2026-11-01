# test: -z of a string that looks like an operator is true
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant z_opstring_true ftb
