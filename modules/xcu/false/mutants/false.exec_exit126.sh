# exec false exits 126
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone false, first on PATH; exec finds it.
mkdir -p pc-mutbin
printf '#!/bin/sh\nexit 126\n' >pc-mutbin/false
chmod +x pc-mutbin/false
PATH=$PWD/pc-mutbin:$PATH
