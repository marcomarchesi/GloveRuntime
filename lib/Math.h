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
//    static float yaw_calibration_array [3];
//    static int yaw_calibration_counter = 0;
    
    /* functions for getting rotation angles */
    /* ROLL */
    float getRoll(Serial::glove_packet* data){
        return atan(data->acc_y / sqrt(data->acc_x*data->acc_x + data->acc_z*data->acc_z));
    };
    /* PITCH */
    float getPitch(Serial::glove_packet* data){
        return atan(data->acc_x / data->acc_z);
    };
    
    
    /* MATH UTILS */
    float degreesToRadians(float degree){
        return degree*(M_PI/180);
    };
    float sign(float value){
        
      if(value > 0)
          return 1.0;
      else{
          return -1.0;
      }
    };
    

    /* get HEADING */
    float getHeading(Serial::glove_packet* data){
        
//        float mag_gx = data->mag_x*cos(getPitch(data)) + data->mag_y*sin(getPitch(data))*sin(getRoll(data)) + data->mag_z*sin(getPitch(data))*cos(getRoll(data));
//        float mag_gy = data->mag_y*cos(getRoll(data)) - data->mag_z*sin(getRoll(data));
//        float heading = atan2(mag_gy, mag_gx);
        float heading = atan2(data->mag_x, -data->mag_y);
        
        // Correct for when signs are reversed.
        if(heading < 0) heading += 2*M_PI;
        if(heading > 2*M_PI) heading -= 2*M_PI;
        return heading;
    };
    float getYaw(Serial::glove_packet* data){
        std::cout << data->acc_x << std::endl;
       float yaw = (getHeading(data) - yaw_start)* cos(getPitch(data));
//        std::cout << yaw << std::endl;
            return yaw;
    };
    float getPush(Serial::glove_packet* data){
        std::cout << sqrt(data->acc_x*data->acc_x + data->acc_y*data->acc_y) << std::endl;
        
        return sqrt(data->acc_x*data->acc_x + data->acc_y*data->acc_y);
    };
    

    void calibrate(Serial::glove_packet* data){
            yaw_start = getHeading(data);
            isCalibrating = false;
    };
    
    
}

#endif /* defined(__GloveRuntime__Math__) */
