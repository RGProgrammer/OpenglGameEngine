#ifndef PFE_SOUNDENGINE_H_
#define PFE_SOUNDENGINE_H_
#include "..//BaseActors//BaseActor.h"
#include "..//..//tools//Openal//al.h"
#include "..//..//tools//Openal//alc.h"
#include <stdlib.h>
#include <stdio.h>

#define SSTOP        0
#define SPAUSE       1
#define SPLAY        2

namespace TTB {

    typedef struct {
        ALenum Format ;
        ALsizei Frequency;
        ALsizei Size;
        _u8b* Buffer;
    } Sound ;

    class SoundEngine {
    public:
        SoundEngine();
        ~SoundEngine();
        int InitEngine();
        void Destroy();
        void setListener(TTB::BaseActor* Listener);
        ALuint LoadSound(Sound sound);
        void PlaySound(ALuint IDSound,TTB::BaseActor* source=NULL);
        _bool LoadMusic(Sound* Music ,_bool repeat=false);
        void setRepeatMusic(_bool Repeat);
        _bool isRepeating ();
        void PlayMusic();
        void PauseMusic();
        void StopMusic();
        _bool isPlaying();
        _bool isPaused();
        _bool isStopped();
        void ReleaseBuffers();//release only the sound buffers and not the music
    private:
        _bool addID(ALuint IDSound,ALuint IDSource );
        ALCdevice*              m_Device ;
        ALCcontext*             m_Context;
        ALuint                  m_MusicBuffer ;
        ALuint                  m_MusicSource;// initialized via set Listener;
        ALuint                  m_MusicStatus ;
        _bool                    m_Repeat ;

        ALuint*                 v_Sounds;//vctor contains all loaded sound buffers
        ALuint*                 v_Sources;
        ALuint                  m_nbSounds;

    };
};
#endif // PFE_SOUNDENGINE_H_
