# rm ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( unset LC_ALL; command rm "$@" )
