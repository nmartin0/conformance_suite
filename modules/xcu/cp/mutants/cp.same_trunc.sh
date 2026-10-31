# copying a file onto itself empties it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( if [ $# -eq 2 ] && [ "$1" -ef "$2" ]; then : >"$2"; exit 0; fi; command cp "$@" )
