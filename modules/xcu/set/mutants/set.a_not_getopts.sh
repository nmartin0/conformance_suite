# the variable getopts assigns is not exported under set -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^getopts a PCG "\$@"$/set +a; getopts a PCG "$@"; set -a/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
