# sed suppresses the default output even without -n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() { command sed -n "$@"; }
