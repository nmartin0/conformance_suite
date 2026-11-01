# join ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
join() ( unset LC_ALL; command join "$@" )
