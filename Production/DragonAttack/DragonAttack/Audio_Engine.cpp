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

//Provide number of songs, and give a function to initialize the list of filenames
Audio_Engine::Audio_Engine(unsigned SoundNum, const Audio_Init& Init)
  :Audio_       { nullptr }, Playlist_{}, 
   Soundlist_   {         }, Channel_ {}, 
   ChannelGroup_{ nullptr }
{
  //Create audio system
  FMOD::System_Create(&Audio_                         );
  //Initialize audio system
  Audio_->init       (SoundNum, FMOD_INIT_NORMAL, NULL);
  //Reserve memory for vectors
  Playlist_ .reserve (SoundNum                        );
  Soundlist_.reserve (SoundNum                        );
  Channel_  .reserve (SoundNum                        );
  //Initialize the vectors
  for (unsigned i = 0; i < SoundNum; ++i)
  {
    Soundlist_.push_back(nullptr);
    Channel_  .push_back(nullptr);
  }
  //Receive the filenames from user
  Init(Playlist_);
  //Creates the sounds from given filenames
  for (unsigned i = 0; i < SoundNum; ++i)
  {
    Audio_->createStream(Playlist_[i].c_str(),
                         FMOD_LOOP_OFF       ,
                         NULL                ,
                         &Soundlist_[i]          );
  }
  //Create the channel group
  Audio_->createChannelGroup(NULL, &ChannelGroup_);
  //Assign the channels to the channel group
  for (unsigned i = 0; i < SoundNum; ++i)
  {
    Channel_[i]->setChannelGroup(ChannelGroup_   );
    Channel_[i]->setPaused      (false           );
  }
}

//Plays the song numnber given
void Audio_Engine::Play(const int SongNum)
{
  //Play the sound, from given song number
  Audio_->playSound(Soundlist_[SongNum], 
                    ChannelGroup_      ,
                    false              ,
                    &Channel_ [SongNum] );
}

//Set volume for the given song number
void Audio_Engine::SetVolume(const int SongNum, const float Volume)
{
  //Set volume for given song number
  Channel_[SongNum]->setVolume(Volume);
}

//Set song to loop, does not loop by default
void Audio_Engine::SetLoop(const int SongNum, FMOD_MODE Loop)
{
  //Activate loop, fro given song number
  Channel_[SongNum]->setMode(Loop);
}

//Pause the given song number
void Audio_Engine::SetPause(const int SongNum, const bool Pause)
{
  //Pause song
  Channel_[SongNum]->setPaused(Pause);
}

//Returns a bool indicating if the song has completed
bool Audio_Engine::GetPlaying(const int SongNum)
{ 
  bool len; 
  //Check if song is playing
  Channel_[SongNum]->isPlaying(&len);
  return len; 
}

//Updates the audio system; CALL THIS!
void Audio_Engine::Update()
{
  //Updates the audio system
  Audio_->update();
}

//Release the audio system
Audio_Engine::~Audio_Engine()
{
  for (unsigned i = 0; i < Playlist_.size(); ++i)
  {
    //Release sound first
    Soundlist_[i]->release();
  }
  //Release and clear the rest of the resources
  ChannelGroup_->release  ();
  Channel_ .clear         ();
  Playlist_.clear         ();
  Audio_->release         ();
}
