//
//  ESAppDelegate.m
//  RightVolume
//
//  Created by Samuel Demers on 2014-07-03.
//  Copyright (c) 2014 Samuel Demers. All rights reserved.
//

#import "ESAppDelegate.h"
#import "defaultvolume.h"

@interface ESAppDelegate()

@property (nonatomic) NSStatusItem *statusItem;

@end

@implementation ESAppDelegate
{
    void *_ctx;
}

- (void)awakeFromNib
{
    self.statusItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength];
    self.statusItem.menu = self.statusMenu;
    self.statusItem.title = @"ES";
    self.statusItem.highlightMode = YES;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    _ctx = setVolumeOnDefaultDeviceChange(0.1f);
}

- (IBAction)quitButtonPressed:(id)sender
{
    if (_ctx)
    {
        stopSettingVolumeOnDefaultDeviceChange(_ctx);
    }
    [NSApp terminate:self];
}

@end
