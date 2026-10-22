# exec true exits 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone true, first on PATH; exec finds it.
mkdir -p pc-mutbin
printf '#!/bin/sh\nexit 1\n' >pc-mutbin/true
chmod +x pc-mutbin/true
PATH=$PWD/pc-mutbin:$PATH
