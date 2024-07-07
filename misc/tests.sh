# List of test cases we should test with at the end 
# Let's keep adding to this as and when we think of something

#############################################################################
# SPACES ####################################################################
#############################################################################
echo -n abc   # i.e., add 3 spaces after abc; prints abc without new line and spaces
echo abc    def #output: abc def

    echo abc #isn't recorded in history 

echo       abc #history includes all the spaces

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

"" #error: command not found (127)
"" echo abc #error: command not found (127)
'' echo abc #error: command not found (127)
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
> a.txt echo >b.txt abc # a.txt and b.txt created; abc directed to b.txt

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

var=
echo abc > $var #error: $var: ambiguous redirect


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
abc=o
ech$abc a #output: a
ech"$abc" a #output: a

abc=
$abc #error: command not found (127)

abc=&& #taken as incomplete line
abc="&&"
echo $abc #output: &&
echo abc $abc echo def #output: abc && echo def

abc="|"
echo abc $abc echo #output: abc | echo

var=out
echo abc  >"$var"put.txt #abc redirected to output.txt

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

var=abc
echo $var #abc
var=
echo $var #<blank>

var1=abc var2=def 123 #error msg for 123
echo $var1 $var2 #blank - var1 and var2 not assigned

var=123 echo abc #output: abc
var=123 echo $var #blank output
var=123 && echo $var #123
var=123 cat input.txt #input.txt contents displayed
var=123 cat input.txt > a.txt #input.txt contents redirected to a.txt

var=123
echo $"$var" # 123
echo $"$var$" #123$
echo $"$var$" $"$var$" #123$ 123$

var=
$var #treated like empty line; not in history; no error
$var echo abc #output: abc

echo hello$*world #output: helloworld
echo hello$%world #output: hello$%world

var=abc
echo $var #abc
var=
echo $var #<blank>

var1=abc var2=def 123 #error msg for 123
echo $var1 $var2 #blank - var1 and var2 not assigned

var=123 echo abc #output: abc
var=123 echo $var #blank output
var=123 && echo $var #123
var=123 cat input.txt #input.txt contents displayed
var=123 cat input.txt > a.txt #input.txt contents redirected to a.txt

var=123
echo $"$var" # 123
echo $"$var$" #123$
echo $"$var$" $"$var$" #123$ 123$

var=
$var #treated like empty line; not in history; no error
$var echo abc #output: abc

echo hello$*world #output: helloworld
echo hello$%world #output: hello$%world

var=123
echo $"$var" # 123
echo $"$var$" #123$
echo $"$var$" $"$var$" #123$ 123$

echo $"abc" #abc
echo $'abc' #abc

var=123 "" #command not found (127)

#############################################################################
# WILDCARD EXPANSIONS #######################################################
#############################################################################
ls *txt
ls i*c # lists files starting with i and ending with c 
ls '*.t' # there are no files ending with t; cannot access *.t 
ls "*.c" # cannot access *.c 
ls '*.c' # cannot access *.c
norminette ***.c

var=*
echo $var #prints all the files in working directory
echo $var abc # prints all the files in working directory followed by abc
echo "$var abc" #prints: * abc

touch test'".c' #create file called: test".c
ls *".c # counts as incomplete line until another " typed in
ls *'".c' #prints 'test.".c'

var=*
echo $var #prints all the files in working directory
echo $var abc # prints all the files in working directory followed by abc
echo "$var abc" #prints: * abc

touch test'".c' #create file called: test".c
ls *".c # counts as incomplete line until another " typed in
ls *'".c' #prints 'test.".c'

var=*
echo $var #prints all the files in working directory
echo $var abc # prints all the files in working directory followed by abc
echo "$var abc" #prints: * abc

touch test'".c' #create file called: test".c
ls *".c # counts as incomplete line until another " typed in
ls *'".c' #prints 'test.".c'

echo e$*e #ee
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
# HEREDOC ###################################################################
#############################################################################
cat << abc << def
abc
text
def
# output: text
# history: all 4 lines

cat << abc << def
abc 
def
#output: nothing 
#history: all 3 lines

var=abc
cat << def
$var
def
#output: abc
#history: cat << def \n $var \n def

