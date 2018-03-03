/* Start Header ************************************************************************/
/*!
\file       Audio_Engine.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Audio Engine class member functions defined here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Audio_Engine.h"
/*Guide Links: 
  https://cuboidzone.wordpress.com/2013/07/26/tutorial-implementing-fmod/
  https://katyscode.wordpress.com/2012/10/05/cutting-your-teeth-on-fmod-part-1-build-environment-initialization-and-playing-sounds/
  https://katyscode.wordpress.com/2013/01/15/cutting-your-teeth-on-fmod-part-2-channel-groups/
*/

Audio_Engine::Audio_Engine(unsigned SoundNum, const std::function<void(std::vector<std::string>&)>& Init)
  :Audio_{ nullptr }, Playlist_{}, Soundlist_{}, Channel_{}
{
  if (FMOD::System_Create(&Audio_) != FMOD_OK)
  {
    throw "Audio system cannot be created. Exception thrown!";
  }
  Audio_->init(SoundNum, FMOD_INIT_NORMAL, NULL);
  Playlist_.reserve(SoundNum);
  Soundlist_.reserve(SoundNum);
  Channel_.reserve(SoundNum);
  for (unsigned i = 0; i < SoundNum; ++i)
  {
    Soundlist_.push_back(nullptr);
    Channel_.push_back(nullptr);
  }
  Init(Playlist_);
  for (unsigned i = 0; i < SoundNum; ++i)
  {
    FMOD_RESULT soundResult;
    soundResult = Audio_->createStream(Playlist_[i].c_str(), FMOD_LOOP_OFF, NULL, &Soundlist_[i]);
    if (soundResult != FMOD_OK)
    {
      throw "Sound cannot be created. Exception thrown!";
    }
  }
  FMOD_RESULT ChannelResult;
  ChannelResult = Audio_->createChannelGroup(NULL, &ChannelGroup_);
  if (ChannelResult != FMOD_OK)
  {
    throw "Channel cannot be created. Exception thrown!";
  }
  for (unsigned i = 0; i < SoundNum; ++i)
  {
    Channel_[i]->setChannelGroup(ChannelGroup_);
    Channel_[i]->setPaused(false);
  }
}

{
  Audio_->playSound(Soundlist_[SongNum], ChannelGroup_, false, &Channel_[SongNum]);
}

void Audio_Engine::SetVolume(const int SongNum, const float Volume)
{
  Channel_[SongNum]->setVolume(Volume);
}

void Audio_Engine::SetLoop(const int SongNum, FMOD_MODE Loop)
{
  Channel_[SongNum]->setMode(Loop);
}

void Audio_Engine::SetPause(const int SongNum, const bool Pause)
{
  Channel_[SongNum]->setPaused(Pause);
}

void Audio_Engine::Update()
{
  Audio_->update();
}

Audio_Engine::~Audio_Engine()
{
  for (unsigned i = 0; i < Playlist_.size(); ++i)
  {
    Soundlist_[i]->release();
    ChannelGroup_->release();
  }
  Channel_.clear();
  Playlist_.clear();
  Audio_->release();
