# the names noglob and noclobber are swapped
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/noglob/pcm_x/g
s/noclobber/noglob/g
s/pcm_x/noclobber/g'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
