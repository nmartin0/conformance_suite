# cp ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( unset LANG; command cp "$@" )
