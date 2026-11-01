# the subshell of a pipeline does not inherit -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^(false; echo one) | cat$/(set +e; false; echo one) | cat/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
