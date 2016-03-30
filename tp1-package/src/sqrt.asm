global asmsqrt

section .data

asmsqrt:

sqrtsd xmm0, [rdi]

ret