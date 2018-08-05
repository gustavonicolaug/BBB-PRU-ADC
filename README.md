# BBB-PRU-ADC

Here you will find an example of how to configure and program PRU(Programable Real-time Unit) from Beaglebone Black to use it's ADC module.

## Getting Started

Beaglebone Black has two Programable Real-time Units(PRUs) that work in parallel to the CPU and a built-in ADC module, with this two features it's possible to capture an analog signal and convert it to digital data. 

This project consists in a C solution named loader that loads an Assembly code in the two Beaglebone Black's PRUs which will trigger the ADC module to sample from channel 1 (AIN0) and store one byte at time in PRU Shared RAM(12kB). After the buffer is full an interrupt signal is send to the loader program and it will read that buffer of data.


### Prerequisites

Assume Debian distribution for IoT version 8.2 (without graphical desktop) with kernel v4.4+.

What things you need to install the software and how to install them

```
Give examples
```

### Installing

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
