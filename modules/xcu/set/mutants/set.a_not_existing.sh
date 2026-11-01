# a variable that already exists is not exported when assigned under set -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^PCX0=new$/set +a; PCX0=new; set -a/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
