# a command that is not found does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^pcm_nosuch_cmd_x$/pcm_nosuch_cmd_x || :/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
