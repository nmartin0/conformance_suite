# wc ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
wc() ( unset LC_ALL; command wc "$@" )
