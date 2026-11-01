# -e: the text lines carry a > prefix
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref e_text_prefixed "$@"; }
