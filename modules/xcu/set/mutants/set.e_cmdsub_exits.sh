# a failing command substitution ends the shell under -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^echo \$(false; echo one) three$/pcm_v=$(false; echo one) || exit $?; echo $pcm_v three/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
