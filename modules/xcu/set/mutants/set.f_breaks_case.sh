# set -f also turns off pattern matching in case
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^case pc.f1 in pc.f\*)/case pc.f1 in "pc.f*")/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
