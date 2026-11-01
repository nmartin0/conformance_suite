# a failing subshell command does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^(exit 7)$/(exit 7) || :/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
