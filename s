#!/bin/sh

# Write my stuff in file "s.output"
cat > s.output << EOF
Hello! 

<blank line above>
That's all I have! Belows are what you said:
EOF

# Prompt for your input
echo "Tell me something:"
echo "(Use \"<CNTL-D>\" to quit)"

# Append your input to file "s.output"
cat >> s.output

# Wrap up
echo 
echo "------------------------------------"
echo "That's all."
echo "Please see the result in file \"s.output\""












