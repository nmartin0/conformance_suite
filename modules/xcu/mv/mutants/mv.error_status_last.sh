# the status is that of the last source only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; for l; do :; done; if [ $# -gt 2 ] && [ -d "$l" ]; then pc_mvmany status_last "$@"; else command mv "$@"; fi; }
