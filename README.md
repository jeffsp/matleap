# Matleap: MATLAB Interface to the Leap Motion Controller

This MATLAB mex program will allow you to get data from a Leap Motion
Controller device.

## Building

* Install the Leap SDK
* Create a symbolic link to the SDK called "LeapSDK", and place it in the
  same directory as the mex source file, for example:

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

    >> test_matleap
    matleap version 0.1
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

* Frames are returned with invalid frame data:  **This happens when the
  motion controller driver is not installed.**

* Motion controller has slow framerate:  **The driver will go into standby
  mode if no movement is detected for a long period of time.**
