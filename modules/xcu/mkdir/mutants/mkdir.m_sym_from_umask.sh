# a symbolic -m mode starts from the masked default, not rwxrwxrwx
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/mkdirref.sh"
mkdir() { pc_mkdirref m_sym_from_umask "$@"; }
