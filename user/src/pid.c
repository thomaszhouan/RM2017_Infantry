/*
    pid.c
    Generic PID controller.
*/

#include "pid.h"
#include "common.h"
#include <string.h>

/*
    Trim val to [-lim, lim]
*/
static float PID_Trim(float val, float lim) {
    if (lim < 0.0f) return val;
    if (val < -lim) val = -lim;
    if (val > lim) val = lim;
    return val;
}

/*
    Restore to initial state
*/
void PID_Reset(PID_Controller *pid) {
    memset(pid->set, 0, sizeof(pid->set));
    memset(pid->real, 0, sizeof(pid->real));
    memset(pid->err, 0, sizeof(pid->err));
    pid->errIntegral = 0.0f;
    pid->output = 0.0f;
}

/*
    Update PID controller and return output
*/
float PID_Update(PID_Controller *pid, float target, float measure) {
    // update error
    pid->set[kNOW] = target;
    pid->real[kNOW] = measure;
    pid->err[kNOW] = target - measure;

    float Pout, Iout, Dout;
    if (pid->mode == kIncremental) {
        Pout = pid->Kp * (pid->err[kNOW] - pid->err[kLAST]);
        Iout = pid->Ki * pid->err[kNOW];
        Dout = pid->Kd * (pid->err[kNOW] - 2*pid->err[kLAST] + pid->err[kLLAST]);

        Iout = PID_Trim(Iout, pid->MAX_Iout);
        pid->output += (Pout + Iout + Dout);
        pid->output = PID_Trim(pid->output, pid->MAX_PIDout);
    }
    else if (pid->mode == kPositional) {
        pid->errIntegral += pid->err[kNOW];

        Pout = pid->Kp * pid->err[kNOW];
        Iout = pid->Ki * pid->errIntegral;
        Dout = pid->Kd * (pid->err[kNOW] - pid->err[kLAST]);

        Iout = PID_Trim(Iout, pid->MAX_Iout);
        pid->output = Pout + Iout + Dout;
        pid->output = PID_Trim(pid->output, pid->MAX_PIDout);
    }

    pid->set[kLLAST] = pid->set[kLAST];
    pid->set[kLAST] = pid->set[kNOW];
    pid->real[kLLAST] = pid->real[kLAST];
    pid->real[kLAST] = pid->real[kNOW];
    pid->err[kLLAST] = pid->err[kLAST];
    pid->err[kLAST] = pid->err[kNOW];

    float ret = pid->output;
    if (ABS(ret) < pid->MIN_PIDout) ret = 0.0f;
    return ret;
}
