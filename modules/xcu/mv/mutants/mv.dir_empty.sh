# a directory is renamed to an empty directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -d "$1" ] && [ ! -d "$2" ]; then mkdir "$2" && rm -rf "$1"; exit; fi; command mv "$@" )
