# rm ignores LC_MESSAGES
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( unset LC_MESSAGES; command rm "$@" )
