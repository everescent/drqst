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

class Audio_Engine {

public:
  Audio_Engine(unsigned SoundNum, const std::function<void (std::vector<std::string>&)>& Init);
  ~Audio_Engine();
  void PlaySound(const int SongNum);
  void SetVolume(const int SongNum);

private:
  FMOD::System *Audio_;
  std::vector <std::string> Playlist_;
  std::vector <FMOD::Sound*> Soundlist_;
  std::vector <FMOD::ChannelGroup*> Channel_;
};
