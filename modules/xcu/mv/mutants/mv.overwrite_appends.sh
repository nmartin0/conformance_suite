# the contents are appended to the existing target
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -f "$2" ]; then cat "$1" >>"$2" && rm -f "$1"; exit; fi; command mv "$@" )
