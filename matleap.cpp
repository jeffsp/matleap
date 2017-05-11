/// @file matleap.cpp
/// @brief leap motion controller interface
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-09-12

#include "matleap.h"

// Under Windows, a Leap::Controller must be allocated after the MEX
// startup code has completed.  Also, a Leap::Controller must be
// deleted in the function specified by mexAtExit after all global
// destructors are called.  If the Leap::Controller is not allocated
// and freed in this way, the MEX function will crash and cause MATLAB
// to hang or close abruptly.  Linux and OS/X don't have these
// constraints, and you can just create a global Leap::Controller
// instance.

// Global instance pointer
matleap::frame_grabber *fg = 0;
int version = 3; // 1: orig, 2: with arm info, 3: with more hand info

// Exit function
void matleap_exit ()
{
    delete fg;
    fg = 0;
}

/// @brief process interface arguments
///
/// @param nlhs matlab mex output interface
/// @param plhs[] matlab mex output interface
/// @param nrhs matlab mex input interface
/// @param prhs[] matlab mex input interface
///
/// @return command number
int get_command (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int command;
    // check inputs
    switch (nrhs)
    {
        case 1:
        command = *mxGetPr (prhs[0]);
        break;
        case 0:
        mexErrMsgTxt ("Not enough input arguments");
        default:
        mexErrMsgTxt ("Too many input arguments");
    }
    // check that inputs agree with command
    switch (command)
    {
        case -1:
        {
            // set debug command requires 0 outputs
            if (nlhs != 0)
                mexErrMsgTxt ("Wrong number of outputs specified");
        }
        break;
        case 0:
        {
            // get version command requires 1 outputs
            if (nlhs != 0 && nlhs != 1)
                mexErrMsgTxt ("Wrong number of outputs specified");
        }
        break;
        case 1:
        {
            // frame grab command only requires one input
            if (nrhs > 1)
                mexErrMsgTxt ("Too many inputs specified");
            // frame grab command requires exactly one output
            if (nlhs != 0 && nlhs != 1)
                mexErrMsgTxt ("Wrong number of outputs specified");
        }
        break;
        default:
        mexErrMsgTxt ("An unknown command was specified");
    }
    return command;
}

/// @brief helper function
///
/// @param v values to fill array with
///
/// @return created and filled array
mxArray *create_and_fill (const Leap::Vector &v)
{
    mxArray *p = mxCreateNumericMatrix (1, 3, mxDOUBLE_CLASS, mxREAL);
    *(mxGetPr (p) + 0) = v.x;
    *(mxGetPr (p) + 1) = v.y;
    *(mxGetPr (p) + 2) = v.z;
    return p;
}

