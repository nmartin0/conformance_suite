# cut ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cut() ( unset LC_ALL; command cut "$@" )
