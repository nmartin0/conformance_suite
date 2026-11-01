# a target named - is standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( if [ $# -eq 2 ] && [ "$2" = - ]; then cat "$1"; exit; fi; command cp "$@" )
