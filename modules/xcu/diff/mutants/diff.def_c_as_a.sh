# default format: a change is introduced by the letter a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref def_c_as_a "$@"; }
