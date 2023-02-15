# Simple Process Hollowing
A quick and simple example of how to use process hollowing for malicious activities.

## What is Process Hollowing ?
Process hollowing is a technique used by malwares to hide their presence on a system by creating a new process and replacing their code and data with malicious code, while preserving the appearance of the legitimate process. By using this technique, malwares can evade detection by security software that relies on detecting malicious processes based on their executable files or other attributes.\
Process hollowing is also a technique used by some legitimate software for software protection and anti-piracy measures.

## How does it work ?
The goal is to create a process and write our malicious shellcode inside. In the example I didn't provided any shellcode but you can easily create one using msfvenom.

We start by creating a new process that will be hollowed out. The process will be created suspended and will only be resumed once we wrote the shellcode inside.\
We then allocate memory in our process and write our shellcode in it.


Now that our malicious shellcode is implemented we have to get the context in order to update it.\
We retrieve the old context and all registers and flags. Then we edit the two registers EAX (used for functions parameters and return values) and EIP (point to the next instruction to be executed) and give them the address of our shellcode. That way, our shellcode will be executed when we will resume the process.\
We can finally overwrite the process context with our context, resume the process and clean up the memory.


## Detailed Method of context updating
We first retrieves the context of the target thread using the ```GetThreadContext``` function, which fills the ```CONTEXT``` structure with the thread context.\
Then we sets the value of the EAX register to the address of the shellcode entry point. This is necessary because the shellcode entry point must be executed by the processor.\
We do the same thing for the EIP register. This register points to the next instruction to be executed. Thanks to this our shellcode will be executed immediately after the process is resumed.\
Finally we update the thread context using ```SetThreadContext``` so it replace the old context by ours.\
We can now resume the suspended process and clean up the memory.
