# STM32
Repository that contains code for STM32 devices. 
Unless stated otherwise, the folders are projects to be opened with [SW4STM32](https://www.openstm32.org/HomePage)

## [STM32F412G_Disco_CDC_RNDIS_lwIP](STM32F412G_Disco_CDC_RNDIS_lwIP)
### Description
The project runs on a [STM32F412G-DISCO](https://www.st.com/en/evaluation-tools/32f412gdiscovery.html) board. 
This project is an example implementation of an USB CDC [Remote NDIS (RNDIS)](https://docs.microsoft.com/en-us/windows-hardware/drivers/network/overview-of-remote-ndis--rndis-) device. After being connected to a host (e.g. PC) the device a **R**emote **N**etwork **D**river **I**nterface **S**pecification ( [RNDIS](https://en.wikipedia.org/wiki/RNDIS) ) device. 
The project also contains a very simple [DHCP](https://en.wikipedia.org/wiki/Dynamic_Host_Configuration_Protocol) server implementation that assignes automatically an IP address to the 'virtual' network device exposed to the host (computer). The network stack is implemented via [lwIP](https://savannah.nongnu.org/projects/lwip/) version 2.1.2. lwIP is configured through the file [lwipopts.h](STM32F412G_Disco_CDC_RNDIS_lwIP/inc/lwipopts.h). Only IPv4 and UDP are enabled.

### Application
The application initializes the 
* USB stack (RNDIS),
* the network stack (lwIP)
* DHCP server

Then it enters the main loop where every second an UDP packet is sent to a specific IP address (that of the first client, the host). The content of the packet reads 'hello!'.

### ToDo
The project is considered WiP. Please frequently check for new updates.
