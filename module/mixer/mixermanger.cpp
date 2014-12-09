#include "mixermanger.h"
#include "../../error/poerror.h"
using namespace std;
using namespace PO;
unsigned int MixerManager::InitCount=0;
std::map<std::string,std::weak_ptr<MusicData> > MusicIndex::MusicMap;
std::map<std::string,std::weak_ptr<SoundData> > SoundIndex::SoundMap;
int MusicIndex::NowMusicID=MixEmptyID;
std::shared_ptr<MusicData> MusicIndex::NowPlaying;
int ChannelIndex::MaxChannel;
MixerManager::MixerManager()
{
    if(InitCount==0)
    {
        Mix_Init(~0);
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)!=0) throw EnvironmentalError();
        ++InitCount;
    }
}
MixerManager::~MixerManager()
{
    --InitCount;
    if(InitCount==0)
    {
        Mix_CloseAudio();
        Mix_Quit();
    }
}
/*std::shared_ptr<MusicData>  MixerManager::CreateMusic(const std::string& L)
{
    auto Yi=MusicMap.find(L);
    if(Yi!=MusicMap.end()&&!Yi->second.expired())
    {
        return Yi->second.lock();
    }else{
        std::shared_ptr<MusicData> Tem(new MusicData(L));
        std::weak_ptr<MusicData> TC(Tem);
        MusicMap[L]=TC;
        return Tem;
    }
}
std::shared_ptr<SoundData>  MixerManager::CreateSound(const std::string& L)
{
    auto Yi=SoundMap.find(L);
    if(Yi!=SoundMap.end()&&!Yi->second.expired())
    {
        return Yi->second.lock();
    }else{
        std::shared_ptr<SoundData> Tem(new SoundData(L));
        std::weak_ptr<SoundData> TC(Tem);
        SoundMap[L]=TC;
        return Tem;
    }
}
void MixerManager::PlayMusic(IDAllocator<MusicIndex>::IDType ID, std::shared_ptr<MusicData>& L,int Loops)
{
    NowMusicID=ID;
    NowPlaying=L;
    Mix_PlayMusic(*L,Loops);
}

MusicIndex::~MusicIndex()
{
    if(Data&&IsPlaying())
    {
        Mix_PauseMusic();
    }
}

bool MixerManager::IsPlayingMusic()
{
    if(Mix_PlayingMusic())
        return true;
    NowMusicID=MixEmptyID;
    return false;
}

/*void MusicIndex::Pause()
{

}*/
