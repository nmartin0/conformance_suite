# cp writes its diagnostics to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( command cp "$@" 2>&1 )
