#!/bin/sh

# The output should be:
# 
# 6
# 15
# 127
# 12

jc << EOF
const 2
const 4
add
p
const 3
const 31
const 6
div
mul
p
const 7
const 8
mul
const 8
const 10
mul
add
sub
sub
p
const 10
div
p
EOF

# 2 4 +
# p
# 3 31 6 / * p
# 7 8 * 8 10 * + - - p
# 10 / p
# EOF



