# Pitonas
**My first programming language!**  
Minimal syntax, very lightweight, very slow, very tedius to write, because all of the keywords are in **Lithuanian** (with its non ASCII characters)

# Docs
## Setting up
To start writing code in this language, you first need to download/build the runtime `pitonas.exe` and stdlib `standartinė_biblioteka.dll` (its not *mandatory* to have it, but its basically required, because to do any meaningful things (add numbers, print stuff) you need it). Also, its advised to also have the `libs` file, it helps to find libraries with *maybe* changed names. All of these files must be placed in the same directory.
### `libs` syntax
```
"<libname in user code>" "<filename 1>" "<filename 2>" "<filename 3>";
```
You may add as many filenames as you like. This file does not support any comments.

### How to run your program
To run your program, you must provide the path to it as the first argument to the runtime executable (`pitonas.exe`). For example, if your executable is in the `C:` disk with the program there also, you would run the program by dragging the code to the executable or run it this command in the Windows console: (assuming your code file is named `code.pi`)
```
C:\> pitonas.exe code.pi
```

## Programming
### Syntax
First of all, there are only a couple of syntax symbols in this language. Heres all of them:
- **`"`**: Its only used to open or close strings
- **`#`**: Used to mark comments. Comments can also be closed by writing the hashtag again
- **`{`**: Used only when opening blocks, more on them later
- **`}`**: Used only for closing blocks
- **`;`**: Used only to mark the end of a line
- **`\`**: In a string, used to mark the next symbol "escaped". Currently, there are only 3 uses for it:
  - **`\"`**: Marks the quites as a part of the string, instead of ending it
  - **`\n`**: Makes a new line in the string, altough, strings may be multiline
  - **`\\`**: In a string reduces to `\`

### Special keywords
- Block keywords (associated with making new blocks):
  - **`funkcija`**: Used to make new functions
    - **`globali`**: A function that has no access to variables outside its block (scope)
    - **`vietinė`**: Unlike a global function, this CAN access variables outside its block (scope)
  - **`jeigu`**: Used to start an IF statement
  - **`priešingai`**: Used to make an ELSE statement after an IF statement
  - **`kilpa`**: Used to make loops
- Command keywords:
  - **`kintamasis`**: Used to create or modify variables
  - **`grąžink`**: Used to return some value (or null) from a function
  - **`importuok`**: Used to import functions from libraries
 
### Concepts
- **Literal**: Anything that is written using quotes. Example of a literal string: `"Hello, World!"`, example of a non literal string: `Hello_Wordl!` (any characters may be used in a non literal string as long as they are not listed in the special syntax section.
- **Token**: A token is how the interpreter reads your code. Every string, every variable name, every special keyword is 1 token. Tokens are seperated by spaces, except if the space is in a string. An example of **3** tokens: `Hello, World! "Hello, World!"`
- **Block**: A block is some part of the code. Every program has at least 1 block, it is the whole program, but the main block may have children blocks. Blocks are started by writing `{` and closed by `}`. It is very easy to know a block start and an end, you just look for `{` and `}` symbols and thats your block.

### Standard library
You can see all of the available functions here (wait, let me finish the file first)

### Variables
Defining variables is really simple. A variable definition takes in 2, 3 or any amount of tokens, depending if you are setting the value or not.  
The simplest way to define a variable is to just make it without assigning it any value
```code
kintamasis variable_name;
# first   | the variable #
# token   | name to make #
# to tell |------------- #
# the interpreter that   #
# we are making a new    #
# variable               #
```
If you want to assign a value to a variable (doesn't matter if it was already defined or not) you would need to write this:
```code
kintamasis variable_name "Hello, World!"
```
This would create a new variable with the value being `Hello, World!`. The 3rd way to define/change a variable is by storing the return value of a function. This is coneverd in the **Invoking/using functions** section.

### Importing libraries
To do anything meaningful, you will at least need to import the standard library. Its done via the `importuok` keyword. Heres an example importing all of the console functions:
```code
importuok "standartinė_biblioteka/konsolė";
# first   | the dll we are       | which     #
# token   | importing the        | namespace #
# to tell | functions from       | to import #
# the     |----------------------|---------- #
# interpreter that we are importing a        #
# library                                    #
# ------------------------------------------ #
# Now, any functions defined inside          #
# "standartinė_biblioteka/konsolė" will be   #
# available in this format:                  #
# "konsolė.function_name"                    #
```
Also, you may change the namespace of the imported functions by providing it as the 3rd token (it doesn't matter if its literal as long as it doesn't contain any spaces). Example:
```code
importuok "standartinė_biblioteka/konsolė" "";
# _________________________________________^^ #
# The functions inside the                    #
# "standartinė_biblioteka/konsolė"            #
# will now be available in this format:       #
# "function_name". You also may change that   #
# to anything other, so if it was "console"   #
# the functions would be available like this  #
# "console.function_name"                     #
```

### Invoking/using functions
It may be an imported or your defined function, but the way to invoke them is the same. Invoking a function without saving its output to a variable is really simple:
```code
function_name;
```
Here, the function was invoked without any parameters, but if you'd like to also pass some parameters, it would look like this:
```code
function_name variable_name1 variable_name2 "A literal value can also be passed";
```
Here, for the code to work, not only `function_name` function must be defined, but also `variable_name1` and `variable_name2` variables.  
To save the value to a variable, you must add 2 tokens to the start of the invocation, but leave the end as is (with or without parameters)
```code
kintamasis result_of_function function_name;
```
or with parameters:
```code
kintamasis result_of_function function_name variable_name1 variable_name2 "A literal value can also be passed";
```
Now, a new variable `result_of_function` will be available containing the return value of the `function_name` function.

### Defining functions
To start defining a function you first must write the `funkcija` keyword. After it, you may choose if the function is global - `globali` or local - `vietinė`. Heres an example of how to define an empty function:
```code
funkcija globali function_name
{
}
```
This function has no code inside itself, so it would immediatly exit and return an empty string (no value). A more interesting function would have some parameters and print them in reverse order, on different lines:
```code
funkcija globali function_name param1 param2 param3
{
  konsolė.atspausdink param3;
  konsolė.atspausdink param2;
  konsolė.atspausdink param1;
}
```
A function cannot be **undersupplied** with arguments (not enough arguments given), but it may be **oversupplied** with arguments (more arguments given than parameters defined). The excess arguments will be stored in an automatically defined variable `argumentai` as a list.  
Below, is some code demonstrating the difference between local and global functions: (I have plans to change a bit how the local/global functions work)
```code
funkcija globali global_printVar
{
	global_printVar; # this function has access to itself #
}
funkcija vietinė local_printVar
{
	local_printVar; # this function DOESN'T have access to itself, so this line will error #
}
```
Functions may return a single value. This can be achieved using the `grąžink` keyword. Heres an example of a function returning a predefined value and the value being printed:
```code
funkcija globali get_value
{
  grąžink "Hello, World!;
}
konsolė.atspausdink get_value;
```

### IF statements
If statements are also really simple, they just check if the provided variable is true - "taip" or false - "ne". There are some function that return such booleans in the stdlib (`standartinė_biblioteka/logika`)
```code
kintamasis ans konsolė.įvesk "Enter a boolean: ";
jeigu ans
{
  konsolė.atspausdink "You entered TRUE";
}
atvirkščiai
{
  konsolė.atspausdink "You entered FALSE";
}
```
Note: an error will be thrown if a non boolean argument is supplied to the IF statement, but there are some function to check if the entered type is a boolean in the stdlib.

### Loops
Loops are also very simple, take only one argument and loop while its true. Heres an example of a that only exists if the user enteres "taip":
```code
kintamasis in_loop "taip";
kilpa in_loop
{
  kintamasis in_loop konsolė.įvesk "Enter \"ne\" to break out of the loop: ";
}
```

### Working examples
All of the time the examples were unfinished, they were lacking any imports. Now, this section is for those who had a hard time understanding all of the bs that is above. Heres a couple of fully finished, working examples:  
**Count to predefined number**
```code
importuok "standartinė_biblioteka/konsolė";
importuok "standartinė_biblioteka/aritmetika";
importuok "standartinė_biblioteka/logika";

