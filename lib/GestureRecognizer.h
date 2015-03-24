//
//  GestureRecognizer.h
//  GloveRuntime
//
//  Created by Marco Marchesi on 3/17/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#ifndef __GloveRuntime__GestureRecognizer__
#define __GloveRuntime__GestureRecognizer__

#include <stdio.h>
#include <GRT/GRT.h>

using namespace GRT;

class GestureRecognizer{
    public:
        //Create a new DTW instance, using the default parameters
        DTW dtw;
        int init();
        TimeSeriesClassificationData generate_random_set(int dimension,int classes);
        int info();
        int train();
        int classify(TimeSeriesClassificationData gloveData);
};

#endif /* defined(__GloveRuntime__GestureRecognizer__) */
