# a failure inside a case branch does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/x) false;/x) set +e; false;/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
