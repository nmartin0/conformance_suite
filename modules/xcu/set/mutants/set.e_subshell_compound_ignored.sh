# a subshell command that fails after an ignored failure does not end the shell under -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^( false && true )$/( false \&\& true ) || :/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
