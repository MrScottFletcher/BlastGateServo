# BlastGateServo with PWM Shield
Blast gate servo controller for Arduino

See 3d printed parts at : https://www.thingiverse.com/thing:3301904

 *   Summry of Forked project:  
 *   Allows multiple push buttons to switch between different blast gates, with only a single one open at a time.
 *   Currently open blast gate is indicated by an LED.
 *   
 *   Requires:  A servo and one button for each blast gate, 
 *   Optional: An LED for each blast gate
 *   Optional: AC currents sensors from http://moderndevice.com/current
 *   =====================================================================
 *   Original Created 2019-01-02 - Greg Pringle
 *   Original Updated 2019-01-20 - Greg Pringle - Added AC sensor support
 *   FORKED 2021-02-02 from https://github.com/Tailslide/BlastGateServo - to MrScottFletcher
 *   =====================================================================
 *  2021-02-02 Updates:
 *  Now uses one button for each gate, using ISR interrupts.
 *  and that requires an ATMEGA board to have enough pins with interrupts.
 *  Also now uses a PWM shield for the motors rather than direct from the Arduino pins.
 *  
