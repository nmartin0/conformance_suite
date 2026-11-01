# $# is one more than the number of parameters
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/\$#/$(($#+1))/g'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
