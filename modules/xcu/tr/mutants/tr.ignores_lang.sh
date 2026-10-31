# tr ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
tr() ( unset LANG; command tr "$@" )
