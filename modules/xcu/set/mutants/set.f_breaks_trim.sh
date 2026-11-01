# set -f also turns off pattern matching in prefix removal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/\${pcv#pc.f\*}/${pcv#"pc.f*"}/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
