/// @file matleap.h
/// @brief leap motion controller interface
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-09-12

#ifndef MATLEAP_H
#define MATLEAP_H

#define MAJOR_REVISION 0
#define MINOR_REVISION 2

#include "Leap.h"
#include "mex.h"

namespace matleap
{

/// @brief a leap frame
struct frame
{
    int64_t id;
    int64_t timestamp;
    Leap::PointableList pointables;
};

/// @brief leap frame grabber interface
class frame_grabber
{
    private:
    bool debug;
    Leap::Controller controller;
    frame current_frame;
    public:
    /// @brief constructor
    frame_grabber ()
        : debug (false)
    {
        // receive frames even when you don't have focus
        controller.setPolicyFlags (Leap::Controller::POLICY_BACKGROUND_FRAMES);
    }
    /// @brief destructor
    ~frame_grabber ()
    {
        if (debug)
            mexPrintf ("Closing matleap frame grabber\n");
    }
    /// @brief debug member access
    ///
    /// @param flag turn it on/off
    void set_debug (bool flag)
    {
        if (flag)
            mexPrintf ("Setting debug on\n");
        debug = flag;
    }
    /// @brief get a frame from the controller
    ///
    /// @return the frame
    const frame &get_frame ()
    {
        const Leap::Frame &f = controller.frame ();
        current_frame.id = f.id ();
        if (debug)
            mexPrintf ("Got frame with id %d\n", current_frame.id);
        current_frame.timestamp = f.timestamp ();
        current_frame.pointables = f.pointables ();
        return current_frame;
    }
};

} // namespace matleap

#endif
