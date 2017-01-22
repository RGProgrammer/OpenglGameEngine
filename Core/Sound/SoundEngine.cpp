#include "SoundEngine.h"

TTB::SoundEngine::SoundEngine():m_Device(NULL) ,m_Context(NULL),m_MusicBuffer(0),m_MusicSource(0),m_MusicStatus(SSTOP),
                            m_Repeat(false),m_nbSounds(0),v_Sounds(NULL),v_Sources(NULL){};
TTB::SoundEngine::~SoundEngine(){
    this->Destroy();
};
int TTB::SoundEngine::InitEngine(){

    m_Device=alcOpenDevice(NULL);
    if(!m_Device){
        return 0 ;
    }
    m_Context=alcCreateContext(m_Device,NULL);
    if(!m_Context){
        return 0 ;
    }
    alcMakeContextCurrent(m_Context);
    alcProcessContext(m_Context);
    return 1 ;
};
void TTB::SoundEngine::Destroy(){
    ReleaseBuffers();
    if(m_MusicSource){
        alDeleteSources(1,&m_MusicSource);
        m_MusicSource=0 ;
    }
    if(m_MusicBuffer){
        alDeleteBuffers(1,&m_MusicBuffer);
        m_MusicBuffer=0 ;
    }
    if(m_Context){
        alcDestroyContext(m_Context);
        m_Context=NULL ;
    }
    if(m_Device){
        alcCloseDevice(m_Device);
        m_Device=NULL ;
    }
};
void TTB::SoundEngine::setListener(TTB::BaseActor* Listener){
    Vertex3d  ver1,ver2;
    if(!Listener){
        Vertex3d  ver1,ver2;
        ver1=Listener->getDirection();
        ver2=Listener->getUp();
        ALfloat tab[6]={ver1.x,ver1.y,ver1.z,ver2.x,ver2.y,ver2.z};
        ver1=Listener->getPosition();
        alListener3f(AL_POSITION,ver1.x,ver1.y,ver1.z);
        alListenerfv(AL_ORIENTATION,tab);
    }else{
        ALfloat tab[6]={0.0f,0.0f,-1.0f,0.0f,1.0f,0.0f};
        alListener3f(AL_POSITION,0.0f,0.0f,0.0f);
        alListenerfv(AL_ORIENTATION,tab);
    }
};
ALuint TTB::SoundEngine::LoadSound(Sound sound){
    ALuint IDSound=0,IDSource=0 ;
    alGenBuffers(1,&IDSound);
    if(!IDSound)
        return 0 ;
    alGenSources(1,&IDSource);
    alBufferData(IDSound,sound.Format,sound.Buffer,sound.Size,sound.Frequency);
    if(!IDSource){
        alDeleteBuffers(1,&IDSound);
        return 0 ;
    }
    alSourcei(IDSource,AL_BUFFER,IDSound);
    if(!addID(IDSound,IDSource)){
        alDeleteSources(1,&IDSource);
        alDeleteBuffers(1,&IDSound);
        return 0 ;
    }
    return IDSound;

};
void TTB::SoundEngine::PlaySound(ALuint IDSound,TTB::BaseActor* Source){
    ALuint ID=0;
    for(;ID<m_nbSounds && v_Sounds[ID]!=IDSound;ID++);
    if(ID<m_nbSounds){
        alSourceStop(v_Sources[ID]);
        if(Source){
            Vertex3d ver =Source->getPosition();
            alSource3f(v_Sources[ID],AL_POSITION,ver.x,ver.y,ver.z);
        }else{
            alSource3f(v_Sources[ID],AL_POSITION,0.0f,0.0f,0.0f);
        }
        alSourcePlay(v_Sources[ID]);
    }
};
bool TTB::SoundEngine::LoadMusic(Sound* Music ,bool Repeat){
    if(!m_MusicSource){
        alGenSources(1,&m_MusicSource);
        if(!m_MusicSource)
            return false ;
    }
    alSourceStop(m_MusicSource);
    m_MusicStatus=SSTOP;
    if(Music){
        if(m_MusicBuffer){
            alDeleteBuffers(1,&m_MusicBuffer);
            m_MusicBuffer=0 ;
        }
        alGenBuffers(1,&m_MusicBuffer);
        if(!m_MusicBuffer)
            return false ;

        alBufferData(m_MusicBuffer,Music->Format,Music->Buffer,Music->Size,Music->Frequency);
        alSourcei(m_MusicSource,AL_BUFFER,m_MusicBuffer);
    }
    else
        alSourcei(m_MusicSource,AL_BUFFER,0);
    this->setRepeatMusic(Repeat);
    return true ;
};
void TTB::SoundEngine::setRepeatMusic(bool Repeat){
    if(m_MusicSource){
        alSourcei(m_MusicSource,AL_LOOPING,(Repeat?AL_TRUE:AL_FALSE));
        m_Repeat=Repeat;
    }
};
bool TTB::SoundEngine::isRepeating (){
    return m_Repeat;
};
void TTB::SoundEngine::PlayMusic(){
    if(m_MusicSource)
    if(m_MusicStatus !=SPLAY){
        alSourcePlay(m_MusicSource);
        m_MusicStatus=SPLAY;
    }
};
void TTB::SoundEngine::PauseMusic(){
    if(m_MusicSource)
    if(m_MusicStatus !=SPAUSE){
        alSourcePause(m_MusicSource);
        m_MusicStatus=SPAUSE;
    }
};
void TTB::SoundEngine::StopMusic(){
    if(m_MusicSource)
    if(m_MusicStatus !=SSTOP){
        alSourceStop(m_MusicSource);
        m_MusicStatus=SSTOP;
    }
};
bool TTB::SoundEngine::isPlaying(){
    if(!m_MusicSource)
        return false;
    if(m_MusicStatus==SPLAY)
        return true ;
    return false ;
    /*ALint tmp ;
    alGetSourcei(m_MusicSource,AL_PLAYING,&tmp);
    if(tmp)
        return true;
    else
        return false ;*/
};
bool TTB::SoundEngine::isPaused(){
    if(!m_MusicSource)
        return false;
    if(m_MusicStatus==SPAUSE)
        return true ;
    return false ;
};
bool TTB::SoundEngine::isStopped(){
    if(!m_MusicSource)
        return false;
    if(m_MusicStatus==SSTOP)
        return true ;
    return false ;
};
void TTB::SoundEngine::ReleaseBuffers(){
    if(v_Sounds){
        for(int i=0; i<m_nbSounds;i++){
            alDeleteBuffers(1,&(v_Sounds[i]));
            alDeleteSources(1,&(v_Sources[i]));
        }
        free(v_Sounds);
        free(v_Sources);
        v_Sources=NULL ;
        v_Sounds=NULL ;
        m_nbSounds=0;
    }
};
bool TTB::SoundEngine::addID(ALuint IDSound,ALuint IDSource){
    ALuint* tmp1=(ALuint*)malloc((m_nbSounds+1)*sizeof(ALuint));//for sounds
    ALuint* tmp2=(ALuint*)malloc((m_nbSounds+1)*sizeof(ALuint));
    if(!tmp2 && !tmp2)
        return false ;
    if(v_Sounds){
        for(int i=0 ; i < m_nbSounds;i++){
            tmp1[i]=v_Sounds[i];
            tmp2[i]=v_Sources[i];}
        free(v_Sounds);
        free(v_Sources);
    }
    tmp1[m_nbSounds]=IDSound ;
    tmp2[m_nbSounds]=IDSource;
    v_Sounds=tmp1;
    v_Sources=tmp2;
    m_nbSounds++ ;
    return true ;

};
