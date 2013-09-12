## Matleap: Leap Motion Controller Interface to MATLAB

This MATLAB mex program will allow you to get data from a Leap Motion Controller device.

# Building

    >> build

# Testing

	>> test_matleap

# Troubleshooting

* make sure you have the Leap SDK installed
* create a symbolic link to the SDK called "LeapSDK", and place it in the same directory as the mex source file, for example:
    ln -s ~/Software/Leap_Developer_Kit/LeapSDK/
