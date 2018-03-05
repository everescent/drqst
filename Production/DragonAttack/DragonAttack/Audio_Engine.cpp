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
  :Audio_{ nullptr }, Playlist_{}, Soundlist_{}, Channel_{}, ChannelGroup_{ nullptr }
{
  //Create audio system
  if (FMOD::System_Create(&Audio_) != FMOD_OK)
  {
    throw std::runtime_error("Audio system cannot be created. Exception thrown!");
  }
  //Initialize audio system
  Audio_->init(SoundNum, FMOD_INIT_NORMAL, NULL);
  //Reserve memory for vectors
  Playlist_.reserve(SoundNum);
  Soundlist_.reserve(SoundNum);
  Channel_.reserve(SoundNum);
  //Initialize the vectors
  for (unsigned i = 0; i < SoundNum; ++i)
  {
    Soundlist_.push_back(nullptr);
    Channel_.push_back(nullptr);
  }
  //Receive the filenames from user
  Init(Playlist_);
  //Creates the sounds from given filenames
  for (unsigned i = 0; i < SoundNum; ++i)
  {
    FMOD_RESULT soundResult;
    soundResult = Audio_->createStream(Playlist_[i].c_str(), FMOD_LOOP_OFF, NULL, &Soundlist_[i]);
    if (soundResult != FMOD_OK)
    {
      throw std::runtime_error("Sound cannot be created. Exception thrown!");
    }
  }
  FMOD_RESULT ChannelResult;
  //Create the channel group
  ChannelResult = Audio_->createChannelGroup(NULL, &ChannelGroup_);
  if (ChannelResult != FMOD_OK)
  {
    throw std::runtime_error("Channel cannot be created. Exception thrown!");
  }
  //Assign the channels to the channel group
  for (unsigned i = 0; i < SoundNum; ++i)
  {
    Channel_[i]->setChannelGroup(ChannelGroup_);
    Channel_[i]->setPaused(false);
  }
}

void Audio_Engine::Play(const int SongNum)
{
  FMOD_RESULT result;
  //Play the sound, from given song number
  result = Audio_->playSound(Soundlist_[SongNum], ChannelGroup_, false, &Channel_[SongNum]);
  if (result != FMOD_OK)
  {
    throw std::runtime_error("Sound cannot be played. Exception thrown!");
  }
}

void Audio_Engine::SetVolume(const int SongNum, const float Volume)
{
  FMOD_RESULT result;
  //Set volume for given song number
  result = Channel_[SongNum]->setVolume(Volume);
  if (result != FMOD_OK)
  {
    throw std::runtime_error("Volume cannot be set. Exception thrown!");
  }
}

void Audio_Engine::SetLoop(const int SongNum, FMOD_MODE Loop)
{
  FMOD_RESULT result;
  //Activate loop, fro given song number
  result = Channel_[SongNum]->setMode(Loop);
  if (result != FMOD_OK)
  {
    throw std::runtime_error("Loop cannot be set. Exception thrown!");
  }
}

void Audio_Engine::SetPause(const int SongNum, const bool Pause)
{
  FMOD_RESULT result;
  //Pause song
  result = Channel_[SongNum]->setPaused(Pause);
  if (result != FMOD_OK)
  {
    throw std::runtime_error("Sound cannot be paused. Exception thrown!");
  }
}

void Audio_Engine::Update()
{
  FMOD_RESULT result;
  //Updates the audio system
  result = Audio_->update();
  if (result != FMOD_OK)
  {
    throw std::runtime_error("Audio system cannot be updated. Exception thrown!");
  }
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
}
