# the refused > empties the file before it fails
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^if echo two >pc.nc1; then/: >|pc.nc1; if echo two >\/nonexistent\/pcm\/x; then/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
