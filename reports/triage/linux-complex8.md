# Linux triage: eight complex-function symmetry FAILs

Every one of these pages says (MXC option) that the result at the conjugate or negation of an argument is "exactly the same value" as the conjugate or negation of the result, "including for the special values". The helper compares part for part, with zero signs and infinity signs exact and any NaN matching any NaN. Re-reading it found no fault: no comparison of -0 with +0 by value, no NaN-sign test, no conj() of a signed zero, and no type or macro issue. The failure lines print with %Lg, which hides a last-bit difference, but a direct %a check (double, same strict flags, with or without -O2) shows genuinely different bits.

| ID | decision | reason | change |
|---|---|---|---|
| XSH.catan.014 | SYSTEM | catan(-1.5+0.75i) and catan(-1.5-0.75i) differ in the last bit of the imaginary part (0x1.a9d8f3ed9f95ap-3 against 0x1.a9d8f3ed9f959p-3); the page requires exact equality. Bit-identical results seen elsewhere were presumably another type or a different build. | docs/linux-deviations.md O.54, table rows, known/linux-glibc.known; test unchanged |
| XSH.catan.015 | SYSTEM | catan at (1.5, -0.75) is not the exact negation of catan at (-1.5, 0.75): imaginary part 0x1.a9d8f3ed9f959p-3 against 0x1.a9d8f3ed9f95ap-3. | as above |
| XSH.catanh.015 | SYSTEM | catanh(0.5+0.25i) real part 0x1.00307f2440124p-1, catanh(-0.5-0.25i) real part -0x1.00307f2440123p-1: not exact negations. | as above |
| XSH.ccos.012 | SYSTEM | ccos(Inf+0i) and ccos(Inf-0i) both return NaN+0i. The sign of the zero is unspecified at one point, but exact conjugate symmetry then fixes the opposite sign at the other. The test checks only the consistency, not which sign (Q-113 already records this reading; NaN sign is not compared). | as above |
| XSH.ccosh.012 | SYSTEM | Same, at (0, Inf) and (0, -Inf): both NaN+0i. | as above |
| XSH.csin.012 | SYSTEM | csin(Inf+iInf) and csin(Inf-iInf) both give an imaginary part of +Inf; the conjugate rule requires opposite signs. | as above |
| XSH.csin.013 | SYSTEM | csin(Inf+iInf) and csin(-Inf-iInf) both give an imaginary part of +Inf; the odd rule requires opposite signs. | as above |
| XSH.csinh.013 | SYSTEM | csinh(Inf+iInf) and csinh(-Inf-iInf) both give a real part of +Inf; the odd rule requires opposite signs. | as above |

No test source was changed, and no standard issue added (Q-113 covers the unspecified-sign reading). The failure message prints with %Lg; the test text was left alone since the tests are correct, but a later change to %La in the shared helper would show last-bit cases at a glance.
