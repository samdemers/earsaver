//
//  ESAppDelegate.h
//  RightVolume
//
//  Created by Samuel Demers on 2014-07-03.
//  Copyright (c) 2014 Samuel Demers. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ESAppDelegate : NSObject <NSApplicationDelegate, NSAlertDelegate>

@property (weak) IBOutlet NSMenu *statusMenu;
- (IBAction)quitButtonPressed:(id)sender;

@end
