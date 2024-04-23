# SmoothWalk (fork)
The following is a fork of the original Smooth Walk project by [Leonardo Molina](https://github.com/leomol).
The upstream repos can be found [here (SmoothWalk)](https://github.com/leomol/SmoothWalk) and [here (Arduino Bridge)](https://github.com/leomol/arduino-bridge/tree/master).
The guide below will walk the user through setting up the treadmills for training and for running VR experiments.

## Setting up treadmill for training (tactile mode)

![tarining](/doc/training.jpeg)

The treadmill can be set to run completely by itself, without the need to be connected to a computer or the VR system.
Typically, treadmills are run in this mode for training purposes or for experiments that rely on tactile cues, where distances and laps are counted in the frame of reference of the treadmill belt itself.
At the end of each lap (detected by the photosensor when the reflective tape at the bottom of the belt crosses the LED), the pinch valve is triggered for reward delivery.
To operate the treadmill in this mode:
1. Download and install [Arduino IDE 2.X.X](https://www.arduino.cc/en/software),
2. Load the project [`standalone`](/standalone/standalone.ino),
3. The sketch will contain many useful parameters at the top, such as pin numbers, reward trigger duration, encoder pulses, etc. You may edit these settings to suite your experiment/setup,
4. Connect the treadmill Arduino to your computer via USB and upload the sketch,
5. Et voilà!

**Note:** The Arduino needs to be supplied +12V in order to function properly. Make sure that the external power supply is connected.

**Tip:** You can either run the treadmill by itself or connect it to a computer. When connected, open **Tools > Serial Monitor** to get a lap counter. This is especially useful during training to keep a track of the animals' performance. You can press the reset button on the Arduino board to reset the counter.

**Important note:** When running experiments, it is important to acquire the encoder channels, photosensor and pinch valve outputs using a DAQ. Please solder these connections onto the existing circuit and send those outputs to the relevant BNC ports. A circuit diagram is provided below.

![circuit](/doc/circuit.svg)

## Setting up treadmill for VR (visual mode)

![vr](/doc/vr.jpeg)

Alternatively, the treadmill can be run in Virtual Reality mode, where the spatial frame-of-reference is centred around a virtual environment displayed on screens in front of the animal.
This mode affords not only the ability to present visual scenes, but also to design more complex tasks.
Getting this to work involves a couple more steps, highlighted below:

### Exporting Unity game
The VR game, SmoothWalk, was created using the Unity game engine.
You will need Unity in order to make changes to the environment, edit the behavioural task, compile the game for Android tablets or PC, etc.
1. Download and install [Unity Hub](https://unity.com/unity-hub),
2. Add the folder [`Unity3D`](/Unity3D/) as an existing project into Unity Hub,
3. Once added, Unity Hub will automatically identify the version of Unity Editor that needs to be installed. Follow the instructions to install the correct version,
4. You should now be able to open the project and make edits,
5. Once ready, you can build the project for either Android or Windows by evoking the menu under **File > Build Settings**. Choose the right platform and hit **build**,
6. Flash SmoothWalk onto all three Android tablets and build a Windows version for the data acquisition computer.

**Tip:** You can directly flash the App onto an Android tablet from Unity.
To do so, enable developer mode and USB debugging on the device and plug device into the computer via USB.
In the build menu, you should now see your device in the drop down list.
Choose the target device and hit **Build And Run**.

### Compiling sketch for Arduino
The Arduino needs to be properly interfaced with SmoothWalk in order to communicate and update the animal's position relative to the treadmill.
This is done by uploading the project under the folder [`Bridge`](/Bridge/) to the treadmil's Arduino using Arduino IDE in the same way as earlier described.

### Establishing connections
You should now have the following setup:
* Three tablets running SmoothWalk,
* A main computer also running SmoothWalk compiled for Windows,
* Treadmill with an Arduino running Bridge and connected to the computer via USB.

First, make sure that all the tablets and the computer are connected to the same local network.
The computer is going to detect changes in the animal's movements and send updates of the current position via the Wifi network to the tablets.
When you open SmoothWalk on the computer, you should see the message
<pre>
  <b>Serial port</b>
  Connected.
</pre>
in the console on the left.
If you do not see this message, make sure that the Arduino is connected to the computer via USB and restart SmoothWalk.
It can take one or two tries before the serial connection is established.

![main menu](/doc/main_menu.png)

Under **Device Settings**, the computer should be set as **Control** (server), while the tablets should be set as **Monitor** (client).
Make a note of the **Device ID** of each tablet.
On the computer, press the **Configure** button under **Device Settings** and input the **Device ID** of each tablet separated by carriage return (on separate lines).
Now, when you hit **ESC** to close the main menu and use the arrow keys to move the player, you should see the scene on the tablets getting updated simultaneously (on Android, the menu is toggled using the *back* button on the phone).
If this is not the case, check your Wifi connections, make sure that all devices are on the same local network, check if your firewall is blocking certain ports, etc.
Move the treadmill belt and make sure that the scene gets updated with the rotation of the wheel.
If this is not the case, make sure that serial connection is indeed established with the Arduino.
Any changes to the serial connection, including if it gets disconnected, will be posted on the console to the left.

![monitor](/doc/monitors.png)

**Note:** If you experience issues uploading the Arduino code or establishing a connection between SmoothWalk and the Arduino, then it could be that the COM ports are not properly set up.
On some computers, such as the 2photon imaging computer, many COM ports would be open simultaneously to control the various components of the microscope.
You can check this in **Device Manager**, under **Ports (COM & LPT)**.
Make sure that the Arduino's port number is not conflicting with an existing COM port used by another device.
If this is the case, right click on the Arduino device and open **Properties**, go into **Port Settings > Advanced > COM Port Number** and change the port number to one that is not used.

### Running an experiment

![vr2](/doc/vr2.jpeg)

We are now ready to run VR experiments.
Below, I will highlight a typical workflow and the things to watch out for in an experiment:
1. Make sure that the Y gain is properly set. A little bit of discrepancy is okay, but we want to make sure that distance travelled under VR is as close to the distance travelled on the treadmill as possible (unless that is your intention). I would typically do the math to get the expected gain value (see tip below), then perform some fine adjustments by moving the treadmill,
2. Make sure that the view angle is set properly for each tablet. From left to right, the three tablets should be set to display *West*, *North* and *East*,
3. Make sure that the reward is triggered at the end of each lap and that there is enough reward in the syringe. Sometimes, the pinch valve might get stuck or might leak. You can adjust the tension spring by turning the screw at the bottom,
4. Make sure that all signals are being captured by the DAQ, if you're using a DAQ,
5. Make sure that the frame pulses from the microscope are being captured by SmoothWalk (see note below). You can check this by previewing a scan on the microscope (which should send frame pulses) and checking the CSV file created by SmoothWalk. There should be a parameter `counts` capturing the timestamps of each frame,
6. Head-fix the animal and have fun!
7. If everything worked correctly, you will have access to all the behavioural parameters, synched to the imaging frames, inside a CSV file. The CSV will be exported into **Documents\SmoothWalk** after you hit the **Exit** button. If you want to create separate CSV files for each session, make sure to exit SmoothWalk after each recording.

**Tip:** The VR gain can be adjusted through the slider, but you might find this method to be too inaccurate.
If you want to punch in an exact number, you can do say by editing the registry in which the values are saved.
Hit the *windows* key then type "regedit".
Under **HKEY_CURRENT_USER\Software\Interfaser\SmoothWalk**, you will find a large list of settings that you can tweak.
For instance, **yGain** can be set to an exact value here by editing the HEX code (use a decimal to HEX converter).

![regedit](/doc/regedit.png)

**Important note:** Pins 18 through 21 are used for binary inputs, such are lick sensors or camera pulses.
These inputs will be saved in the CSV file under the parameter name `counts`.
Make sure to solder the proper connections needed for your experiments.
