This folder contains the files print.h, print.c and code.c
print.c converts ints into chars and print.h is simply its header file. The only function you need from print.c is printformat().
printformat() works in the same way as printf() ie it takes a string as the first argument with %i, %d etc for each variable you want to print. Each subsequent argument is a number you want to print.
it will then print the specified string to the serial line.
for example printformat("temperature: %i", temp); 
this would print "temperature: temp" to the serial line except temp would be replaced with the number temp is assigned to.

code.c is a file that reads analogue value from pin 1.5 and turns the green led on if the value is above 511 and turns the red led on if the value is below 511. It also uses printformat() to print the value of pin 1.5 to the serial line.
