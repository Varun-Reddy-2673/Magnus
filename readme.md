# Magnus

## Overview

Magnus is an interpreted programming language written using C++.

## Syntax

### The basics

<p>The main block of code that is to be executed is written under **main**.</p>

> main
>   { main code }
> end

<p>Lines in Magnus **do not** end with a semicolon (;). An EOL will suffice.</p>

> project_name = 'Magnus'

<p>Blocks of code, for example **if** blocks and **main** blocks, start with the corresponding defining keyword and end with the **end** keyword. Curly braces ({}) are not used and tab-spaces are only used for readability.</p>
<p>New variables do not require any special form of declaration. The initial value has to be directly assigned during initialisation.</p>

### Datatypes

<p>**num** is used for numeric values. Both integers and decimals.</p>

> five = 5
> pi_rounded_to_two_digits = 3.14

<p>**str** behaves like how strings do in any other programming language. They are defined using single-quotes ('').</p>

> cliche_string = 'Hello World!'

<p>**bool** can only hold one of **true** and **false**.</p>

> is_value_of_this_variable_true = false

<p>**array** is used to hold multiple values. It is not necessary that all the values within an array are of the same datatype. They are defined using square-braces ([]). A comma (,) is **not** required to separate multiple values within an array.</p>

> sample_array = [five, 'Hi', ['nested', 'array']]

### Blocks

<p>**main** contains the code that is executed at the start of the program.</p>
<p>**if**, **elif**, and **else** are conditional blocks whose execution depends on the value of the conditional statement.</p>

> if { condition }
>   { code }
> elif { condition }
>   { code }
> else
>   { code }
> end

<p>**repeat** is the Magnus equivalent of a while loop.</p>

> repeat { condition }
>   { code }
> end

<p>**block** is used to write your own functions. **return** is used to return a value.</p>

> block add (a b)
>   return a + b
> end
