# m4: still treats the number sign as a comment start after changecom
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F changecom_keeps_hash "$@"; }
