# Calib-Sled
Arduino-controlled calibration sled for endoscopes (and potentially other things).

--IMPORTANT NOTE--

If you are using ROS Melodic (but not later versions) you will need to download and open https://github.com/ros-drivers/rosserial/archive/refs/tags/0.7.7.zip, and manually move `rosserial_python/src/rosserial_python/SerialClient.py` to `/opt/ros/melodic/lib/python2.7/dist-packages/rosserial_arduino`. This will require sudo permissions.
This is a fix for arduino-based services exiting as soon as they are called: https://github.com/ros-drivers/rosserial/issues/408

#Setup From Cold Start
* Turn on both power supplies. The switch on the top power supply should always be set to "on".
* Turn on the da Vinci camera system, with the switch labeled "camera power" in the translucent box on the instrument cart (underneath the CRT TV).
* Start up the vision computer; select Ubuntu 16.04 on the bootloader screen and log into the Tom account. The password is on the computer right next to this one.
* In the terminal, run `roslaunch hw_camera run_camera.launch`. This will start up a pair of windows with the outputs from the da Vinci cameras.
* Plug in the Arduino board.
* In another tab on the terminal, run `rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0`. This will start the Arduino communications.
* The sled will automatically reset to the 0 positions when the board is powered on.

# Manually moving the calibration sled
Once Arduino communications are set up, you can manually send motion commands to the sled system. This is done by entering the command `rosservice call /motion_command` and pressing TAB twice (in a new terminal). That will produce a line of text that looks like this:
```
tes77@surgical31:~$ rosservice call /motion_command "x: 0.0
y: 0.0
theta: 0.0
name: ''" 
```

Using the arrow keys, you can fill in the 'x', 'y', and 'theta' (actually Z) fields with distances in meters. The sleds will then move to those positions.

To reset the sled to the zero position, run `rosservice call /reset`.

# Running the calibrator

Make sure the sleds are at their 0 positions (all the way up, to the left, and with the endoscope all the way forward, using the /reset command).
Then, run `roslaunch intrinsic_acquisition intrinsic_acquisition.launch path:=/home/tes77/ros_ws/src/Calib-Sled/task_description.yml topic:=/davinci_endo/left/image_raw`. This will run the calibration.

Call /reset after calibration to re-zero the sleds.


## Orthogonality testing

If for some reason you are worried about the orthogonality of the Z axis w.r.t. the X and Y axes, you can perform the following using a Polaris sensor:

* Tape the Polaris marker somewhere on the XY sled.
* ``rostopic echo /polaris_sensor/targets >> first_two.txt``
* Move the sled from 0, 0, 0 to 0.15, 0.15, 0 (or anywhere, really).
* Tape the Polaris marker on the endoscope Z sled.
* ``rostopic echo /polaris_sensor/targets >> last.txt``
* ``grep "x:\|y:\|z:\|w:\|orientation:" last.txt > last_proc.csv``
* ``grep "x:\|y:\|z:\|w:\|orientation:" first_two.txt > first_two_proc.csv``
* Using Gedit, replace ``\n orientation:\n x:`` with ``,``, ``\n y:``, ``\n z:``, ``\n w:``, with ``,``, and `` x: `` with nothing.
* Run the Matlab script orthogonality_test.m

This will return the angle, in radians, between the normal vector of the XY plane and the vector of Z motion. Ideally this should be pi, our value is around 3.1230, off by 0.5%.
