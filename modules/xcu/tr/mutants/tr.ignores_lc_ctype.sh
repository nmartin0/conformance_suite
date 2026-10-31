# tr ignores LC_CTYPE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
tr() ( unset LC_CTYPE; command tr "$@" )
