/* Start Header ************************************************************************/
/*!
\file       Knight.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Header file for Knight class

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Characters.h"   // parent class
#include "Dragon.h"       // player class
#include "Boss_States.h"  // Ai states

/**************************************************************************************
// 
// A class that creates a knight object
//
**************************************************************************************/
class Knight : public Characters
{
private:
    //------------------------------------------------------------------
    //
    //  BEHAVIOUR FOR KNIGHT
    //
    //------------------------------------------------------------------
    void Idle(const Dragon&, const float dt);         // idle state
    void Moving(const Dragon&, const float dt);       // moving state
    void Attack(Dragon&, const float dt);             // attack state
    void Dead(void);                                  // dead state

    //------------------------------------------------------------------
    //
    //  HELPER FUNCTIONS
    //
    //------------------------------------------------------------------
    void Set_Forward_Dir(const Dragon&);      // direction the knight faces
    bool Line_Of_Sight(const Dragon&);        //player visible to knight

    //------------------------------------------------------------------
    //
    //  KNIGHT VARIABLES
    //
    //------------------------------------------------------------------
    Boss_Action_State    current_action;      // current state of knight
    Boss_Attack          stab;                // stab attack of knight
    float                time_traveled;       // timer for duration moved
    Animation            anim;                // knight animation
    Audio_Engine         music;               // music for knight

public:
    Knight(const AEVec2&, Sprite* );          // coverting constructor/ only constructor
    ~Knight();                                // destructor to delete sprite
    void Update(Dragon&, const float dt);     // update the knight behaviour
    void Render(void);                        // render the knight on screen
    void Mute(void) override;
    void Unmute(void) override;
};