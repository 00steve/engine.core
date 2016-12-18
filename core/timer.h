#ifndef TIMER_H
#define TIMER_H

#include <windows.h>
#include <GLFW/glfw3.h>
struct reference_point
{
  FILETIME file_time;
  LARGE_INTEGER counter;
};

class Timer {
    private:
        reference_point ref_point;
        LARGE_INTEGER   frequency;
        FILETIME        file_time;
        SYSTEMTIME      system_time;


        /*the time when the program started, or more specifically,
        when the timer class was created. It is the origin time, or
        otime.*/
        double otime;
        /*the current time of the system. This is on the same level
        as the origin time.*/
        double ctime;
        /*the relative system time, since the program started. it is
        the time since the origin time that has passed.*/
        double rtime;
        /*the amount of time since last update*/
        double tslu;

        /*the frames per second that the timer is updating at*/
        int fpsTMP,fps;
        /*keeps track of how much of a second has passed*/
        double fpsTimer;

        double frameTimeGoal;
        double frameTime;

        bool tick;
    public:
        Timer();

        /*get the system time and update the time
        variables that are based on the system time*/
        void Update();


        double StartTime();
        double CurrentTime();
        double TimeStep();
        double* TimeStepRef();
        float TStep();
        int FPS();
        bool Step();
};


#endif
