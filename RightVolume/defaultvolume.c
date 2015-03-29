//
//  defaultvolume.c
//  RightVolume
//
//  Created by Samuel Demers on 2014-07-03.
//  Copyright (c) 2014 Samuel Demers. All rights reserved.
//

#include <AudioToolbox/AudioServices.h>
#include <CoreAudio/CoreAudio.h>
#include <CoreServices/CoreServices.h>

typedef struct
{
    AudioDeviceID deviceID;
    Float32       volume;
} CallbackData;

static void setDeviceVolume(AudioObjectID deviceID, Float32 volume)
{
    AudioObjectPropertyAddress prop = {
        kAudioHardwareServiceDeviceProperty_VirtualMasterVolume,
        kAudioDevicePropertyScopeOutput,
        kAudioObjectPropertyElementMaster
    };
    UInt32 dataSize = sizeof(volume);
    AudioObjectSetPropertyData(deviceID, &prop, 0, NULL, dataSize, &volume);
}

static OSStatus onDataSourceChanged(AudioObjectID                       inObjectID,
                                       UInt32                              inNumberAddresses,
                                       const AudioObjectPropertyAddress    inAddresses[],
                                       void*                               inClientData)
{
    CallbackData *data = (CallbackData *)inClientData;
    printf("Setting volume to %f\n", data->volume);
    setDeviceVolume(data->deviceID, data->volume);
    return noErr;
}

static void registerDefaultDeviceListener(CallbackData *data)
{
    AudioObjectPropertyAddress prop = {
        kAudioDevicePropertyJackIsConnected,
        kAudioDevicePropertyScopeOutput,
        kAudioObjectPropertyElementMaster
    };
    
    verify_noerr(AudioObjectAddPropertyListener(data->deviceID, &prop, onDataSourceChanged, data));
}

static void unregisterDefaultDeviceListener(CallbackData *data)
{
    AudioObjectPropertyAddress prop = {
        kAudioDevicePropertyJackIsConnected,
        kAudioDevicePropertyScopeOutput,
        kAudioObjectPropertyElementMaster
    };
    verify_noerr(AudioObjectRemovePropertyListener(data->deviceID, &prop, onDataSourceChanged, data));
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

void *setVolumeOnJackChange(Float32 volume)
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

void stopSettingVolumeOnJackChange(void *ctx)
{
    unregisterDefaultDeviceListener(ctx);
    free(ctx);
}
