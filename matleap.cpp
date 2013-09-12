/// @file matleap.cpp
/// @brief leap motion controller interface
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-09-12

#include "mex.h"
#include "Leap.h"

class frame_counter
{
    private:
    uint64_t frames;
    uint64_t first_ts;
    uint64_t last_ts;
    public:
    frame_counter ()
        : frames (0)
        , first_ts (0)
        , last_ts (0)
    {
    }
    void update (uint64_t ts)
    {
        if (frames == 0)
            first_ts = ts;
        else
            last_ts = ts;
        ++frames;
    }
    uint64_t get_frames () const
    {
        return frames;
    }
    float fps () const
    {
        float secs = static_cast<float> (last_ts - first_ts) / 1000000;
        // don't count the last frame
        if (frames != 0 && secs != 0.0f)
            return (frames - 1) / secs;
        return -1;
    }
};

class matleap : public Leap::Listener
{
    private:
    bool debug;
    frame_counter fc;
    std::string callback;
    void Log (const char *s)
    {
        if (!debug)
            return;
        mexPrintf ("%s\n", s);
    }
    public:
    matleap ()
        : debug (false)
    {
        Log ("Initializing matleap");
    }
    ~matleap ()
    {
        Log ("Closing matleap");
        mexPrintf ("%d fps\n", fc.fps ());
    }
    void set_debug (bool flag)
    {
        debug = flag;
        if (flag)
            Log ("Debug on");
    }
    void set_callback (const std::string &s)
    {
        Log ("Setting callback");
        callback = s;
    }
    virtual void onInit (const Leap::Controller&)
    {
        Log ("onInit");
    }
    virtual void onConnect (const Leap::Controller&)
    {
        Log ("onConnect");
    }
    virtual void onDisconnect (const Leap::Controller&)
    {
        Log ("onDisconnect");
    }
    virtual void onFrame(const Leap::Controller& c)
    {
        //Log ("onFrame");
        const Leap::Frame &f = c.frame ();
        uint64_t t = f.timestamp ();
        fc.update (t);
        Leap::PointableList p = f.pointables ();
        //mxArray *x = mxCreateDoubleMatrix (100, 100, mxREAL);
        //mexCallMATLAB (0, NULL, 1, &x, callback.c_str ());
        //mxDestroyArray (x);
    }
} listener;

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
