# with -f nothing is copied and the status is 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( for a; do [ "$a" = -f ] && exit 0; done; command cp "$@" )
