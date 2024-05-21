# List of test cases we should test with at the end 
# Let's keep adding to this as and when we think of something

# spaces
   echo abc   # 3 spaces after abc

# operators: |, ||, <, <<, >, >>, &&, ()
cat file.txt |< echo "Hello world"

# unsupport operators: 

# parenthesis
cat input.txt && echo abc || echo def
cat input.txt && (echo abc || echo def)

# quotes
ech"o" "ab""bc""$SHLVL"'$SHLVL'
ls "'"

# wildcard expansion
norminette ***.c
