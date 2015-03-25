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
        return 0;
    };
}

#endif /* defined(__GloveRuntime__Math__) */
