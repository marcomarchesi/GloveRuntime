//
//  Math.h
//  GloveRuntime
//
//  Created by Marco Marchesi on 3/25/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#ifndef __GloveRuntime__Math__
#define __GloveRuntime__Math__

#include <stdio.h>
#include <math.h>
#include "Serial.h"

namespace Math {
    
    static float yaw_start = 0;
    static float yaw_calibration_array [3];
    static int yaw_calibration_counter = 0;
    
    /* functions for getting rotation angles */
    /* ROLL */
    float getRoll(Serial::glove_packet* data){
        return atan(data->acc_y / sqrt(data->acc_x*data->acc_x + data->acc_z*data->acc_z));
    };
    /* PITCH */
    float getPitch(Serial::glove_packet* data){
        return atan(data->acc_x / data->acc_z);
    };
    /* TODO */
    float getYaw(Serial::glove_packet* data){
        if(yaw_start ==0)
            yaw_start = data->mag_x;
        return data->mag_x - yaw_start;
    };
    float degreesToRadians(float degree){
        return degree*(M_PI/180);
    };
    float getHeading(Serial::glove_packet* data){
        float heading = atan2(data->mag_y, data->mag_x);
        
        // Correct for when signs are reversed.
        if(heading < 0) heading += 2*M_PI;
        if(heading > 2*M_PI) heading -= 2*M_PI;
        
//        return degreesToRadians(heading); //radians to degrees
        return heading;
    };
    
}

#endif /* defined(__GloveRuntime__Math__) */
