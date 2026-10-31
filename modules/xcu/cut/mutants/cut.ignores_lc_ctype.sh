# cut ignores LC_CTYPE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cut() ( unset LC_CTYPE; command cut "$@" )
