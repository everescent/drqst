#include "Input_Handler.h"
#include "AEEngine.h"
#include <iostream>

struct Input_Queue
{
	int value;
	Input_Queue *next;
};

// input buffer
namespace
{
	Input_Queue *user_input = nullptr;
	Input_Queue *input_tail = nullptr;

	const int jump_up   = AEVK_SPACE;
	const int go_left   = AEVK_A;
	const int go_right  = AEVK_D;
	const int fire      = AEVK_RETURN;
	const int special   = AEVK_P;
	const int quit_game = AEVK_ESCAPE;
}

// create a new input
Input_Queue *New_Input(int value)
{
	Input_Queue *new_node = new Input_Queue;
	new_node->value = value;
	new_node->next = nullptr;

	return new_node;
}

// checks if the input buffer is empty or not
bool IsEmpty(void)
{
	return (user_input == nullptr);
}


// adds a new node to the end of the input buffer
void Enqueue(int value)
{
	// input buffer is empty
	if (IsEmpty())
	{
		user_input = New_Input(value);
		input_tail = user_input;
		return;
	}
	else // adds the node to the end of the input buffer
	{
		input_tail->next = New_Input(value);
		input_tail = input_tail->next;
	}


}


// removes the front of the input buffer
void Dequeue(void)
{
	// input buffer is empty
	if (IsEmpty())
		return;

	if (user_input->next == nullptr) // last item in input buffer
	{
		delete user_input;
		user_input = nullptr;
		return;
	}

	else 
	{
		Input_Queue *temp = user_input;
		user_input = temp->next;
		delete temp;
		return;
	}
}

// clears the input buffer/queue
void Clear_Buffer(void)
{
	if (IsEmpty())
		return;

	Input_Queue *temp = user_input;

	while (user_input)
	{
		user_input = user_input->next;
		delete temp;
		temp = user_input;
	}
	input_tail = nullptr;
}


void Get_User_Input(void)
{
	AEInputUpdate();
	
	if (AEInputCheckCurr(go_right))
	{
		Enqueue(go_right);
	}

	if (AEInputCheckCurr(go_left))
	{
		Enqueue(go_left);
	}

	if(AEInputCheckTriggered(jump_up))
	{
		Enqueue(jump_up);
	}

	if (AEInputCheckTriggered(fire))
	{
		Enqueue(fire);
	}

	if (AEInputCheckTriggered(special))
	{
		Enqueue(special);
	}

	// if esc key or the close button was triggered, stop the game
	if (AEInputCheckTriggered(quit_game) || 0 == AESysDoesWindowExist())
	{
		GSM::current = GS_QUIT;
	}
}