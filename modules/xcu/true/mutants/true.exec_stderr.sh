# exec true writes to stderr
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone true, first on PATH; exec finds it.
mkdir -p pc-mutbin
printf '#!/bin/sh\necho x >&2\n' >pc-mutbin/true
chmod +x pc-mutbin/true
PATH=$PWD/pc-mutbin:$PATH
