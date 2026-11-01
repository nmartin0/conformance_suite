# join ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
join() ( unset LANG; command join "$@" )
