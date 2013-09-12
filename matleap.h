/// @file matleap.h
/// @brief leap motion controller interface
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-09-12

#ifndef MATLEAP_H
#define MATLEAP_H

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
};

#endif
