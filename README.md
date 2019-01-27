# rcswitch433_arduino
sketches and tools for controlling (chinese) rf 433Mhz switches - record, send, etc  
  
original http://www.theforce.dk/rf/
and https://github.com/sui77/rc-switch with https://github.com/ninjablocks/433Utils  
 
 I had to rewrite this code to fit my devices, and to see verbose outputs. (some controllers could not be recorded, i had to see why?)  
   
 On ATmega based boards (UNO, Nano, Mini, Mega), 
 it takes about 100 microseconds (0.0001 s) to read an analog input, 
 so the maximum reading rate is about 10,000 times a second.
   
 There will be alwys noise and misreading, 
 the most times found code is maybe the good one ;)
 

The analogRead method on Arduino Nano is uploaded now to the _433doorbell_original_v2 folder.  

Use codesend from 433Utils/RPi_utils/ to send codes.
