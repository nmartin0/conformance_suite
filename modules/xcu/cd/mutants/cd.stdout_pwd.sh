# cd writes the new directory to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@" && printf '%s\n' "$PWD"; }
