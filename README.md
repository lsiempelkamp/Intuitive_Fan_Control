# Intuitive Coding Challenge: Fan Control

![Simulation Plot](https://github.com/lsiempelkamp/Intuitive_Fan_Control/blob/master/Docs/GraphOutput.png)

## Description
[Coding Challenge Description PDF Doc](Docs/IntuitiveCodingChallenge_FanControl.pdf)

A mocked up fan controller for an embedded application with simulator running from a console application.

## Installation/Setup

Developed and built using [Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/) with the "Desktop development with C++" workload installed **with default settings**.

See [Install C and C++ support in Visual Studio](https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170) for step-by-step installation instructions.

The latest build of the project can be run from within a command terminal. Find the executable here: [Intuitive_Fan_Control/Latest Build/Intuitive_Fan_Control.exe](Latest&#32;Build/)

## Application Notes
![System Diagram](https://github.com/lsiempelkamp/Intuitive_Fan_Control/blob/master/Docs/Intuitive&#32;Fan&#32;Control&#32;Diagram.png)
The source code is architected loosely as shown in the diagram above.
There are a set of objects related to simulated/mock systems, a set of objects that make up the target micro-controller running the fan controller, and a pair of objects that represent the shared (mock) IPC and Fan Control Registers.

At startup - the user is prompted to set some parameters for the systems/simulation: Ambient Temperature, Number Of Active SubSystems, Simulation Run-Time, etc. 

![System Diagram](https://github.com/lsiempelkamp/Intuitive_Fan_Control/blob/master/Docs/console_user_setup.png)

These inputs populate a user_config struct that is depended upon by all other components. See user_config.h for additional hard-coded settings.

The requested number of active sub-systems are spawned/dynamically allocated for the simulation according to the user_config.

Then the simulation begins - with fan controller intiially disabled - and then enabled after a configured delay time.
Live system tempertures (represented as decimal float and 4-byte hex vals) and fan speeds (duty_cycle 0-100% and associated uint32_t pwm val) are periodically reported to the console
Once the simulation has run for the configured amount of time it ends and the application closes - it must be restarted to run another simulation.

![System Diagram](https://github.com/lsiempelkamp/Intuitive_Fan_Control/blob/master/Docs/console_sim_results.png)

When plotted - the time history of temperatures for the mock systems looks something like below:
![Simulation Plot](https://github.com/lsiempelkamp/Intuitive_Fan_Control/blob/master/Docs/GraphOutput.png)
