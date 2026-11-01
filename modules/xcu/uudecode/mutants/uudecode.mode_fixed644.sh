# uudecode always gives the file mode 644
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref mode_fixed644 "$@"; }
pc_uu_install mode_fixed644
