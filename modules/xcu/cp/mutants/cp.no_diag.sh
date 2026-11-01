# cp hides its diagnostics
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( command cp "$@" 2>/dev/null )
