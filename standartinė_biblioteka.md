# STD Lib (standartinė_biblioteka)
Here are all of the namespaces and functions in the stdlib:
- **`konsolė`**: Commands with the console
  - **`atspausdink`**:  
    **Parameters**: `[string...]`  
    **Returns**: *none*  
    **Description**: Prints all of the arguments to the console
  - **`įvesk`**:  
    **Parameters**: `[string...]`  
    **Returns**: `string`  
    **Description**: Prints all of the arguments and returns one line that the user enters
  - **`palauk`**:  
    **Parameters**: *none*  
    **Returns**: *none*  
    **Description**: Waits for any key press by invoking the native windows console command "pause"
- **`atitmetika`**: Commands with artihmetic
  - **`sudėk`**:  
    **Parameters**: `<number 2...>`  
    **Returns**: `number`  
    **Description**: Sums up all of the given numbers
  - **`atimk`**:  
    **Parameters**: `<number 2...>`  
    **Returns**: `number`  
    **Description**: Takes away all of the numbers from the first one
  - **`padaugink`**:  
    **Parameters**: `<number 2...>`  
    **Returns**: `number`  
    **Description**: Multiplies all of the given numbers together
  - **`padalink`**:  
    **Parameters**: `<number 2...>`  
    **Returns**: `number`  
    **Description**: Divides all of the numbers from the first one
- **`logika`**: Logic commands
  - **`daugiau`**:  
    **Parameters**: `<number> <number>`  
    **Returns**: `boolean`  
    **Description**: Returns **TRUE** if the first number is more than the second number, **FALSE** otherwise
  - **`mažiau`**:  
    **Parameters**: `<number> <number>`  
    **Returns**: `boolean`  
    **Description**: Returns **TRUE** if the second number is more than the first number, **FALSE** otherwise
  - **`lygu`**:  
    **Parameters**: `<any> <any>`  
    **Returns**: `boolean`  
    **Description**: Returns **TRUE** if the given values match, **FALSE** otherwise
  - **`nelygu`**:  
    **Parameters**: `<any> <any>`  
    **Returns**: `boolean`  
    **Description**: Returns **TRUE** if the given values don't match, **FALSE** otherwise
  - **`ir`**:  
    **Parameters**: `<boolean 2...>`  
    **Returns**: `boolean`  
    **Description**: Returns **TRUE** if all of the given values are TRUE, **FALSE** otherwise
  - **`arba`**:  
    **Parameters**: `<boolean 2...>`  
    **Returns**: `boolean`  
    **Description**: Returns **TRUE** if at least one of the given values is TRUE, **FALSE** otherwise
  - **`atvirkščiai`**:  
    **Parameters**: `<boolean>`  
    **Returns**: `boolean`  
    **Description**: Inverts the given boolean
- **`tipai`**: Commands with variable types
  - **`yra_skaičius`**:  
    **Parameters**: `<any>`  
    **Returns**: `boolean`  
    **Description**: Returns **TRUE** if the given value is a NUMBER, **FALSE** otherwise
  - **`yra_loginis`**:  
    **Parameters**: `<any>`  
    **Returns**: `boolean`  
    **Description**: Returns **TRUE** if the given value is a BOOLEAN, **FALSE** otherwise
- **`sistema`**: Commands with the system
  - **`laikas`**:  
    **Parameters**: *none*  
    **Returns**: `number`  
    **Description**: Returns the current time
  - **`komanada`**:  
    **Parameters**: `<any>`  
    **Returns**: *none*  
    **Description**: Executes the given console command
