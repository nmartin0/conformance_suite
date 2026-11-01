# a case command with the status of an ignored failure ends the shell under -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^case x in x) false && true ;; esac$/case x in x) false \&\& true ;; esac || exit $?/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
