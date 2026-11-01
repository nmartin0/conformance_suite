# the contents are emptied when source and destination are the same file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ "$1" -ef "$2" ]; then : >"$2"; exit 0; fi; command mv "$@" )
