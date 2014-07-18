//
//  defaultvolume.h
//  EarSaverOSX
//
//  Created by Samuel Demers on 2014-07-03.
//  Copyright (c) 2014 Samuel Demers. All rights reserved.
//

#ifndef EarSaverOSX_defaultvolume_h
#define EarSaverOSX_defaultvolume_h

/**
 * Starting monitoring default audio device changes (which happen, for example, when earphones or speakers are
 * connected or disconnected). Reset the audio volume to the one specified whenever that happens.
 */
void *setVolumeOnDefaultDeviceChange(Float32 volume);

/**
 * Cancel maintainVolumeOnDefaultDeviceChange() using the pointer it returned.
 */
void stopSettingVolumeOnDefaultDeviceChange(void *ctx);

#endif
