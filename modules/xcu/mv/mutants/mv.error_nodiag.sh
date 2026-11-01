# a failed source gives no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; for l; do :; done; if [ $# -gt 2 ] && [ -d "$l" ]; then pc_mvmany nodiag "$@"; else command mv "$@"; fi; }
