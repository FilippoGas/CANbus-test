# CANbus-test

Basic implementation of the principal CAN utilities such as, **candump**, **canlog**, **canplay**.

>## Candump
Candump is used to receive can messages on the specified interface, and display them.
### Compiling
`gcc -o [name] canplay_test.c`
### Usage
>
`./[name] [interface]`

>## Canlog
Similar to candump but instead of displaying the messages they're logged into a .txt file formatted sa follow:  

    [timestamp] [id] [payload]  

Payload is divided into couples of hex values (max 8 couples).  

Example:

    1574758828 699  C8 CE 89 73 DF B6 48 00

The file will be automatically generated with name *canlog_test-* [date] *.txt*

### Compiling  
`gcc -o [name] canlog_test.c`  
### Usage
`./[name] [interface]`  
>## Canplay  
Canplay reads the .txt log file generated from *canlog_test* and send the logged messages through the specified interface with the same time gap they had when logged.
### Compiling
`gcc -o [name] canplay_test.c`  
### Usage  
`./[name] [interface] [input file you want to read]` 
