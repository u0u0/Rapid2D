// Copyright 2016 KeNan Liu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory.h>

#include "RDAudio.h"
#include "RDFileSystem.h"
#include "RDLog.h"
#include "RDAudioOGG.h"

static ALCdevice *s_device = NULL;
static ALCcontext *s_context = NULL;

void RDAudio_init(void)
{
    // Initialization device
    if (!s_device) {
        // select the "preferred device"
        s_device = alcOpenDevice(NULL);
        if (s_device) {
            s_context = alcCreateContext(s_device, NULL);
            alcMakeContextCurrent(s_context);
        }
        // Check for EAX 2.0 support
        ALboolean g_bEAX = alIsExtensionPresent("EAX2.0");
        alGetError(); // clear error code
    }
}

/* load ogg into alBuffer */
ALuint RDAudio_LoadFile(const char *path)
{
    ALuint bufferID = 0;
    alGenBuffers(1, &bufferID);
    if (alGetError() != AL_NO_ERROR) {
        RDLog("Error: RDAudio_LoadFile can't gen OpenAL Buffer");
        return 0;
    }
    
    RD_FileType type = RDFileSystem::getInstance()->getFileType(path);
    if (type <= RD_FILETYPE_MUSIC_START || type >= RD_FILETYPE_MUSIC_END) {
        RDLog("[Error]: RDAudioDecode unknown music file: %s\n", path);
        alDeleteBuffers(1, &bufferID);
        return 0;
    }
    
    RDData *data = RDFileSystem::getInstance()->getData(RD_FILEDIR_RES, path, false);
    if (!data) {
        alDeleteBuffers(1, &bufferID);
        return 0;
    }
    
    RDPcmData pcmData;
    memset(&pcmData, 0, sizeof(RDPcmData));
    
    int rtn = 0;
    switch (type) {
        case RD_FILETYPE_OGG:
            rtn = decodeOgg(data, &pcmData);
            break;
        default:
            break;
    }
    delete data;
    
    // fill OpenAL buffer
    if (rtn >= 0) {
        ALenum format = (pcmData.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        alBufferData(bufferID, format, pcmData.data, pcmData.size, pcmData.rate);
        if (alGetError() != AL_NO_ERROR) {
            RDLog("Error: RDAudio_LoadFile alBufferData Fail");
            alDeleteBuffers(1, &bufferID);
            bufferID = 0;
        }
    }
    
    // free pcm data
    if (pcmData.data) {
        free(pcmData.data);
    }
    
    return bufferID;
}
