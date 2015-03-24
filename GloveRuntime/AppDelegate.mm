//
//  AppDelegate.m
//  GloveRuntime
//
//  Created by Marco Marchesi on 3/16/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#import "AppDelegate.h"
#import "Serial.h"
#import "GestureRecognizer.h"



@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate
@synthesize log;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    
    // 1) Open a serial port to get data from the glove controller
    Serial serialPort;
    serialPort.init();
    int glove = serialPort.connect();
    Serial::glove_packet glove_data;
    
    /* Allocate memory for read buffer */
    char buffer [21];
    memset (&buffer, '\0', sizeof buffer);
    
    int buffer_index = 0;
    
    [log setStringValue:@"connected"];
    
    // 2) on connection
    while (serialPort.isConnected)
    {
        int n = (int)read( glove, &buffer[buffer_index], sizeof(buffer)-buffer_index);
        buffer_index += n;
        if(buffer_index == 21){
            glove_data = serialPort.process_packet((Serial::serial_packet*)buffer);
            buffer_index = 0;
        }
        /* Objective-C part */
        memcpy(&buffer, &glove_data, sizeof(glove_data));
        NSString *dataString = [NSString stringWithFormat:@"%f",glove_data.acc_x];
        NSLog(@"%@",dataString);
        
    // 3) send glove data by web socket
        [socketIO sendMessage:dataString];
        
        usleep(2000);
        memset (&buffer, '\0', sizeof buffer);
        //next read command
        serialPort.sendReadCommand();
    }
    
    /* 4) Create a Gesture Recognizer to process data */
    
//    GestureRecognizer recognizer;
//      recognizer.generate_random_set(6,4);              //generate random training set of 4 classes
//      recognizer.init();
    
    
//    [self connect];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    
}

- (void)connect;
{
    // connect to the socket.io server that is running locally at port 8080
    socketIO = [[SocketIO alloc] initWithDelegate:self];
    [socketIO connectToHost:@"localhost" onPort:8080];
    
}


# pragma mark -
# pragma mark socket.IO-objc delegate methods

- (void) socketIODidConnect:(SocketIO *)socket
{
    NSLog(@"socket.io connected.");
}

- (void) socketIO:(SocketIO *)socket didReceiveEvent:(SocketIOPacket *)packet
{
    NSLog(@"didReceiveEvent()");
    
    // test acknowledge
    SocketIOCallback cb = ^(id argsData) {
        NSDictionary *response = argsData;
        // do something with response
        NSLog(@"ack arrived: %@", response);
        
        // test forced disconnect
        [socketIO disconnectForced];
    };
    [socketIO sendMessage:@"hello back!" withAcknowledge:cb];
    
    // test different event data types
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    [dict setObject:@"test1" forKey:@"key1"];
    [dict setObject:@"test2" forKey:@"key2"];
    [socketIO sendEvent:@"welcome" withData:dict];
    
    [socketIO sendEvent:@"welcome" withData:@"testWithString"];
    
    NSArray *arr = [NSArray arrayWithObjects:@"test1", @"test2", nil];
    [socketIO sendEvent:@"welcome" withData:arr];
}

- (void) socketIO:(SocketIO *)socket onError:(NSError *)error
{
    if ([error code] == SocketIOUnauthorized) {
        NSLog(@"not authorized");
    } else {
        NSLog(@"onError() %@", error);
    }
}


- (void) socketIODidDisconnect:(SocketIO *)socket disconnectedWithError:(NSError *)error
{
    NSLog(@"socket.io disconnected. did error occur? %@", error);
}


@end
