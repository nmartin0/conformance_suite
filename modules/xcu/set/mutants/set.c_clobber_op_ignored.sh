# >| is refused like > under set -C
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^echo three >|pc.nc1$/echo three >pc.nc1/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
