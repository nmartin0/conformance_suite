# cp ignores LC_MESSAGES
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( unset LC_MESSAGES; command cp "$@" )
