# a directory replaces the existing file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -d "$1" ] && [ -e "$2" ] && [ ! -d "$2" ]; then rm -f "$2"; command mv "$1" "$2"; exit; fi; command mv "$@" )
