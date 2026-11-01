# the length of an unset variable is not an error under -u
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/\${#PCU1}/0/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
