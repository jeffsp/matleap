/// @file matleap.cpp
/// @brief leap motion controller interface
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-09-12

#include "matleap.h"

// Global instance
matleap::frame_grabber fg;

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // check outputs
    if (nlhs != 0 && nlhs != 1)
        mexErrMsgTxt ("Too many outputs specified");

    // check inputs
    if (nrhs != 0)
        mexErrMsgTxt ("Too many input arguments");

    // only get a frame if an output was specified
    if (nlhs == 0)
        return;

    // get the frame
    const matleap::frame &f = fg.get_frame ();

    // create matlab frame struct
    const char *frame_field_names[] =
    {
        "id",
        "timestamp",
        "pointables"
    };
    // create a frame struct
    int frame_fields = sizeof (frame_field_names) / sizeof (*frame_field_names);
    mexPrintf ("Creating %d frame fields\n", frame_fields);
    plhs[0] = mxCreateStructMatrix (1, 1, frame_fields, frame_field_names);
    // fill the frame struct
    //mxSetFieldByNumber (plhs[0], 0, 0, mxCreateDoubleScalar (f.id));
    mxSetFieldByNumber (plhs[0], 0, 0, mxCreateDoubleScalar (100));
    //mexPrintf ("setting timestamp\n");
    //mxSetFieldByNumber (plhs[0], 0, 1, mxCreateDoubleScalar (f.timestamp));
    mxSetFieldByNumber (plhs[0], 0, 1, mxCreateDoubleScalar (200));
    /*
    if (f.pointables.count () > 0)
    {
        const char *pointable_field_names[] =
        {
            "id",
            "position",
            "velocity",
            "direction"
        };
        int pointable_fields = sizeof (pointable_field_names) / sizeof (*pointable_field_names);
        // create some pointable structs
        mexPrintf ("Creating %d pointable fields\n", pointable_fields);
        mxArray *p = mxCreateStructMatrix (1, f.pointables.count (), pointable_fields, pointable_field_names);
        // fill the frame struct
        mxSetFieldByNumber (plhs[0], 0, 2, p);
        // fill the pointables struct
        for (size_t i = 0; i < f.pointables.size (); ++i)
        {
            // set the id
            mxSetFieldByNumber (p, i, 0, mxCreateDoubleScalar (f.pointables[i].id));
            // create arrays for vectors
            mxArray *pos = mxCreateNumericMatrix (1, 3, mxDOUBLE_CLASS, mxREAL);
            mxArray *vel = mxCreateNumericMatrix (1, 3, mxDOUBLE_CLASS, mxREAL);
            mxArray *dir = mxCreateNumericMatrix (1, 3, mxDOUBLE_CLASS, mxREAL);
            double *destr = mxGetPr (pos);
            destr[0] = f.pointable.pos[i].x;
            destr[1] = f.pointable.pos[i].x;
            destr[2] = f.pointable.pos[i].x;
            // set vectors
            mxSetFieldByNumber (p, i, 1, pos);
            mxSetFieldByNumber (p, i, 2, vel);
            mxSetFieldByNumber (p, i, 3, dir);
        }
    }
    */
}
