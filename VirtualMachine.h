#include <cstdint>

#if defined _MSC_VER && !defined _CRT_USE_BUILTIN_OFFSETOF
    #ifdef __cplusplus
        #define offsetof(s,m) ((::size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
    #else
        #define offsetof(s,m) ((size_t)&(((s*)0)->m))
    #endif
#else
    #define offsetof(s,m) __builtin_offsetof(s,m)
#endif

#define EAX offsetof(VirtualMachine::VirtualMachineContext, eax)
#define EBX offsetof(VirtualMachine::VirtualMachineContext, ebx)
#define ECX offsetof(VirtualMachine::VirtualMachineContext, ecx)
#define EDX offsetof(VirtualMachine::VirtualMachineContext, edx)
#define EBP offsetof(VirtualMachine::VirtualMachineContext, ebp)
#define ESP offsetof(VirtualMachine::VirtualMachineContext, esp)
#define ESI offsetof(VirtualMachine::VirtualMachineContext, esi)
#define EDI offsetof(VirtualMachine::VirtualMachineContext, edi)

#define PUSH_REGISTER(x) 0, x
#define POP_REGISTER(x) 1, x
#define VM_EXIT 2
#define XCHG_EAX_EBX 3
#define MOV_EAX_VALUE(x) 4, x 

namespace VirtualMachine
{
	extern std::uint32_t instruction_table[];

	struct VirtualMachineContext
	{
		std::uint32_t eax = 0;
		std::uint32_t ebx = 0;
		std::uint32_t ecx = 0;
		std::uint32_t edx = 0;
		std::uint32_t ebp = 0;
		std::uint32_t esp = 0;
		std::uint32_t esi = 0;
		std::uint32_t edi = 0;
	}virtual_machine_context;

	_declspec(naked, noinline) void PushRegister()
	{
		__asm
		{
			add edi, 0x4
			mov eax, [edi]
			mov eax, [esi + eax]
			sub [esi]VirtualMachineContext.esp, 0x4
			mov ebx, [esi]VirtualMachineContext.esp
			mov [ebx], eax
			add edi, 0x4
			mov eax, [edi]
			imul eax, 4
			add eax, ebp
			mov eax, [eax]
			jmp eax
		}
	}

	_declspec(naked, noinline) void PopRegister()
	{
		__asm 
		{
			add edi, 0x4
			mov eax, [edi]
			lea eax, [esi + eax]
			mov ebx, [esi]VirtualMachineContext.esp
			mov ebx, [ebx]
			mov [eax], ebx
			add [esi]VirtualMachineContext.esp, 0x4
			add edi, 0x4
			mov eax, [edi]
			imul eax, 4
			add eax, ebp
			mov eax, [eax]
			jmp eax
		}
	}

	_declspec(naked, noinline) void MovEAXValue()
	{
		__asm
		{
			add edi, 0x4
			mov eax, [edi]
			mov [esi]VirtualMachineContext.eax, eax
			add edi, 0x4
			mov eax, [edi]
			imul eax, 4
			add eax, ebp
			mov eax, [eax]
			jmp eax
		}
	}

	_declspec(naked, noinline) void XchgEAXEBX()
	{
		__asm
		{
			sub [esi]VirtualMachineContext.esp, 0x4
			mov eax, [esi]VirtualMachineContext.eax
			mov ebx, [esi]VirtualMachineContext.esp
			mov [ebx], eax
			mov eax, [esi]VirtualMachineContext.ebx
			mov [esi]VirtualMachineContext.eax, eax
			mov ebx, [esi]VirtualMachineContext.esp
			mov eax, [ebx]
			mov [esi]VirtualMachineContext.ebx, eax
			add [esi]VirtualMachineContext.esp, 0x4
			add edi, 0x4
			mov eax, [edi]
			imul eax, 4
			add eax, ebp
			mov eax, [eax]
			jmp eax
		}
	}

	_declspec(naked, noinline) void VMExit()
	{
		__asm
		{
			mov eax, virtual_machine_context.eax
			mov ebx, virtual_machine_context.ebx
			mov ecx, virtual_machine_context.ecx
			mov edx, virtual_machine_context.edx
			mov ebp, virtual_machine_context.ebp
			mov esp, virtual_machine_context.esp
			mov esi, virtual_machine_context.esi
			mov edi, virtual_machine_context.edi
			ret
		}
	}

	_declspec(naked, noinline) void __cdecl VMEntry(std::uint32_t* payload)
	{
		__asm
		{
			mov virtual_machine_context.eax, eax
			mov virtual_machine_context.ebx, ebx
			mov virtual_machine_context.ecx, ecx
			mov virtual_machine_context.edx, edx
			mov virtual_machine_context.ebp, ebp
			mov virtual_machine_context.esp, esp
			mov virtual_machine_context.esi, esi
			mov virtual_machine_context.edi, edi
			lea esi, virtual_machine_context
			mov edi, [esp + 0x4]
			lea ebp, instruction_table
			mov eax, [edi]
			imul eax, 4
			add eax, ebp
			mov eax, [eax]
			jmp eax
		}
	}

	std::uint32_t instruction_table[] = { reinterpret_cast<std::uint32_t>(PushRegister), reinterpret_cast<std::uint32_t>(PopRegister), reinterpret_cast<std::uint32_t>(VMExit), reinterpret_cast<std::uint32_t>(XchgEAXEBX), reinterpret_cast<std::uint32_t>(MovEAXValue) };
}
