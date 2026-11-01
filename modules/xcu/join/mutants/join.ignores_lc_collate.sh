# join ignores LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
join() ( unset LC_COLLATE; command join "$@" )
