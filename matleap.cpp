/// @file matleap.cpp
/// @brief leap motion controller interface
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-09-12

#include "matleap.h"

// Global instances
matleap listener;
Leap::Controller controller;

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // check outputs
    if (nlhs != 0)
        mexErrMsgTxt ("This function does not return any values");

    // check inputs
    switch (nrhs)
    {
        case 2:
        // set debug flag
        listener.set_debug (static_cast<bool> (*mxGetPr (prhs[1])));
        // fall through to next case
        case 1:
        {
            // make sure it's a function handle
            if (!mxIsClass (prhs[0], "char"))
                mexErrMsgTxt("The first input argument must be a function name");
            // set its name
            std::string s;
            s.resize (256);
            if (mxGetString (prhs[0], &s[0], s.size ()) != 0)
                mexErrMsgTxt("Could not convert function handle to string");
            listener.set_callback (s);
        }
        break;
        case 0:
        {
            const Leap::Frame &f = controller.frame ();
            Leap::PointableList p = f.pointables ();
            mexPrintf ("get frame %d\n", p.count ());
            break;
        }
        default:
        mexErrMsgTxt ("Too many input arguments");
    }

    // receive frames even when you don't have focus
    controller.setPolicyFlags (Leap::Controller::POLICY_BACKGROUND_FRAMES);

    // add our custom listener
    controller.addListener (listener);
}
