# default format: ranges written with a hyphen instead of a comma
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref def_dash_range "$@"; }
