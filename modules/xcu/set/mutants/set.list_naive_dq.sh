# values are double-quoted without escaping the characters that stay special
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_MODE=naive_dq
PCM_PRE=$(cat "${PC_MUTANT_FILE%/*/*}/mutants/_listfn.sh")
PCM_SED='s/^set >\(.*\)$/pcm_list >\1/'
export PCM_MODE
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
