# only nine positional parameters are kept
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_MODE=max9
PCM_PRE=$(cat "${PC_MUTANT_FILE%/*/*}/mutants/_setfn.sh")
PCM_SED='s/^set -- \(..*\)$/eval "$(pcm_set $# "$@" :: \1)"/
s/^set \([^-+ ].*\)$/eval "$(pcm_set $# "$@" :: \1)"/'
export PCM_MODE
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
