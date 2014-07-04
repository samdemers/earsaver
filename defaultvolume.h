//
//  defaultvolume.h
//  EarSaverOSX
//
//  Created by Samuel Demers on 2014-07-03.
//  Copyright (c) 2014 Samuel Demers. All rights reserved.
//

#ifndef EarSaverOSX_defaultvolume_h
#define EarSaverOSX_defaultvolume_h

void *maintainVolumeOnDefaultDeviceChange(Float32 volume);
void stopMaintainingVolumeOnDefaultDeviceChange(void *ctx);

#endif
