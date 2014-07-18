//
//  defaultvolume.c
//  EarSaverOSX
//
//  Created by Samuel Demers on 2014-07-03.
//  Copyright (c) 2014 Samuel Demers. All rights reserved.
//


#include <CoreAudio/CoreAudio.h>
#include <CoreServices/CoreServices.h>

typedef struct
{
    AudioDeviceID deviceID;
    Float32       volume;
} CallbackData;

static void setDeviceChannelVolume(AudioDeviceID deviceID, UInt32 channel, Float32 volume)
{
    AudioObjectPropertyAddress prop = {
        kAudioDevicePropertyVolumeScalar,
        kAudioDevicePropertyScopeOutput,
        channel
    };
    
    UInt32 dataSize = sizeof(volume);
    verify_noerr(AudioObjectSetPropertyData(deviceID, &prop, 0, NULL, dataSize, &volume));
}

static void setDeviceVolume(AudioObjectID deviceID, Float32 volume)
{
    // Assume a stereo device
    setDeviceChannelVolume(deviceID, 1, volume);
    setDeviceChannelVolume(deviceID, 2, volume);
}

static OSStatus onDefaultDeviceChanged(AudioObjectID                       inObjectID,
                                       UInt32                              inNumberAddresses,
                                       const AudioObjectPropertyAddress    inAddresses[],
                                       void*                               inClientData)
{
    CallbackData *data = (CallbackData *)inClientData;
    printf("Setting volume to %f\n", data->volume);
    setDeviceVolume(data->deviceID, data->volume);
    return noErr;
}

static void registerDefaultDeviceListener(void *data)
{
    AudioObjectPropertyAddress prop = {
        kAudioHardwarePropertyDefaultOutputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster
    };
    verify_noerr(AudioObjectAddPropertyListener(kAudioObjectSystemObject, &prop, onDefaultDeviceChanged, data));
}

static void unregisterDefaultDeviceListener(void *data)
{
    AudioObjectPropertyAddress prop = {
        kAudioHardwarePropertyDefaultOutputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster
    };
    verify_noerr(AudioObjectRemovePropertyListener(kAudioObjectSystemObject, &prop, onDefaultDeviceChanged, data));
}

static int builtinAudioDeviceID()
{
    AudioObjectPropertyAddress prop = {
        kAudioHardwarePropertyDefaultOutputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster
    };
    
    AudioDeviceID deviceID;
    UInt32 propsize = sizeof(deviceID);
    verify_noerr(AudioObjectGetPropertyData(kAudioObjectSystemObject, &prop, 0, NULL, &propsize, &deviceID));
    
    return deviceID;
}

void *setVolumeOnDefaultDeviceChange(Float32 volume)
{
    AudioDeviceID deviceID = builtinAudioDeviceID();
    if (deviceID == kAudioDeviceUnknown)
    {
        fprintf(stderr, "Could not find built-in audio device.");
        return NULL;
    }
    
    CallbackData *data = (CallbackData *)malloc(sizeof(CallbackData));
    data->deviceID = deviceID;
    data->volume = volume;
    registerDefaultDeviceListener(data);
    
    return data;
}

void stopSettingVolumeOnDefaultDeviceChange(void *ctx)
{
    unregisterDefaultDeviceListener(ctx);
    free(ctx);
}
