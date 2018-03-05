/* Start Header ************************************************************************/
/*!
\file       Audio_Engine.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Audio Engine class declared here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "fmod.hpp"
#include "fmod_studio.hpp"
#include <vector>
#include <string>
#include <functional>
#include <iostream>

class Audio_Engine {

public:
  //Provide number of songs, and give a function to initialize the list of filenames
  Audio_Engine(unsigned SoundNum, const std::function<void (std::vector<std::string>&)>& Init);
  //Release the audio system
  ~Audio_Engine();
  //Plays the song numnber given
  void Play(const int SongNum);
  //Set volume for the given song number
  void SetVolume(const int SongNum, const float Volume);
  //Set song to loop, does not loop by default
  void SetLoop(const int SongNum, FMOD_MODE Loop);
  //Pause the given song number
  void SetPause(const int SongNum, const bool Pause);
  //Updates the audio system; CALL THIS!
  void Update();

private:
  FMOD::System *Audio_;                  //The audio system
  std::vector <std::string> Playlist_;   //The filenames for each song
  std::vector <FMOD::Sound*> Soundlist_; //The songs
  std::vector <FMOD::Channel*> Channel_; //The channel for each song
  FMOD::ChannelGroup* ChannelGroup_;     //Channel group to hold all channels
};
