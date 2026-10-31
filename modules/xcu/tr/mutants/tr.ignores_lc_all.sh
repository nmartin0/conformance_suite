# tr ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
tr() ( unset LC_ALL; command tr "$@" )
