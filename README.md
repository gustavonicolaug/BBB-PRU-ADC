# BBB-PRU-ADC

Here you will find an example of how to configure and program PRU(Programable Real-time Unit) from Beaglebone Black to use it's ADC module.

## Getting Started

Beaglebone Black has two Programable Real-time Units(PRUs) that work in parallel to the CPU and a built-in ADC module, with this two features it's possible to capture an analog signal and convert it to digital data. 

This project consists in a C solution named loader that loads an Assembly code in the two Beaglebone Black's PRUs which will trigger the ADC module to sample from channel 1 (AIN0) and store one byte at time in two buffers of 6kB from PRU Shared RAM(12kB). 

After one buffer is full an interrupt signal is send to the loader program and it will read that buffer of data.

To make the sample rate more accurate the second PRU is used to send a timing signal to syncronize the first PRU which reads the ADC value, so the time between each measure will be controlled by the constant TIME in adc.hp.

### Prerequisites

Assume Debian distribution for IoT version 8.2 (without graphical desktop) with kernel v4.4+.

The library used to interface with the PRU can be cloned by this repository https://github.com/beagleboard/am335x_pru_package.git, using the following commands:
	
	git clone https://github.com/beagleboard/am335x_pru_package.git
	mkdir /usr/include/pruss/
	cp am335x_pru_package/pru_sw/app_loader/include/pruss_intc_mapping.h /usr/include/pruss
	cp am335x_pru_package/pru_sw/app_loader/include/prussdrv.h /usr/include/pruss
	cd am335x_pru_package/pru_sw/app_loader/interface/
	CROSS_COMPILE= make
	cd /root/am335x_pru_package/pru_sw/app_loader/lib 
	
	// Make sure that libprussdrv.a libprussdrv.so libprussdrvd.a libprussdrvd.so can be listed in the current directory
	
	cp libprussdrv.a libprussdrv.so libprussdrvd.a libprussdrvd.so /usr/lib
	cd /usr/lib/
	ldconfig
	cd /root/am335x_pru_package/pru_sw/utils/pasm_source/
	source linuxbuild
	cd ..
	cp pasm /usr/bin
	cd /usr/bin
	./pasm

	//Check for pasm usage statement

### Installing

## Running the tests
 
## Authors

Gustavo Nicolau Gonçalves
README.md based on **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

## License

see the [LICENSE.md](LICENSE.md) file for details
