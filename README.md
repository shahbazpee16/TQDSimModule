# TQDSimModule
This is a public repository that provides the code for Tandem Queue Decomposition (TQD), a state-of-the-art fast and safe routing policy for Quantum Key Distribution (QKD) networks.
Desclaimer: Anyone using this code must cite our paper "Fast and Secure Routing Algorithms for Quantum Key Distribution Networks" available on "....".

TQD Policy: 
    Please refer to our paper which was published in COMSNETS 2022 titled for more information about this policy: [TQD_COMSNET_2022.pdf](https://github.com/shahbazpee16/TQDSimModule/files/8342115/TQD_COMSNET_2022.pdf)

OMNeT++ is a modular, component-based C++ simulation library and framework, primarily for building network simulators. OMNeT++ can be used for free for non-commercial simulations like those at academic institutions and for teaching.

OMnet++ Simulation Manual: [SimulationManual.pdf](https://github.com/shahbazpee16/IIT_Madras_28_02_22/files/8162070/SimulationManual.pdf)

OMNeT++ Installation Guide: Please refer to [Installation_Guide.pdf](https://github.com/shahbazpee16/IIT_Madras_28_02_22/files/8162138/Installation_Guide.pdf) for installing the simulator for different platforms.

OMNeT++ at a Glance:
  (a) IDE: The Simulation IDE is where we can create and evaluate network simulations. We can also develop, build, and run simulations from the command line as well. Because the IDE is based on Eclipse, we can add our own extensions to it in addition to the OMNeT++ tools.
	
  (b) C++: Models are written in C++. They make use of the simulation kernel, an efficient and feature-rich C++ runtime library. The IDE has your codebase fully indexed, and offers code completion, go to definition, call hierarchy, find references, and other features that make C++ programming a joy.
	
  (c) NED: Model components are described and assembled in a high-level domain-specific language called NED. NED can be edited both graphically and in source mode. The NED source editor also boasts intelligent navigation and code manipulation features similar to the C++ editor.
	
  (d) Msg: Protocol headers and other messages are represented in the code with C++ classes, but the simulation framework spares you the tedium of writing the C++ code manually by providing a domain-specific language that the code can be generated from, alongside with serialization and reflection code.
	
  (e) Ini: In OMNeT++, model parameters can be assigned or given default value already in NED, but real parameterization takes place in ini files that also carry configuration options for the simulator. Ini files also let you describe multiple configurations (~experiments) and parameter studies (simulation campaigns) to be carried out on your model. Ini file details can be hidden behind a GUI for non-expert users of your model.
	
  (f) Tools: Many additional command line and IDE tools help your work. Simulations can be run under a GUI that lets you inspect, animate and debug your model. Simulations can record a detailed event log that can be visualized as an interactive sequence chart for debugging, demo or documentation. Simulation results can be plotted in the IDE's analysis tool that also lets you post-process data, and store reproducible "recipes." Command-line tools let you build the model, run batches, process event log and result files, and do other tasks.
  
  
