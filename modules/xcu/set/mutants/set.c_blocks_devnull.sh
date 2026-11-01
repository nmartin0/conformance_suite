# > fails for /dev/null under set -C
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^echo four >\/dev\/null$/echo four >\/nonexistent\/pcm\/x/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
