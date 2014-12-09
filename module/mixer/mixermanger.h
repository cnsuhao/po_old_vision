#ifndef MIXERMANGER_H_INCLUDED
#define MIXERMANGER_H_INCLUDED
#include <SDL2/SDL_mixer.h>
#include "../../plugin.h"
#include "../../templatemetaprogramming/idallocate.h"
#include <set>
namespace PO
{
    const int MixEmptyID=0;
    const int MixChannelID=-1;

    class MusicData
    {
        Mix_Music* MM;
    public:
        MusicData(const std::string& Adress) throw (MusicUnavailable){ MM=Mix_LoadMUS(Adress.c_str());if(MM==nullptr) throw MusicUnavailable();    }
        ~MusicData(){Mix_FreeMusic(MM);}
        operator Mix_Music* (){return MM;}
    };

    class SoundData
    {
        Mix_Chunk* MC;
    public:
        SoundData(const std::string& Adress) throw (MusicUnavailable){MC=Mix_LoadWAV(Adress.c_str());if(MC==nullptr) throw MusicUnavailable();}
        ~SoundData(){Mix_FreeChunk(MC);}
        operator Mix_Chunk* (){return MC;}
    };

    class MixerManager
    {
        static unsigned int InitCount;
    public:
        MixerManager();
        ~MixerManager();
    };

    class MusicIndex:public IDAllocator<MusicIndex,MixEmptyID>::ID
    {
        static std::map<std::string,std::weak_ptr<MusicData> > MusicMap;
        static int NowMusicID;
        static std::shared_ptr<MusicData> NowPlaying;
        std::shared_ptr<MusicData> Data;
    public:
        void CreateMusic(const std::string& L)
        {
            auto Yi=MusicMap.find(L);
            if(Yi!=MusicMap.end()&&!Yi->second.expired())
            {
                Data=Yi->second.lock();
            }else{
                std::shared_ptr<MusicData> Tem(new MusicData(L));
                std::weak_ptr<MusicData> TC(Tem);
                MusicMap[L]=TC;
                Data=Tem;
            }
        }
        void Play(int L=1)
        {
            NowMusicID=GetID();
            NowPlaying=Data;
            Mix_PlayMusic(*Data,L);
        }
        bool Hold() const
        {
            if(!Mix_PlayingMusic())
                NowMusicID=MixEmptyID;
            return SameID(NowMusicID);
        }
        bool IsPlaying()const   { return Hold()&&Mix_PlayingMusic();}
        bool IsPause()  const   { return Hold()&&Mix_PausedMusic();}
        void Pause()            { if(IsPlaying()) Mix_PauseMusic();}
        void Resume()           { if(IsPause())   Mix_ResumeMusic();}
        void Rewind()           { if(IsPlaying()) Mix_RewindMusic();}
        void Free()             { if(IsPlaying()) {Mix_PauseMusic();NowMusicID=MixEmptyID;Data.reset();}}
        ~MusicIndex(){Pause();}
    };

    class SoundIndex
    {
        static std::map<std::string,std::weak_ptr<SoundData> > SoundMap;
        std::shared_ptr<SoundData> Data;
        friend class ChannelIndex;
    public:
        void CreateSound(const std::string& L)
        {
            auto Yi=SoundMap.find(L);
            if(Yi!=SoundMap.end()&&!Yi->second.expired())
            {
                auto I=Yi->second.lock();
                Data=I;
            }else{
                std::shared_ptr<SoundData> Tem(new SoundData(L));
                std::weak_ptr<SoundData> TC(Tem);
                SoundMap[L]=TC;
                Data=Tem;
            }
        }
        void Free(){Data.reset();}
    };

    class ChannelIndex:public IDAllocator<ChannelIndex,MixChannelID>::ID
    {
        static int MaxChannel;
        std::shared_ptr<SoundData> SD;
    public:
        void Pause()                { Mix_Pause(GetID());}
        void Resume()               { Mix_Resume(GetID());}
        bool IsPlaying() const      { return Mix_Playing(GetID());}
        bool IsPause()   const      { return Mix_Paused(GetID());}
        void SetLeftVolume(Uint8 X) { Mix_SetPanning(GetID(),X,255-X);}
        void SetRightVolume(Uint8 X){ Mix_SetPanning(GetID(),X,255-X);}
        void SetVolume(Uint8 L,Uint8 R){  Mix_SetPanning(GetID(),L,R);}
        void SetVolume(Uint8 X){  Mix_Volume(GetID(),X);}
        void Play(const SoundIndex& SI,int L=1){Mix_PlayChannel(GetID(),*SI.Data,L);SD=SI.Data;}
        ChannelIndex()
        {
            if(MaxChannel!=IDAllocator<ChannelIndex,MixChannelID>::GetMaxID())
            {
                MaxChannel=IDAllocator<ChannelIndex,MixChannelID>::GetMaxID();
                Mix_AllocateChannels(MaxChannel+1);
            }
        }
        ~ChannelIndex()
        {
            if(GetID()==IDAllocator<ChannelIndex,MixChannelID>::GetMaxID()-1)
            {
                MaxChannel--;
                Mix_AllocateChannels(MaxChannel+1);
            }
        }
    };

    typedef MixerManager MixerMode;

}


#endif // MIXERMANGER_H_INCLUDED
