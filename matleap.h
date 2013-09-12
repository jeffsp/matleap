/// @file matleap.h
/// @brief leap motion controller interface
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-09-12

#ifndef MATLEAP_H
#define MATLEAP_H

#define MAJOR_REVISION 0
#define MINOR_REVISION 1

#include "Leap.h"
#include "mex.h"

namespace matleap
{

struct frame
{
    int64_t id;
    int64_t timestamp;
    Leap::PointableList pointables;
};

class frame_grabber
{
    private:
    Leap::Controller controller;
    frame current_frame;
    public:
    frame_grabber ()
    {
        mexPrintf ("Initializing matleap frame grabber version %d.%d\n", MAJOR_REVISION, MINOR_REVISION);
        // receive frames even when you don't have focus
        controller.setPolicyFlags (Leap::Controller::POLICY_BACKGROUND_FRAMES);
    }
    ~frame_grabber ()
    {
        mexPrintf ("Closing matleap frame grabber\n");
    }
    const frame &get_frame ()
    {
        const Leap::Frame &f = controller.frame ();
        current_frame.id = f.id ();
        mexPrintf ("Got frame %d\n", current_frame.id);
        current_frame.timestamp = f.timestamp ();
        current_frame.pointables = f.pointables ();
    }
};

} // namespace matleap

#endif
