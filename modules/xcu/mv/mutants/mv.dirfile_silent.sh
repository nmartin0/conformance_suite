# a directory onto a file gives status 1 and no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -d "$1" ] && [ -e "$2" ] && [ ! -d "$2" ]; then exit 1; fi; command mv "$@" )
