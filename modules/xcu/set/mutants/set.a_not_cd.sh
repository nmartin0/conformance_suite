# the variables cd assigns are not exported under set -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^cd \/$/set +a; cd \/; pcm_o=$OLDPWD; unset OLDPWD; OLDPWD=$pcm_o; set -a/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
