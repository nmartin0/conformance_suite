# replacing an existing target exits 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -e "$2" ]; then command mv "$@"; exit 1; fi; command mv "$@" )
