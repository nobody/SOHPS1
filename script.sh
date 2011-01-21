#!/bin/sh

# Example script.sh file
# Edwin Sha
#
# This is a shell script which you can use to test your programs
# once they're written.  If any of it seems confusing to you,
# read the man page on "sh".  The first line of all shell scripts
# should be "#!/bin/sh".

# Set the path. Note that there's nothing user-dependant here except
# for '.'
#
# '.' should always be at the end of your path.



# First make jc:

make jc
# In the later assignment, you should use gmake instead of make.
# Next, pass input into jc, and put the output into jc.output.
# The output should be:
# 
# 5
# 20
# 131
# 26

jc > jc.output << EOF
const 2
const 3
add
p
const 4
const 31
const 6
div
mul
p
const 7
const 8
mul
const 9
const 10
mul
add
sub
sub
p
const 5
div
p
EOF

# 2 3 +
# p
# 4 31 6 / * p
# 7 8 * 9 10 * + - - p
# 5 / p
# EOF




q


