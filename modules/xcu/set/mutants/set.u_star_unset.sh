# under -u, $* with no parameters is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/\$\*/${pcm_unset_x}/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
