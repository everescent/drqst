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
#include <vector>     //vector
#include <string>     //string
#include <functional> //function

namespace {
  //Name was too long. Yuck. 
  typedef std::function< void(std::vector< std::string >&) > Audio_Init;
}

/****************************************************************************************
Description:
  Creates an Audio Engine, powered by FMOD.
Constructors:
  Contructs the audio engine by using the user provided number of audio states and
  list of audio files. 
Deconstructor:
  Relase the audio resources. 
****************************************************************************************/
class Audio_Engine {

public:
  /**************************************************************************************
  Description:
    Provide number of songs, and give a function to initialize the list of filenames.
  SoundNum:
    The number of audio states.
  Init:
    A function that takes in a reference to a vector of std::string and returns void.
    User pushes back the filenames of the music he needs. 
  **************************************************************************************/
  Audio_Engine   (unsigned          SoundNum,
                  const Audio_Init& Init               );

  /**************************************************************************************
  Description:
    Release the audio system.
  **************************************************************************************/
  ~Audio_Engine  (                                     );

  /**************************************************************************************
  Description:
    Plays the song numnber given.
  SoundNum:
    The song to be played.
  **************************************************************************************/
  void Play      (const int SongNum                    );

  /**************************************************************************************
  Description:
    Set volume for the given song number.
  SoundNum:
    The song to which the volume is to be set for. 
  **************************************************************************************/
  void SetVolume (const int SongNum, const float Volume);

  /**************************************************************************************
  Description:
    Set song to loop, does not loop by default.
  SoundNum:
    The song to which the loop is to be set for.
  **************************************************************************************/
  void SetLoop   (const int SongNum, FMOD_MODE   Loop  );

  /**************************************************************************************
  Description:
    Pause the given song number.
  SoundNum:
    The song to pause.
  **************************************************************************************/
  void SetPause  (const int SongNum, const bool  Pause );

  /**************************************************************************************
  Description:
    Returns a bool indicating if the song has completed. 
  SoundNum:
    The song to check for.
  **************************************************************************************/
  bool GetPlaying(const int SongNum                    );

  /**************************************************************************************
  Description:
    Updates the audio system. CALL THIS!
  **************************************************************************************/
  void Update    (                                     );

private:
  FMOD::System*                        Audio_       ; //The audio system
  std ::vector        <std ::string  > Playlist_    ; //The filenames for each song
  std ::vector        <FMOD::Sound*  > Soundlist_   ; //The songs
  std ::vector        <FMOD::Channel*> Channel_     ; //The channel for each song
  FMOD::ChannelGroup*                  ChannelGroup_; //Channel group to hold all channels
};
