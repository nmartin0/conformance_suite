# the shell finishes the brace group before it exits for a failure
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^{ false; echo after; }$/{ pcm_s=0; false || pcm_s=$?; echo after; [ "$pcm_s" -eq 0 ] || exit "$pcm_s"; }/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
