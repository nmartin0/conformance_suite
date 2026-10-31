# fixture: wrong only under shell a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
if [ "${PC_FX_WHICH:-}" = a ]; then pcfs_answer() { echo 41; }; else pcfs_answer() { echo 42; }; fi
