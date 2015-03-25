//
//  AppDelegate.h
//  GloveRuntime
//
//  Created by Marco Marchesi on 3/16/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SocketIO.h"
#include <assert.h>
#include <pthread.h>


@interface AppDelegate : NSObject <NSApplicationDelegate,SocketIODelegate>{
    SocketIO *socketIO;
    NSDictionary* gloveDataDictionary;
}
@property IBOutlet NSTextField *log;

@end;

