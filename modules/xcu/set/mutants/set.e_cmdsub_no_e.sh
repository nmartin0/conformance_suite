# the subshell of a command substitution does not inherit -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^echo \$(false; echo one) three$/echo $(set +e; false; echo one) three/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
