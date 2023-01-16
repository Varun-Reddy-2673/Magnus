# Magnus

## Overview

Magnus is an interpreted programming language written using C++.

## Syntax

### The basics

The main block of code that is to be executed is written under **main**.

> main
>   { main code }
> end

Lines in Magnus **do not** end with a semicolon (;). An EOL will suffice.

> project_name = 'Magnus'

Blocks of code, for example **if** blocks and **main** blocks, start with the corresponding defining keyword and end with the **end** keyword. Curly braces ({}) are not used and tab-spaces are only used for readability.
<br>
New variables do not require any special form of declaration. The initial value has to be directly assigned during initialisation.

### Datatypes

**num** is used for numeric values. Both integers and decimals.

> five = 5
> pi_rounded_to_two_digits = 3.14

**str** behaves like how strings do in any other programming language. They are defined using single-quotes ('').

> cliche_string = 'Hello World!'

**bool** can only hold one of **true** and **false**.

> is_value_of_this_variable_true = false

**array** is used to hold multiple values. It is not necessary that all the values within an array are of the same datatype. They are defined using square-braces ([]). A comma (,) is **not** required to separate multiple values within an array.

> sample_array = [five, 'Hi', ['nested', 'array']]

### Blocks

**main** contains the code that is executed at the start of the program.
<br>
**if**, **elif**, and **else** are conditional blocks whose execution depends on the value of the conditional statement.

> if { condition }
>   { code }
> elif { condition }
>   { code }
> else
>   { code }
> end

**repeat** is the Magnus equivalent of a while loop.

> repeat { condition }
>   { code }
> end

**block** is used to write your own functions. **return** is used to return a value.

> block add (a b)
>   return a + b
> end
