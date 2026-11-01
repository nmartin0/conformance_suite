# join always compares by byte value
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
join() ( LC_ALL=POSIX; export LC_ALL; unset LC_COLLATE LANG; command join "$@" )
