//
//  Serial.h
//  GloveApp
//
//  Created by Marco Marchesi on 2/12/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#ifndef __GloveApp__Serial__
#define __GloveApp__Serial__

#include <stdio.h>
class Serial{
    public:
    int init();
    int connect();
    int disconnect();
    struct packet;
    private:
    int glove;
    bool isConnected;
    void process_packet(Serial::packet* p);
};

#endif /* defined(__GloveApp__Serial__) */
