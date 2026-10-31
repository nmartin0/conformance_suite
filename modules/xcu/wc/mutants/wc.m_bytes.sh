# -m always counts bytes, whatever the locale
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref m_bytes "$@"; }
