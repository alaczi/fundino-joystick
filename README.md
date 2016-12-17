# FUNDINO JOYSTICK Shield data reading

This example is to show how to read the joystick and button event data from the Fundino Joystick Shield v1.A.

## Challenges

While keyboards on PC recognises keydown/keyup events and sends the code of the pressed buttons, we have no such options with this shield.
The keys must be monitored, reading the connected pins status regularly and saving the data.
Later in the main loop, the data can be analysed or forwarded to your remote controlled device (RC car etc).

## Dependencies

It uses [TimerOne](https://github.com/PaulStoffregen/TimerOne) for software interrupts.


