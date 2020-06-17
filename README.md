Program receives one param from console - path to test. 
For example: ```./translate tests/test04```  
By default **tests/test01** path is being used. 
_Program doesn't work with list of paths._
If any lexic error is found it's being printed to the table_of_tokens.txt file.
If any syntax error is found it's being printed to the tree.txt file.
If any semantic error is found it's being printed to the generated.asm file as a comment. Example: ```;SomeErrorMessage```.
Also there is a lable in the end of generated.asm file ```;STATUS OK``` if no semantic errors and ```;STATUS FAILED``` if semantic errors are found. 

### Part of Signal grammar:

```
<signal-program> --> <program>
<program> --> PROGRAM <procedure-identifier> ;
<block>.
<block> --> <declarations> BEGIN <statements-
list> END
<statements-list> --> <empty>
<declarations> --> <math-function-declarations>
<math-function-declarations> --> DEFFUNC
<function-list> |
<empty>
<function-list> --> <function> <function-list>
|
<empty>
<function> --> <function-identifier> =
<constant><function-characteristic> ;
<function-characteristic> --> \ <unsigned-
integer> , <unsigned-integer>
<constant> --> <unsigned-integer>
<procedure-identifier> --> <identifier>
<function-identifier> --> <identifier>
<identifier> --> <letter><string>
<string> --> <letter><string> |
<digit><string> |
<empty>
<unsigned-integer> --> <digit><digits-string>
<digits-string> --> <digit><digits-string> |
<empty>
<digit> --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
9
<letter> --> A | B | C | D | ... | Z
```


NTUU “Igor Sikorsky Kyiv Polytechnic Institute”, FAM, 06.17.2020.