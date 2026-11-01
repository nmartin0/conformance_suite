# conforming: set +o writes its commands on one line separated by semicolons
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_MODE=po_oneline
PCM_PRE=$(cat "${PC_MUTANT_FILE%/*/*}/mutants/_optfn.sh")
PCM_SED='s/^set -o \(>.*\)$/pcm_so \1/
s/set +o /pcm_po /'
export PCM_MODE
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
