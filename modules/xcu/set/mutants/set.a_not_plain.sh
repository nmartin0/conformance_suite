# plain assignments are not exported under set -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^PCA=1$/set +a; PCA=1; set -a/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
