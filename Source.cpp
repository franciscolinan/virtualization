#include "virtual_machine.h"

int main()
{
	std::uint32_t payload[] = { PUSH_REGISTER(EAX), POP_REGISTER(EBP), VM_EXIT };
	virtual_machine::VMEntry(payload);
	return 0;
}
