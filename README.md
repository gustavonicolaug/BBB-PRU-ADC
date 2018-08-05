# BBB-PRU-ADC

Here you will find an example of how to configure and program PRU(Programable Real-time Unit) from Beaglebone Black to use it's ADC module.

## Getting Started

Beaglebone Black has two Programable Real-time Units(PRUs) that work in parallel to the CPU and a built-in ADC module, with this two features it's possible to capture an analog signal and convert it to digital data. 

This project consists in a C solution named loader that loads an Assembly code in the two Beaglebone Black's PRUs which will trigger the ADC module to sample from channel 1 (AIN0) and store one byte at time in two buffers of 6kB from PRU Shared RAM(12kB). 

After one buffer is full an interrupt signal is send to the loader program and it will read that buffer of data.


### Prerequisites

Assume Debian distribution for IoT version 8.2 (without graphical desktop) with kernel v4.4+.

### Installing

## Running the tests
 
## Authors
	Gustavo Nicolau Gonçalves
	README.md based on **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)
## License

see the [LICENSE.md](LICENSE.md) file for details
