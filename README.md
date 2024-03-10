# Seeb61

A 60% through-hole keyboard using a Pro Micro and an MCP23017 IO expander.

## Why?

Good question... 

I had an extra Pro Micro laying around from a macropad build that I did, and I also had recently worked with the MCP23017 in a separate embedded project. Put the two together and I decided to make a fully through-hole keyboard to have some fun with KiCAD and Fusion 360. It gave me something to do over winter break too. 

## Design Process

Before starting the PCB, I decided to draw up the schematic and create a breadboard circuit with everything I wanted on the board. This also allowed me to make sure my firmware was correct so that when I wouldn't have to wait a week or two for my order to come in to start testing the board. 
I tested the breadboard circuit first using Arduino and wrote a driver for the MCP23017, in order to understand how it worked just in case I needed to debug any issues with QMK. Then I started on the QMK code, in which there were already some examples for the expander which helped greatly. All that needed was some adjusting with the 
JSON, matrix, and config file. 

<img src="https://i.imgur.com/znSnLJn.jpg" width=50% height=50%>

Then came the schematic and layout. (KiCAD)

<img src="https://i.imgur.com/EqQ4xuG.png">

<img src="https://i.imgur.com/oKAIdvO.png">

PCB's ordered from JLCPCB

<img src="https://i.imgur.com/hHp4bYz.jpeg" width=50% height=50%>

Decided to buy a 3D printer and try my hand at designing a simple case

<img src="https://i.imgur.com/Dc0FWZM.png">

# Result

<img src="https://i.imgur.com/VftGtn3.jpeg">

# What I'd do different
There are some annoyances I have with my current design, two being the rotary placement needs to be adjusted slightly to the left to match the keycap spacing, and the other being the silkscreen design I decided to put on there...
Otherwise I was fairly happy with the result, everything worked as it should and I've been using it for the past couple months. I decided to go with gateron milky yellows as switches and used foam inside the 3d printed case which made it pretty good. 
