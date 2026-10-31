# cp ignores LC_CTYPE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( unset LC_CTYPE; command cp "$@" )
