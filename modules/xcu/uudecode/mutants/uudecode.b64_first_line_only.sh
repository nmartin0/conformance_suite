# uudecode decodes only the first Base64 line (57 bytes)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref b64_first_line_only "$@"; }
pc_uu_install b64_first_line_only
