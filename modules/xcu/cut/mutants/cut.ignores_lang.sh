# cut ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cut() ( unset LANG; command cut "$@" )