kintamasis count_to "5"; # Set the number to count to #
kintamasis current_count "0"; # Make a variable to store the current cound #
konsolė.atspausdink "Counting to " count_to; # Print the number we are counting to #

kintamasis counting "taip"; # Add a variable to stop the loop #
kilpa counting # Make the counting loop #
{
	kintamasis current_count aritmetika.sudėk current_count "1"; # Add one to the current count #
	kintamasis counting logika.daugiau count_to current_count; # Add an exit condition (when the number we are counting to is larger than the current number, the loop will continue #
	konsolė.atspausdink current_count;
}

konsolė.atspausdink "Done counting to " count_to;
konsolė.palauk;
```
**Count to a number entered by the user**
```code
importuok "standartinė_biblioteka/konsolė";
importuok "standartinė_biblioteka/aritmetika";
importuok "standartinė_biblioteka/logika";
importuok "standartinė_biblioteka/tipai";

kintamasis havent_evntered_number "taip"; # Add an exit condition to the input loop #
kilpa havent_evntered_number # Make the input loop #
{
	kintamasis count_to konsolė.įvesk "Enter the number to count to: "; # Prompt the user to enter a number #
	kintamasis havent_evntered_number tipai.yra_skaičius count_to; # Check if what they entered is a number #
	kintamasis havent_evntered_number logika.atvirkščiai havent_evntered_number; # Flip the result for a correct loop #
}

