# Matleap: Leap Motion Controller Interface to MATLAB

This MATLAB mex program will allow you to get data from a Leap Motion
Controller device.

## Building

* Install the Leap SDK
* Create a symbolic link to the SDK called "LeapSDK", and place it in the
  same directory as the mex source file, for example:

    $ ln -s ~/Software/Leap_Developer_Kit/LeapSDK/ ~/matleap/LeapSDK

* Build the mex file from within MATLAB

    >> build

## Testing

    >> test_matleap

## Examples
    >> matleap(0)
    ans =
         0     1
    >> f=matleap(1)
    f =
                  id: 263725
             timestamp: 8.1430e+09
          pointables: [1x5 struct]
    >> f.pointables(1).position
    ans =
       8.9769  220.3197   -5.8013
    >> f.pointables(1).velocity
    ans =
          -6.7446   -4.2978  -13.0157
    >> f.pointables(1).direction
    ans =
       0.2089    0.3257   -0.9221

## Troubleshooting

* Frames are returned with invalid looking frame data:  **This happens when
  the motion controller driver is not installed.**

* Motion controller has slow framerate:  **The driver will go into standby
  mode if no movement is detected for a long period of time.  This will
  cause the framerate to slow down.**
