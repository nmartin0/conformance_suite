# test: -f is false for a regular file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant f_reg_false ftb
