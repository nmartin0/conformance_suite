# uudecode keeps the padding as NUL bytes in Base64 output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref b64_pad_kept "$@"; }
pc_uu_install b64_pad_kept
