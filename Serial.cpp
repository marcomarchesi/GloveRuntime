//
//  Serial.cpp
//  GloveApp
//
//  Created by Marco Marchesi on 2/12/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#include "Serial.h"

#include <stdio.h>    // Standard input/output definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>    // File control definitions
#include <errno.h>    // Error number definitions
#include <termios.h>  // POSIX terminal control definitions
#include <string.h>   // String function definitions
#include <iostream>

#define DEVICE_PORT          "/dev/cu.AmpedUp-AMP-SPP"
#define READ_COMMAND    "\x01\x02\x00\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03"
#define TERM_SPEED      "B115200"

#define G_FACTOR        0.00390625
#define GYRO_FACTOR     14.375
#define ACC_X_OFFSET    0
#define ACC_Y_OFFSET    0
#define ACC_Z_OFFSET    38.46
#define GYR_X_OFFSET    0.626
#define GYR_Y_OFFSET    -1.895
#define GYR_Z_OFFSET    0

using namespace std;


struct Serial::packet{
    int16_t header;
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
    int16_t gyr_x;
    int16_t gyr_y;
    int16_t gyr_z;
    int16_t mag_x;
    int16_t mag_y;
    int16_t mag_z;
    int8_t chksum;
};


int Serial::init(){
    
    isConnected = false;
    
    glove = open(DEVICE_PORT, O_RDWR | O_NOCTTY);
    /* Error Handling */
    if ( glove < 0 )
    {
        cout << "Error " << errno << " opening " << DEVICE_PORT << ": " << strerror (errno) << endl;
        return EXIT_FAILURE;
    }
    
    /* *** Configure Port *** */
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    
    /* Error Handling */
    if ( tcgetattr ( glove, &tty ) != 0 )
    {
        cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
        return EXIT_FAILURE;
    }
    
    /* Set Baud Rate */
    cfsetospeed (&tty, B115200);
    cfsetispeed (&tty, B115200);
    
    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;        // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;
    tty.c_cflag     &=  ~CRTSCTS;       // no flow control
    tty.c_lflag     =   0;          // no signaling chars, no echo, no canonical processing
    tty.c_oflag     =   0;                  // no remapping, no delays
    tty.c_cc[VMIN]      =   0;                  // read doesn't block
    tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout
    
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
    tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);// turn off s/w flow ctrl
    tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    tty.c_oflag     &=  ~OPOST;              // make raw
    
    /* Flush Port, then applies attributes */
    tcflush( glove, TCIFLUSH );
    
    if ( tcsetattr ( glove, TCSANOW, &tty ) != 0)
    {
        cout << "Error " << errno << " from tcsetattr" << endl;
    }

    return EXIT_SUCCESS;
};

int Serial::connect(){
    
    /* *** WRITE READ COMMAND *** */
    int n_written = (int)write( glove, READ_COMMAND, sizeof(READ_COMMAND) -1 );
    
    if(!n_written)
    {
        cout << "Error " << errno << " opening " << DEVICE_PORT << ": " << strerror (errno) << endl;
        return EXIT_FAILURE;
    }
    
    isConnected = true;
    
    /* Allocate memory for read buffer */
    char buffer [21];
    memset (&buffer, '\0', sizeof buffer);
    
    int buffer_index = 0;
   
    while (isConnected)
    {
        int n = (int)read( glove, &buffer[buffer_index], sizeof(buffer)-buffer_index);
        buffer_index += n;
//        cout << buffer_index << endl;
        if(buffer_index == 21){
            process_packet((Serial::packet*)buffer);
            buffer_index = 0;
        }
        
        usleep(2000);
        memset (&buffer, '\0', sizeof buffer);
        write( glove, READ_COMMAND, sizeof(READ_COMMAND) -1 );
    }
    
    return EXIT_SUCCESS;
};

int Serial::disconnect(){
    
    isConnected = false;
    return EXIT_SUCCESS;
    
}

void Serial::process_packet(Serial::packet* p) {
    
    float _acc_x = (p->acc_x + ACC_X_OFFSET)*G_FACTOR;
    float _acc_y = (p->acc_y + ACC_Y_OFFSET)*G_FACTOR;
    float _acc_z = (p->acc_z + ACC_Z_OFFSET)*G_FACTOR;
    float _gyr_x = (p->gyr_x)/GYRO_FACTOR - GYR_X_OFFSET;
    float _gyr_y = (p->gyr_y)/GYRO_FACTOR - GYR_Y_OFFSET;
    float _gyr_z = (p->gyr_z)/GYRO_FACTOR - GYR_Z_OFFSET;
    float _mag_x = p->mag_x;
    float _mag_y = p->mag_y;
    float _mag_z = p->mag_z;
    
    cout << _acc_z << endl;

};

