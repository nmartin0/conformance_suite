# a failure inside a loop body does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/do false;/do set +e; false;/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
