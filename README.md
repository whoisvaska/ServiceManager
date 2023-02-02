# Technical task for Veeam C++ Developer job
# Service Manager that implements Windows Service Management.

## Task
> 1. Write a COM object in C++ that implements the Windows Service Management functionality.
> 2. Develop a user interface (UI) application, using any language of your choice, that uses the COM object
> created in section 1) and allows you to select a service on a computer, display its current state, and
> perform operations applicable to the selected service.

## Example of work of the developed solution:

The image shows:
- the general parameters of the **RpcSs** service;
- the list of the services installed on machine;
- the recovery options of the **AnyDesk** service.

<img width="863" alt="examples" src="https://user-images.githubusercontent.com/48870780/192868007-906c341f-009e-4b47-b701-de898b718a0d.png">

## Diagram of the developed solution

[UML of GUI application and simplified algorithm of interaction with COM](https://github.com/whoisvaska/ServiceManager/blob/master/uml_and_algorithm.pdf)

## Development tools

- The server side was build using **Microsoft Visual Studio 2019**. 
- The client side was built using **Qt VS Tools (Qt 5.15.2)**.
  
