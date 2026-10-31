# copied directories get the source mode, the creation mask not applied
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( umask 000; command cp "$@" )
