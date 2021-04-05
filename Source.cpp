#include "VirtualMachine.h"

int main()
{
	std::uint32_t payload[] = { PUSH_REGISTER(EAX), POP_REGISTER(EBP), VM_EXIT };
	VirtualMachine::VMEntry(payload);
	return 0;
}
