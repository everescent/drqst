/* Start Header ************************************************************************/
/*!
\file       Input_Handler.cpp
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Cpp file for Input_Handler

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/

#include "Input_Handler.h"
#include "Camera.h"
#include <iostream>


namespace Input {

	

    int input[NUM_OF_KEYS] {};


    const int* Get_User_Input(void)
    {
        using namespace Input;

        memset(input, 0, sizeof(int) * NUM_OF_KEYS);

        if (AEInputCheckCurr(go_right))
        {
            input[0] = go_right;
        }

        if (AEInputCheckCurr(go_left))
        {
            input[1] = go_left;
        }

        if (AEInputCheckCurr(go_up))
        {
            input[2] = go_up;
        }

        if (AEInputCheckCurr(go_down))
        {
            input[3] = go_down;
        }

        if (AEInputCheckCurr(jump_up))
        {
            input[4] = jump_up;
        }

        if (AEInputCheckTriggered(fire))
        {
            input[5] = fire;
        }

        if (AEInputCheckTriggered(special))
        {
            input[6] = special;
        }

        return input;
    }

    bool Quit_Triggered()
    {
        if (AEInputCheckTriggered(quit_game) || 0 == AESysDoesWindowExist())
        {
            return true;
        }
        return false;
    }
}