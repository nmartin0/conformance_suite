# a space is written on each side of the equals sign
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_MODE=spaced
PCM_PRE=$(cat "${PC_MUTANT_FILE%/*/*}/mutants/_listfn.sh")
PCM_SED='s/^set >\(.*\)$/pcm_list >\1/'
export PCM_MODE
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
