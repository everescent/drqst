/* Start Header ************************************************************************/
/*!
\file       Input_Handler.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Input from user handled here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Input_Handler.h"
#include "Camera.h"
#include <iostream>


namespace Input {

	std::vector <int> input;
	
	const std::vector <int> &Get_User_Input(void)
	{
		using namespace Input;

		AEInputUpdate();

		if (AEInputCheckCurr(go_right))
		{
			input.push_back(go_right);
		}

		if (AEInputCheckCurr(go_left))
		{
			input.push_back(go_left);
		}

		if (AEInputCheckCurr(go_up))
		{
			input.push_back(go_up);
		}

		if (AEInputCheckCurr(go_down))
		{
			input.push_back(go_down);
		}

		if (AEInputCheckTriggered(jump_up))
		{
			input.push_back(jump_up);
		}

		if (AEInputCheckTriggered(fire))
		{
			input.push_back(fire);
		}

		if (AEInputCheckTriggered(special))
		{
			input.push_back(special);
		}

		// if esc key or the close button was triggered, stop the game
		if (AEInputCheckTriggered(quit_game) || 0 == AESysDoesWindowExist())
		{
			GSM::next = GS_QUIT;
		}

		return input;
	}

	void ClearBuffer(void)
	{
		input.clear();
	}
}