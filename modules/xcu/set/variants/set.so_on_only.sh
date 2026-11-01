# conforming: set -o lists only the options that are on
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_MODE=so_on_only
PCM_PRE=$(cat "${PC_MUTANT_FILE%/*/*}/mutants/_optfn.sh")
PCM_SED='s/^set -o \(>.*\)$/pcm_so \1/
s/set +o /pcm_po /'
export PCM_MODE
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
