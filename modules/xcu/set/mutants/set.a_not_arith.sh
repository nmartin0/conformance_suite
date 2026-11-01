# variables assigned in an arithmetic expansion are not exported under set -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^: \$((PCEI = 5))$/set +a; : $((PCEI = 5)); set -a/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
