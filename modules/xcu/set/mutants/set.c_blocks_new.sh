# > fails for a file that does not exist yet under set -C
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^echo one >pc.nc1$/echo one >\/nonexistent\/pcm\/x/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
