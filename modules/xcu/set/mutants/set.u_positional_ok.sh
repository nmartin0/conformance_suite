# an unset positional parameter is not an error under -u
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/\$1\]/${1-}]/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
