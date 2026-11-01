# a directory onto a file gives a diagnostic and status 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -d "$1" ] && [ -e "$2" ] && [ ! -d "$2" ]; then echo "mv: cannot move a directory onto a file" >&2; exit 0; fi; command mv "$@" )
