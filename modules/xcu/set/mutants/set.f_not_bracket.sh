# set -f leaves bracket expressions active
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^echo pc.\[f\]1$/set +f; echo pc.[f]1; set -f/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
