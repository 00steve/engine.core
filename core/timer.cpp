#include "timer.h"


Timer::Timer(){
    rtime = ctime = otime = glfwGetTime();
    tslu = 0;
}


void Timer::Update() {

    ctime = glfwGetTime();
    tslu = rtime;
    rtime = ctime - otime;
    tslu = rtime-tslu;
    /*keep track of the frames per second that
    the timer is being updated at*/
    fpsTimer += tslu;
    ++fpsTMP;
    if(fpsTimer > 1) {
        fps = fpsTMP;
        fpsTMP = 0;
        fpsTimer -= 1;
    }

    /*keep track of a tick that allows the user to
    update the engine at a specified interval*/
    frameTime += tslu;
    if(frameTime > frameTimeGoal) {
        frameTime -= frameTimeGoal;
        tick = true;
    }

}



double Timer::StartTime(){return otime;}
double Timer::CurrentTime(){return rtime;}
double Timer::TimeStep(){return tslu;}
double* Timer::TimeStepRef(){ return &tslu; }
float Timer::TStep(){return (float)tslu;}
int Timer::FPS(){return fps;}
bool Timer::Step(){return !(!tick || (tick = false));}

