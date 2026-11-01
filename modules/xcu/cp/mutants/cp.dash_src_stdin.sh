# a source named - is read from standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( if [ $# -eq 2 ] && [ "$1" = - ]; then cat >"$2"; exit; fi; command cp "$@" )
