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
        /* METHODS */
        /* serial setup */
        int init();
        /* connect to acquire glove data */
        int connect();
        /* disconnect glove streaming */
        int disconnect();
    
    private:
        /* VARIABLES */
        /* 18 bytes glove data structure */
        struct packet;
        /* glove serial port handler */
        int glove;
        /* connection status */
        bool isConnected;
        /* METHODS */
        /* get complete packet of glove data */
        void process_packet(Serial::packet* p);
};

#endif /* defined(__GloveApp__Serial__) */