var=abc
cat << abc
text
$var
abc
#output: ddd \n abc
#history: all lines but with $var

var=abc
cat << $var
text
abc
$var
#output: text \n abc

cat << a b c
a b c
a
#output: error because no such file or directory for b and c

cat << a a.txt b.txt
text
a
#output: text followed by content of a.txt and b.txt

echo abc << a >a.txt
text
a
# output of abc redirected to a.txt

echo abc && cat << a
fff
a
# output: abc \n fff

var=123
cat << abc
$var
"$var"
'$var'
abc
#output: 123\n"123"\n'123'

cat << ab"c"
$var
"$var"
'$var'
abc
#output: $var\n"$var"\n'$var'

cat << ab'c'
$var
"$var"
'$var'
abc
#output: $var\n"$var"\n'$var'

cat << ab"c"
ab"c"
ab'c'
abc
#output: ab"c"\nab'c'

cat << ab'c'
ab"c"
ab'c'
abc
#output: ab"c"\nab'c'

cat << abc"'"
abc'
#output: nothing

cat << abc "''"
abc
abc''
#output: abc

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
# ( <in echo >out "r")in this case is not working
echo # new line printed 
echo -n #no new line printed
echo -n -n abc # output: abc without new line 
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

export x=y=z
export #x="y=z"
env #x=y=z

var=123
export var=987
echo $var #987
unset $var 
echo $var # nothing i.e., doesn't revert to var=123

var=123
export var=987
echo $var #987
var=888 
echo $var #888
export # var = 888
env # var = 888
unset var # var is unset

export var=123
export 123 = var # error message for 123 and = 
export # var is listed: var=123

var=123
export var
export #var added: var=123
env #var=123

export var=123
export var= #updates var to empty string 
env #var is listed


export abc
export #declare -x abc
unset abc
export abc= 
export #declare -x abc=""

#############################################################################
# CD ######################################################################## 
#############################################################################
cd a.out # error: a.out: Not a directory (exit status = 1)

chmod 0 test_dir
cd test_dir # error: test_dir: Permission denied (exit_status = 1)

cd aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
# error: File name too long (exit = 1)

cd noexist #error: No such file or directory (exit = 1)

#############################################################################
# UNSET ##################################################################### 
#############################################################################
var=123
echo $var #123
unset var
echo $var #blank

export var=123
unset 123 var #error message for 123 but var is unset
echo $? #return value of 1
export #var was unset

#############################################################################
# EXIT ######################################################################
#############################################################################
exit #exit status = 0
exit 00000 #exit status = 0
exit +2 #exit status = 2
exit "" #error message: "exit: : numeric argument required" --> exit status = 2
exit "" "" "" #error message: "exit: : numeric argument required" --> exit status = 2
exit "" "2" "" #error message: "exit: : numeric argument required"
exit "2" "" "" #error message: "exit: too many arguments" --> exit status = 1
exit --2 #error message: "exit: --2: numeric argument required"
exit ++2 #error message: "exit: ++2: numeric argument required"
exit +-2 #error message: "exit: +-2: numeric argument required"
exit -+2 #error message: "exit: -+2: numeric argument required"
exit a #error message: "exit: a: numeric argument required"
exit 1 2 #error message: "exit: too many arguments"
exit -10 #exit status = 246
exit 10 - #error message: "exit: too many arguments"
exit - 10 #error message: "exit: -: numeric argument required"
exit 9223372036854775807 #exit status = 255
exit 9223372036854775808 #error message: "exit: 9223372036854775808: numeric argument required"
exit -9223372036854775808 #exit status = 0
exit -9223372036854775809 #error message: "exit: -9223372036854775809: numeric argument required"

#############################################################################
# MISC ######################################################################
#############################################################################
. #error: filename argument required (2)
.. #error: command not found (127)
./ #error: ../: Is a directory (126)
../ #error: ../: Is a directory (126)
./a #error: no such file or directory (127)

var=
$var #blank (0)
"$var" #error: command not found (127)

"" #error: command not found (127)
"""" #error: command not found (127)

'' #error: command not found (127)
'''' #error: command not found (127)
