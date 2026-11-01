# after a failed source the next source is skipped
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; for l; do :; done; if [ $# -gt 2 ] && [ -d "$l" ]; then pc_mvmany skip_next "$@"; else command mv "$@"; fi; }
