# copying a file onto itself removes it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( if [ $# -eq 2 ] && [ "$1" -ef "$2" ]; then rm -f "$2"; exit 0; fi; command cp "$@" )
