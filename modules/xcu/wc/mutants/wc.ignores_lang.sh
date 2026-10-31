# wc ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
wc() ( unset LANG; command wc "$@" )
