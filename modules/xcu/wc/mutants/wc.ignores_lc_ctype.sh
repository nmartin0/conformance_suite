# wc ignores LC_CTYPE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
wc() ( unset LC_CTYPE; command wc "$@" )
