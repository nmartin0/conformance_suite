# the target is left empty
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( command mv "$@" || exit; if [ $# -eq 2 ]; then : >"$2"; fi; exit 0 )
