# a failure in the body of a while does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/pcn=1; false;/pcn=1; set +e; false;/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
