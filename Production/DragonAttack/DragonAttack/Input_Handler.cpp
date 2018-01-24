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
	if (AEInputCheckCurr(AEVK_D))
	{
		Enqueue(AEVK_D);
	}

	if (AEInputCheckCurr(AEVK_A))
	{
		Enqueue(AEVK_A);
	}

	if(AEInputCheckTriggered(AEVK_SPACE))
	{
		Enqueue(AEVK_SPACE);
	}

	if (AEInputCheckTriggered(AEVK_RETURN))
	{
		Enqueue(AEVK_RETURN);
	}

	if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
	{

	}
}