# the edited lines are written to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() { command sed "$@" >&2; }
