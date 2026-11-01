# a directory is copied and not removed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -d "$1" ] && [ ! -d "$2" ]; then command cp -R "$1" "$2"; exit; fi; command mv "$@" )
