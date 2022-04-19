# Calib-Sled
Arduino-controlled calibration sled for endoscopes (and potentially other things).

#Setup From Cold Start
* Turn on both power supplies. They should always be set to the +6V button and 6.83ish volts on the digital display.
* Turn on the da Vinci camera system, with the switch labeled "camera power" in the translucent box on the instrument cart (underneath the CRT TV).
* Make sure the Arduino board is plugged in.
* Start up the vision computer; select Ubuntu 16.04 on the bootloader screen and log into the Tom account. The password is on the computer right next to this one.
* In the terminal, run `roslaunch hw_camera run_camera.launch`. This will start up a pair of windows with the outputs from the da Vinci cameras.
* In another tab on the terminal, run `rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0`. This will start the Arduino communications.

# Manually moving the calibration sled
Once Arduino communications are set up, you can manually send motion commands to the sled system. This is done by entering the command `rosservice call /motion_command` and pressing TAB twice. That will produce a line of text that looks like this:
```
tes77@surgical31:~$ rosservice call /motion_command "x: 0.0
y: 0.0
theta: 0.0
name: ''" 
```

Using the arrow keys, you can fill in the 'x', 'y', and 'theta' (actually Z) fields with distances in meters. The sleds will then move to those positions. Note that the board ALWAYS STARTS assuming the position is zero if it is plugged in, REGARDLESS of what the sleds' actual positions are. To make sure they actually ARE at zero, you may want to command them to large negative values at startup, then unplug the board, plug it bakc in, and restart `rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0`.

# Running the calibrator

Make sure the sleds are at their 0 positions (all the way up, to the left, and with the endoscope all the way forward). This can be done by the command line method above, or just by cranking a lot. Then, unplug the Arduino and plug it back in, and restart `rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0`, to reset the board's controller.

Then, run `roslaunch intrinsic_acquisition intrinsic_acquisition.launch path:=/home/tes77/Desktop/Calib-Sled/task_description.yml topic:=/davinci_endo/left/image_raw`. This will run the calibration.

Run
```
tes77@surgical31:~$ rosservice call /motion_command "x: 0.0
y: 0.0
theta: 0.0
name: ''" 
```

after the calibration is done to re-zero the sleds.
