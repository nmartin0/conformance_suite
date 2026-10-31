# cp ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( unset LC_ALL; command cp "$@" )