kintamasis current_count "0"; # Make a variable to store the current cound #
konsolė.atspausdink "Counting to " count_to; # Print the number we are counting to #

kintamasis counting "taip"; # Add a variable to stop the loop #
kilpa counting # Make the counting loop #
{
	kintamasis current_count aritmetika.sudėk current_count "1"; # Add one to the current count #
	kintamasis counting logika.daugiau count_to current_count; # Add an exit condition (when the number we are counting to is larger than the current number, the loop will continue #
	konsolė.atspausdink current_count;
}

konsolė.atspausdink "Done counting to " count_to;
konsolė.palauk;
```
```code
importuok "standartinė_biblioteka/konsolė";
importuok "standartinė_biblioteka/aritmetika";
importuok "standartinė_biblioteka/logika";
importuok "standartinė_biblioteka/tipai";

funkcija globali enter_number display_text # Define a function to not repeat code #
{
	kintamasis havent_entered_number "taip"; # Make a variable to track when to exit the loop #
	kilpa havent_entered_number # Make the loop #
	{
		kintamasis entered_number konsolė.įvesk display_text; # Prompt the user to enter a number #
		kintamasis havent_entered_number tipai.yra_skaičius entered_number; # Check if what they entered is a number #
		kintamasis havent_entered_number logika.atvirkščiai havent_entered_number; # Reverse the result to not exit the loop if they didn't enter a number #
	}
	grąžink entered_number; # Return the entered number #
}

kintamasis loop "taip"; # Currently, its impossible to make a loop with a literal value, so it must be made into a variable, but I'll fix this in the next version of the runtime #
kilpa loop # Define the loop #
{
	kintamasis 1st_number enter_number "Enter the first number: "; # Use our defined function to prompt for the first number #
	kintamasis 2nd_number enter_number "Enter the second number: "; # Prompt for the second number #
	
	# Below is all of the printing of the results #

	kintamasis calculated aritmetika.sudėk 1st_number 2nd_number;
	konsolė.atspausdink 1st_number " + " 2nd_number " = " calculated;
	
	kintamasis calculated aritmetika.atimk 1st_number 2nd_number;
	konsolė.atspausdink 1st_number " - " 2nd_number " = " calculated;
	
	kintamasis calculated aritmetika.padaugink 1st_number 2nd_number;
	konsolė.atspausdink 1st_number " * " 2nd_number " = " calculated;
	
	kintamasis calculated aritmetika.padalink 1st_number 2nd_number;
	konsolė.atspausdink 1st_number " / " 2nd_number " = " calculated;
}
```

### To the still alive reader
Thanks for checking out my repository! Its my first language and the first time writing documentation, so I'm sorry if some parts were unclear, you may open an issue if you have questions or find bugs in my language! Again, thanks for checking it out. Also, for those who are also looking into making their own language, don't be stumped by the question of *how* to make the language. Just sit down and try whatever! It was really fun making this language, I hope you'll have fun making your own too!
