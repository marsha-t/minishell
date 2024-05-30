# List of test cases we should test with at the end 
# Let's keep adding to this as and when we think of something

#############################################################################
# SPACES ####################################################################
#############################################################################
echo -n abc   # i.e., add 3 spaces after abc; prints abc without new line and spaces
echo abc    def #output: abc def

#############################################################################
# QUOTES ####################################################################
#############################################################################
echo 'Hello $USER' #output: Hello $User
echo "Hello $USER" #output: Hello <username>
ech"o" "ab""bc""$SHLVL"'$SHLVL' #abbc1$SHLVL
ech"o abc" #error detecting command
echo '"'  # "
echo 'It'"'"'s' # It's
echo "'"
echo "\"" # Output is " but here, \ is unsupported
ech"o" "a"b"cd'ef" 1> a".txt" #abcd'ef in a.txt

#############################################################################
# PIPES #####################################################################
#############################################################################
echo a | wc -l # results in 1
echo a > a.txt | wc - l # results in 0; a is directed to a.txt

#############################################################################
# REDIRECTION ###############################################################
#############################################################################
> a.txt # no error; a.txt is created or truncated to empty if it exists
>> a.txt # same as above
< a.txt # error if a.txt doesn't exist but otherwise okay (and a.txt is unchanged regardless of what was in it)
> a.txt echo abc # abc is directed into a.txt

# IO NUMBER OUTPUT
echo 1 > exists.txt # 1 in exists.txt
echo 1 > noexists.txt # noexists.txt created and 1 in noexists.txt
echo -1>exists.txt # -1 in exists.txt
echo 1>exists.txt # redirects nothing from standard output to exists.txt (empty file)
echo abc "1">exists.txt # redirects abc 1 to exists.txt
echo a b s 1 > exists.txt # a b s 1 in exists.txt
echo a b s 1> exists.txt #a b s in exists.txt
echo abc 5>exists.txt #abc displayed on screen
cat in 2>exists.txt #error directed into exists.txt
cat in>y.txt #error displayed on screen; y.txt created
echo a > a.txt > b.txt > c.txt #a.txt and b.txt are created and are empty; a in c.txt
echo abc a>a.txt>b.txt # a.txt and b.txt are created; a.txt is empty; abc in b.txt
echo abc 1>input.txt >extra.txt #nothing in input.txt; abc in extra.txt
echo abc 1>b.txt1>a.txt #abc directed to a.txt but b.txt1 created
echo abc > a.txt abc # file called a.txt and inside: abc abc

# IO NUMBER INPUT 
cat 0< exists.txt # equivalent to cat < exists.txt
cat 5<exists.txt # redirect contents of exists.txt to 5 and cat reads from fd 5
cat < a.txt < c.txt # both files exist; will display only c.txt contents
cat < noexist.txt < c.txt # error for noexist.txt
cat < c.txt < noexist.txt # error for noexist.txt

# IO NUMBER APPEND
echo 1>>a.txt # appends to a.txt
echo 2>>a.txt # abc in output; nothing in a.txt
echo af >>a.txt >>b.txt #appends only once to b.txt
echo af >>a.txt >> a.txt # appends only once in a.txt (i.e., the last one)
echo abc >a.txt>>b.txt #both files created and appended to b.txt
echo abc >>b.txt>a.txt #both files created and output directed to a.txt

# REDIRECTION AND QUOTES
echo b '>c.txt' # prints b >c.txt
echo b \>c.txt # prints b >c.txt
echo b > \c.txt # b in c.txt 
echo b >\c.txt # b in c.txt 
echo abc > "c.txt" # abc in c.txt
echo abc >">" a.txt #file called > and output inside is abc a.txt
echo "1"">"a.txt # output: 1>a.txt
echo "1">"a.txt" # output of 1 in a.txt
echo "1"1>a.txt # output of 11 in a.txt

#############################################################################
# COMBINED OPERATORS ########################################################
#############################################################################
cat file.txt |< echo "Hello world"

#############################################################################
# PARENTHESIS ###############################################################
#############################################################################
# cmd1 && cmd2 || cmd3 
cat input.txt && echo abc || echo def # error + def
echo abc && cat input.txt || echo def # abc + error + def
echo abc && echo def || echo ghi # abc + def 
  
# cmd1 && (cmd2 || cmd3)
cat input.txt && (echo abc || echo def) # error
echo abc && (cat input.txt || echo def) # abc + error + def
echo abc && (echo def || echo ghi) # abc + def

echo a ( || echo c && echo d) #error near (

#############################################################################
# VARIABLE ASSIGNMENT & EXPANSIONS ##########################################
#############################################################################
var=a
ech$abc a #output: a
ech"$abc" a #output: a

abc=&& #taken as incomplete line
abc="&&"
echo $abc #output: &&
echo abc $abc echo def #output: abc && echo def

abc="|"
echo abc $abc echo #output: abc | echo

var=Hello
echo $var

var=in
cat <"$var"put.txt

var"="abc #error: var=abc command not found
var'='abc #error: var=abc command not found
va"r"=abc #error: var=abc command not found

var=ab"c"
echo $var #abc

var=abc=
echo $var #abc=

var=abc=def
echo $var #abc=def

var==abc
echo $var #=abc

#############################################################################
# WILDCARD EXPANSIONS #######################################################
#############################################################################
ls *txt
norminette ***.c

#############################################################################
# REDIRECTION ORDER #########################################################
#############################################################################

VAR=123 > a.txt echo "Hello $VAR" #Hello in a.txt 
VAR=123 > a.txt echo 'Hello $VAR' #Hello $VAR in a.txt 

VAR=123 
> a.txt echo 'Hello $VAR' #Hello 123 in a.txt

VAR=123 echo "Hello $VAR" > a.txt # Hello 123 in a.txt

#############################################################################
# FILES #####################################################################
#############################################################################
chmod 0 a.txt
echo abc > a.txt # error msg: Permission denied

#############################################################################
# ECHO ######################################################################
#############################################################################
echo -n abc       # output: abc without new line
echo -nnnnnn abc  # output: abc without new line
echo -n -n abc    # output: abc without new line
echo abc -n       # output: abc -n
echo abc -n def   #output: abc -n def
echo -n -invalid abc  #output: -invalid abc without new line
echo -invalid -n abc  #output: -invalid -n abc with new line
echo # new line printed 
echo -n #no new line printed

#############################################################################
# PWD #######################################################################
#############################################################################
pwd -n # error: invalid option
pwd a # prints working dir
pwd a b a b # prints working dir
pwd a -n # prints working dir

#############################################################################
# EXPORT #################################################################### 
#############################################################################
export hello=world test
export #test='' listed
env #hello=world but test is not added

export hello=world test=
export #test='' listed
env #test='' added
