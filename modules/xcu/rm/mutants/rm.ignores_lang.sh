# rm ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( unset LANG; command rm "$@" )
