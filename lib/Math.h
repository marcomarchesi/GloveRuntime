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
    
    static bool isCalibrating = false;
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
    /* get HEADING */
    float getHeading(Serial::glove_packet* data){
        float heading = atan2(data->mag_y, data->mag_x);
        
        // Correct for when signs are reversed.
        if(heading < 0) heading += 2*M_PI;
        if(heading > 2*M_PI) heading -= 2*M_PI;
        
        //        return degreesToRadians(heading); //radians to degrees
        return heading;
    };
    float average(float *array){
        
        float avg = 0;
        for(int i=0;i<3;++i){
            avg += array[i];
        }
        avg /= 3;
        return avg;
    };
    float calibrate(Serial::glove_packet* data){
        if(yaw_calibration_counter < 3){
            yaw_calibration_array[yaw_calibration_counter] = getHeading(data);
            yaw_calibration_counter++;
        }else{
            yaw_start = average(yaw_calibration_array);
            yaw_calibration_counter = 0;
            isCalibrating = false;
        }
        return yaw_start;
    };
    
    /* TODO */
    float getYaw(Serial::glove_packet* data){
        
//        std::cout << "heading is " << getHeading(data) << " and yaw start is " << yaw_start << std::endl;
        float yaw = getHeading(data) - yaw_start;
            if(yaw>0)
                yaw *= 2;
            return yaw;
    };
    
    float degreesToRadians(float degree){
        return degree*(M_PI/180);
    };
    
    
}

#endif /* defined(__GloveRuntime__Math__) */
