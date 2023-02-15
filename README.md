# Simple-Process-Hollowing
A quick and simple example of how to use process hollowing for malicious activities

# What is Process Hollowing ?
Process hollowing is a technique used by malware to hide its presence on a system by creating a new process and replacing its code and data with that of a legitimate process, while preserving the appearance of the legitimate process. By using this technique, malware can evade detection by security software that relies on detecting malicious processes based on their executable files or other attributes./ 
Process hollowing is also a technique used by some legitimate software for software protection and anti-piracy measures.

# How does it work ?
We start by creating a new process that will be hollowed out. The process will be created suspended and will only be resumed once we wrote the shellcode inside./

