Lab report contains 5 lab tasks those were done by me. Each task demand as follow accordingly:

Task 1
Via the ArduinoIDE use digitalwrite() and blink the onboard LED at 1 Hz. Then use registers to blink the onboard LED at 1 Hz. Estimate and compare the 
execution time and memory allocation. Using both methods, blink the onboard LED as fast as possible; evidence and explain any differences. Recommended 
to use an oscilloscope

Task 2
Create a serial interface for subtracting two numbers using CPU registers only, store the result in a register, and output all register values. You must 
then examine the SREG status register and clearly identify the state of the Zero (Z) and Carry (C) flags after the subtraction.Evidence the SREG behavior 
and explain why these flags are set or cleared for this operation, with particular emphasis on how the AVR represents underflow in unsigned subtraction 
and how the Carry flag behaves during subtraction.

Task 3
Using AVR language, implement a counter that increments a single register by 1 every 1 second. Identify the number of CPU cycles  and instructions that 
that place within this window for your code. Implement a serial command that allows you to store or reset the current counter value in the EEPROM. Unplug 
Arduino. On power-up, the counter should continue from the last stored value rather than starting over (unless reset). Explain any anomalies or limitations 
that may occur. Indicate the address location of the register you are using to increment a value.

Task 4
Build an interrupt-driven system where pressing a button connected to D2 turns an output pin ON for exactly 10 milliseconds and then turns it OFF automatically. 
Configure Timer1 to generate an interrupt every 1 millisecond, and use this interrupt to control a 10 ms countdown (i.e., interrupt would trigger 10 times). 
Configure the external interrupt on D2 so that when the button is pressed, it immediately sets the output pin HIGH and starts the countdown. Each 1 ms timer 
interrupt should reduce the countdown value, and when it reaches zero, the output pin must be set LOW. The main loop must remain empty, and all behaviour must be
handled using direct AVR register configuration without Arduino helper functions. You must calculate how to achieve the 1 ms timer interval, predict the 10 ms 
pulse width, measure the actual pulse using an oscilloscope, and compare the measured result with your calculated expectation.
Detect a button press using an external interrupt.
• Immediately set a chosen output pin HIGH when the button is pressed.
• Keep the output pin HIGH for exactly 10 milliseconds.
• Automatically set the output pin LOW after 10 milliseconds without using delay functions.
• Use a timer interrupt running at 1 ms intervals to control the pulse timing.
• Operate fully through interrupts, with the main loop doing nothing.

Task 5
AVR CONTROL FLOW LAB: RJMP VS JMP VS IJMP ON ARDUINO UNO (ATMEGA328P)
Hardware: Arduino Uno (ATmega328P) | LED: Built-in LED on D13 (PB5) | Button: D8 (PB0) to GND (use internal pull-up)
Objective: Build one firmware that behaves like a simple “one-button LED gadget” and demonstrates three different jump styles:
• RJMP: tight local polling loop at boot
• JMP: hard jump into one of three modes (Mode A/B/C)
• IJMP: runtime dispatch to one of several “actions” inside the selected mode
Final behaviour: On power-up, the device waits for the first button press, then confirms with a short LED flash. During the next 2 seconds, the number of presses 
selects a mode:
• 1 press = Mode A (slow blink)
• 2 presses = Mode B (double blink)
• 3 presses = Mode C (fast strobe)
While running in a mode, each short press cycles an “action” (0..3). The action is executed using a jump table and IJMP.
Phase requirements:
• Phase 1 (RJMP): Implement the boot wait loop in assembly using a label, a bit-test (SBIC or SBIS), and rjmp back to the label while the button is not pressed.
• Phase 2 (JMP): After counting presses, enter the chosen mode by using an explicit jmp modeX_entry. The mode entry functions run forever and do not return.
• Phase 3 (IJMP): Inside the mode loop, implement action dispatch by loading a handler address into Z (r31:r30) and executing ijmp. Each handler must jump back
to the mode loop (since IJMP does not push a return address).

TASK 5A
On power-up, the arduino must do nothing until the user presses the button. Implement the waiting loop as a tiny assembly loop that checks the button bit and uses
RJMP to jump back to the same label while the button is not pressed. When the press happens, the loop falls through and the LED turns on for 200 ms to confirm 
“start detected”.
Deliverable: show the LED does not start until a press, and show rjmp in the disassembly.

TASK 5B
After the first press, the arduino must choose one of three modes based on how many button presses happen within the next 2 seconds:
• 1 press = Mode A (slow blink)
• 2 presses = Mode B (double blink)
• 3 presses = Mode C (fast strobe)
Once the choice is made, the firmware must commit to that mode by using JMP to jump to modeA_entry, modeB_entry, or modeC_entry. Each mode runs forever 
and does not “return” to the chooser.
Deliverable: show you can pick a mode by pressing 1, 2, or 3 times, and show jmp modeX_entry in the disassembly.

TASK 5C
While running inside any mode, the arduino must respond to short button presses by cycling through a small set of actions, for example:
• Action 0: LED pattern normal
• Action 1: LED pattern inverted (on becomes off)
• Action 2: LED brightness simulation (short on-time, long off-time)
• Action 3: reset action state back to 0
Implement this “action selection” using a jump table: the current action index selects a handler address, you load it into Z (r31:r30), and you execute 
IJMP to jump to that handler. Each handler does its small behaviour update, then jumps back to the mode loop (because IJMP is a jump, not a call).
Deliverable: show button presses cycle actions, and show ijmp in the disassembly.
