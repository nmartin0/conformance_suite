# rm ignores LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( unset LC_COLLATE; command rm "$@" )