/// @brief helper function
///
/// @param m Leap Matrix object to fill array with
///
/// @return created and filled 3x3 array of real doubles
mxArray *create_and_fill (const Leap::Matrix &m)
{
    mxArray *p = mxCreateNumericMatrix (3, 3, mxDOUBLE_CLASS, mxREAL);

    const Leap::Vector& x = m.xBasis;
    const Leap::Vector& y = m.yBasis;
    const Leap::Vector& z = m.zBasis;

    *(mxGetPr (p) + 0) = x.x;
    *(mxGetPr (p) + 1) = x.y;
    *(mxGetPr (p) + 2) = x.z;

    *(mxGetPr (p) + 3) = y.x;
    *(mxGetPr (p) + 4) = y.y;
    *(mxGetPr (p) + 5) = y.z;

    *(mxGetPr (p) + 6) = z.x;
    *(mxGetPr (p) + 7) = z.y;
    *(mxGetPr (p) + 8) = z.z;
    return p;
}
/// @brief get a frame from the leap controller
///
/// @param nlhs matlab mex output interface
/// @param plhs[] matlab mex output interface
void get_frame (int nlhs, mxArray *plhs[])
{
    // get the frame
    const matleap::frame &f = fg->get_frame ();
    // create matlab frame struct
    const char *frame_field_names[] =
    {
        "id",
        "timestamp",
        "pointables",
        "hands",
        "version"
    };
    int frame_fields = sizeof (frame_field_names) / sizeof (*frame_field_names);
    plhs[0] = mxCreateStructMatrix (1, 1, frame_fields, frame_field_names);
    // fill the frame struct
    mxSetFieldByNumber (plhs[0], 0, 0, mxCreateDoubleScalar (f.id));
    mxSetFieldByNumber (plhs[0], 0, 1, mxCreateDoubleScalar (f.timestamp));
    // create the pointables structs

    if (f.pointables.count () > 0)
    {
        const char *pointable_field_names[] =
        {
            "id",
            "position",
            "velocity",
            "direction",
            "is_extended",
            "is_finger",
            "is_tool",
            "is_valid",
            "length",
            "width",
            "touch_distance",
            "time_visible"
        };
        int pointable_fields = sizeof (pointable_field_names) / sizeof (*pointable_field_names);
        mxArray *p = mxCreateStructMatrix (1, f.pointables.count (), pointable_fields, pointable_field_names);
        mxSetFieldByNumber (plhs[0], 0, 2, p);

        // fill the pointables structs
        for (size_t i = 0; i < f.pointables.count (); ++i)
        {
            // set the id
            mxSetFieldByNumber (p, i, 0, mxCreateDoubleScalar (f.pointables[i].id ()));
            // create and fill arrays for vectors
            mxArray *pos = create_and_fill (f.pointables[i].tipPosition ());
            mxArray *vel = create_and_fill (f.pointables[i].tipVelocity ());
            mxArray *dir = create_and_fill (f.pointables[i].direction ());

            // set them in the struct
            mxSetFieldByNumber (p, i, 1, pos);
            mxSetFieldByNumber (p, i, 2, vel);
            mxSetFieldByNumber (p, i, 3, dir);
            mxSetFieldByNumber (p, i, 4, mxCreateDoubleScalar (f.pointables[i].isExtended ()));
            mxSetFieldByNumber (p, i, 5, mxCreateDoubleScalar (f.pointables[i].isFinger ()));
            mxSetFieldByNumber (p, i, 6, mxCreateDoubleScalar (f.pointables[i].isTool ()));
            mxSetFieldByNumber (p, i, 7, mxCreateDoubleScalar (f.pointables[i].isValid ()));
            mxSetFieldByNumber (p, i, 8, mxCreateDoubleScalar (f.pointables[i].length ()));
            mxSetFieldByNumber (p, i, 9, mxCreateDoubleScalar (f.pointables[i].width ()));
            mxSetFieldByNumber (p, i, 10, mxCreateDoubleScalar (f.pointables[i].touchDistance ()));
            mxSetFieldByNumber (p, i, 11, mxCreateDoubleScalar (f.pointables[i].timeVisible ()));
        }

    }

    if (f.hands.count () > 0)
    {
        const char *hand_field_names[] =
        {
            "id", // 0

            "basis", // 1
            "confidence", // 2
            "direction", // 3

            "grab_strength", // 4

            "is_left", // 5
            "is_right", // 6
            "is_valid", // 7

            "palm_normal", // 8
            "palm_position", // 9
            "palm_velocity", // 10
            "palm_width", // 11

            "pinch_strength", // 12

            "sphere_center", // 13
            "sphere_radius", // 14

            "stabilized_palm_position", // 15

            "time_visible", // 16
            "wrist_position", // 17

            "arm_elbowPosition", // 18
            "arm_wristPosition", // 19
            "arm_direction", // 20

            "fingers" // 21
        };

        int hand_fields = sizeof (hand_field_names) / sizeof (*hand_field_names);
        mxArray *p = mxCreateStructMatrix (1, f.hands.count (), hand_fields, hand_field_names);
        mxSetFieldByNumber (plhs[0], 0, 3, p);
        // 3 because hands is the third (fourth) field name in
        // the overall struct we are creating.

        for (int i = 0; i < f.hands.count(); ++i)
        {
            // one by one, get the fields for the hand
            mxSetFieldByNumber (p, i, 0, mxCreateDoubleScalar (f.hands[i].id ()));
            mxSetFieldByNumber (p, i, 1, create_and_fill (f.hands[i].basis ()));
            mxSetFieldByNumber (p, i, 2, mxCreateDoubleScalar (f.hands[i].confidence ()));
            mxSetFieldByNumber (p, i, 3, create_and_fill (f.hands[i].direction ()));
            mxSetFieldByNumber (p, i, 4, mxCreateDoubleScalar (f.hands[i].grabStrength ()));
            mxSetFieldByNumber (p, i, 5, mxCreateDoubleScalar (f.hands[i].isLeft ()));
            mxSetFieldByNumber (p, i, 6, mxCreateDoubleScalar (f.hands[i].isRight ()));
            mxSetFieldByNumber (p, i, 7, mxCreateDoubleScalar (f.hands[i].isValid ()));
            mxSetFieldByNumber (p, i, 8, create_and_fill (f.hands[i].palmNormal ()));
            mxSetFieldByNumber (p, i, 9, create_and_fill (f.hands[i].palmPosition ()));
            mxSetFieldByNumber (p, i, 10, create_and_fill (f.hands[i].palmVelocity ()));
            mxSetFieldByNumber (p, i, 11, mxCreateDoubleScalar (f.hands[i].palmWidth ()));
            mxSetFieldByNumber (p, i, 12, mxCreateDoubleScalar (f.hands[i].pinchStrength ()));
            mxSetFieldByNumber (p, i, 13, create_and_fill (f.hands[i].sphereCenter ()));
            mxSetFieldByNumber (p, i, 14, mxCreateDoubleScalar (f.hands[i].sphereRadius ()));
            mxSetFieldByNumber (p, i, 15, create_and_fill (f.hands[i].stabilizedPalmPosition ()));
            mxSetFieldByNumber (p, i, 16, mxCreateDoubleScalar (f.hands[i].timeVisible ()));
            mxSetFieldByNumber (p, i, 17, create_and_fill (f.hands[i].wristPosition ()));
            // get arm position
            Leap::Arm arm = f.hands[i].arm();
            mxSetFieldByNumber (p, i, 18, create_and_fill (arm.elbowPosition ()));
            mxSetFieldByNumber (p, i, 19, create_and_fill (arm.wristPosition ()));
            mxSetFieldByNumber (p, i, 20, create_and_fill (arm.direction ()));

            // get bones for all fingers
            const char *finger_field_names[] =
            {
                "finger_type", // 0
                "bones" // 1
            };
            int finger_fields = sizeof (finger_field_names) / sizeof (*finger_field_names);
            Leap::FingerList fingers = f.hands[i].fingers();
            mxArray *f = mxCreateStructMatrix (1, 5, finger_fields, finger_field_names);
            mxSetFieldByNumber (p, 0, 21, f);

            int finger_index = 0;
            for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); fl++)
            {
                mxSetFieldByNumber(f, 0, finger_index, mxCreateDoubleScalar((*fl).type())); // finger_type
                const char *bone_field_names[] =
                {
                    "basis",    // 0
                    "center",   // 1
                    "direction",// 2

                    "is_valid", // 3

                    "length",   // 4
                    "width",    // 5
                    "nextJoint",// 6
                    "prevJoint",// 7

                    "type"      // 8
                };
                int bone_fields = sizeof (bone_field_names) / sizeof (*bone_field_names);
                mxArray *bones = mxCreateStructMatrix(1, 4, bone_fields, bone_field_names);
                mxSetFieldByNumber(f, 0, finger_index, bones);

                Leap::Bone bone;
                Leap::Bone::Type boneType;
                for (int bi = 0; bi < 4; bi++)
                {
                    // WATCH OUT: bones can be invalid(?)
                    boneType = static_cast<Leap::Bone::Type>(bi);
                    bone = (*fl).bone(boneType);

                    mxSetFieldByNumber(bones,bi,0,create_and_fill(bone.basis())); // 0
                    mxSetFieldByNumber(bones,bi,1,create_and_fill(bone.center())); // 1
                    mxSetFieldByNumber(bones,bi,2,create_and_fill(bone.direction())); // 2
                    mxSetFieldByNumber(bones,bi,3,mxCreateDoubleScalar(bone.isValid()));
                    mxSetFieldByNumber(bones,bi,4,mxCreateDoubleScalar(bone.length()));
                    mxSetFieldByNumber(bones,bi,5,mxCreateDoubleScalar(bone.width()));
                    mxSetFieldByNumber(bones,bi,5,create_and_fill(bone.nextJoint()));
                    mxSetFieldByNumber(bones,bi,5,create_and_fill(bone.prevJoint()));
                    mxSetFieldByNumber(bones,bi,6,mxCreateDoubleScalar(boneType));

                }
                finger_index = finger_index + 1;
            }
        } // re: for f.hands.count()
    } // re: if f.hands.count() > 0

    mxSetFieldByNumber (plhs[0], 0, 4, mxCreateDoubleScalar (version));
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (!fg)
    {
        fg = new matleap::frame_grabber;
        if (fg == 0)
            mexErrMsgTxt ("Cannot allocate a frame grabber");
        mexAtExit (matleap_exit);
    }
    switch (get_command (nlhs, plhs, nrhs, prhs))
    {
        // turn on debug
        case -1:
            fg->set_debug (true);
            return;
            // get version
        case 0:
            plhs[0] = mxCreateNumericMatrix (1, 2, mxDOUBLE_CLASS, mxREAL);
            *(mxGetPr (plhs[0]) + 0) = MAJOR_REVISION;
            *(mxGetPr (plhs[0]) + 1) = MINOR_REVISION;
            return;
            // get frame
        case 1:
            get_frame (nlhs, plhs);
            return;
        default:
            // this is a logic error
            mexErrMsgTxt ("unknown error: please contact developer");
    }
}
