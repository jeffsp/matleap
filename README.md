# Matleap: MATLAB Interface to the Leap Motion Controller

This MATLAB mex-file will allow you to get data from a Leap Motion
Controller device.

In order to build the mex-file, you need to have a C++ compiler
installed, and you need to setup Matlab for building mex-files.

For more information on setting up Matlab for building mex-files, see
[the Matlab documentation](http://www.mathworks.com/help/matlab/ref/mex.html).

## Building

* Install the Leap SDK
* Give the matleap build module access the Leap SDK:
  * If you are running **Windows**, copy the LeapSDK directory to the
  directory that contains the matleap code.
  * If you are running **OS/X** or **Linux**, you can either copy the
  directory, or better still, create a symbolic link to the LeapSDK
  directory and place it in the same directory as the matleap code,
  for example:

```
    $ ln -s ~/Software/Leap_Developer_Kit/LeapSDK/ ~/matleap/LeapSDK
```

* Build the mex file from within MATLAB

```
    >> build
```

## Testing

```
    >> test_matleap
```

## Examples
```
    >> matleap_version
    ans =
         0     4
    >> f=matleap_frame
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

    >> test_matleap
    matleap version 0.4
    frame id 309984
    frame timestamp 8767122812
    frame pointables 5
    pointable 1
    id 6
    position  -18.529303 279.245270 -9.088086
    velocity  -19.887272 -16.154881 -2.646765
    direction  0.098189 0.300899 -0.948588
    pointable 2
    id 7
    ...
    pointable 5
    id 10
    position  34.367825 264.370300 41.408348
    velocity  -3.097427 13.046532 9.727820
    direction  0.558435 0.059477 -0.827414
    89 frames
    1.000254 seconds
    88.977400 fps
```

## Troubleshooting

### General

* Frames are returned with invalid frame data:  **This happens when the
  motion controller driver is not installed.**

* Motion controller has slow framerate:  **The driver will go into standby
  mode if no movement is detected for a long period of time.**

### OS/X

* "libLeap.dylib can't load": **The Leap dynamic link library must be made
  available to the mex-file executable either by changing your path,
  creating a symbolic link to the library, or by copying the library to the
  same directory as the mex-file.**  For example:

```
    $ cp '/Applications/Leap Motion.app/Contents/MacOS/libLeap.dylib' path_to_matleap
```

### Windows

* "Invalid MEX-file '...\matleap.mexw64/32': The specified module could not
  be found.": **Leap.dll must be made available to the mex-file executeable,
  either by adding it to your path or copying it to the same directory as
  the mex-file.  The leap DLL is located in the LeapSDK\lib\x86 directory on
  32 bit systems and in the LeapSDK\lib\x64 directory on 64 bit systems.**

### Linux

* "Invalid MEX-file ... libLeap.so: cannot open shared object file: No such
  file or directory":  **The leap shared library must be made available to the
  mex-file executable.**  For example:

```
    # ln -s /usr/lib/Leap/libLeap.so /usr/lib/
```
