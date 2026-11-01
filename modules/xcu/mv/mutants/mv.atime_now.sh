# the access time of the moved file is set to now
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( command mv "$@" || exit; if [ $# -eq 2 ]; then touch -a "$2"; fi; exit 0 )
