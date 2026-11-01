# a failure in the body of an if does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/then false;/then set +e; false;/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
