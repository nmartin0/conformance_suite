# a directory is renamed with its plain files only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -d "$1" ] && [ ! -d "$2" ]; then mkdir "$2" || exit 1; for f in "$1"/*; do if [ -f "$f" ]; then command cp "$f" "$2/"; fi; done; rm -rf "$1"; exit 0; fi; command mv "$@" )
