# Getting Started with iperf test

These sections will guide you through a series of steps from configuring development environment to running iperf examples using the **WIZnet's ethernet products**.

- [Getting Started with iperf test](#getting-started-with-iperf-test)
  - [Development environment configuration](#development-environment-configuration)
  - [Hardware requirements](#hardware-requirements)
  - [iperf testing](#iperf-testing)


<a name="development_environment_configuration"></a>
## Development environment configuration

To test the iperf example, the development environment must be configured to use Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico.

The this example were tested by configuring the development environment for **Windows**. Please refer to the '**9.2. Building on MS Windows**' section of '**Getting started with Raspberry Pi Pico**' document below and configure accordingly.

- [**Getting started with Raspberry Pi Pico**][link-getting_started_with_raspberry_pi_pico]

**Visual Studio Code** was used during development and testing of this examples, the guide document in each directory was prepared also base on development with Visual Studio Code. Please refer to corresponding document.



<a name="hardware_requirements"></a>
## Hardware requirements

The examples use **Raspberry Pi Pico** and **WIZnet Ethernet HAT** - ethernet I/O module built on WIZnet's [**W5100S**][link-w5100s] ethernet chip, **W5100S-EVB-Pico** - ethernet I/O module built on [**RP2040**][link-rp2040] and WIZnet's [**W5100S**][link-w5100s] ethernet chip or **W5500-EVB-Pico** - ethernet I/O module built on [**RP2040**][link-rp2040] and WIZnet's [**W5500**][link-w5500] ethernet chip.

- [**Raspberry Pi Pico**][link-raspberry_pi_pico]

<p align="center"><img src="https://assets.raspberrypi.com/static/74679d6c81ffc5503a20b64feae2ed4f/2b8d7/pico-rp2040.webp"></p>

- [**WIZnet Ethernet HAT**][link-wiznet_ethernet_hat]

<p align="center"><img src="https://docs.wiznet.io/assets/images/wiznet-ethernet-hat-c8220ff29095e0b95a364782826a1a18.png"></p>

- [**W5100S-EVB-Pico**][link-w5100s-evb-pico]

<p align="center"><img src="https://docs.wiznet.io/assets/images/w5100s-evb-pico-1.1-side-adf5e1b613524c4256126b1bb4bd58a0.png"></p>

- [**W5500-EVB-Pico**][link-w5500-evb-pico]

<p align="center"><img src="https://docs.wiznet.io/assets/images/w5500_evb_pico_side-da676c5d9c41adedc0469b9f1810b81b.png"></p>




<a name="iperf_testing"></a>
## iperf testing

1. Download

If the iperf examples are cloned, the library set as a submodule is an empty directory. Therefore, if you want to download the library set as a submodule together, clone the iperf examples with the following Git command.

```cpp
/* Change directory */
// change to the directory to clone
cd [user path]

// e.g.
cd D:/RP2040

/* Clone */
git clone --recurse-submodules https://github.com/wiznet-mason/RP2040-HAT-IPERF-C

/* build */
cd RP2040-HAT-IPERF-C
mkdir build
cmake -G "NMake Makefiles" ..
nmake

```

With Visual Studio Code, the library set as a submodule is automatically downloaded, so it doesn't matter whether the library set as a submodule is an empty directory or not, so refer to it.

2. Setup ethetnet chip

Setup the ethernet chip in '**CMakeLists.txt**' in '**RP2040-HAT-IPERF-C/**' directory according to the evaluation board to be used referring to the following.

- WIZnet Ethernet HAT : W5100S
- W5100S-EVB-Pico : W5100S
- W5500-EVB-Pico : W5500

For example, when using WIZnet Ethernet HAT or W5100S-EVB-Pico :

```cpp
# Set ethernet chip
set(WIZNET_CHIP W5100S)
```

When using W5500-EVB-Pico :

```cpp
# Set ethernet chip
set(WIZNET_CHIP W5500)
```

3. Test

Please refer to 'README.md' in each example directory to find detail guide for testing iperf examples.
<!--
Link
-->

[link-getting_started_with_raspberry_pi_pico]: https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
[link-rp2040]: https://www.raspberrypi.org/products/rp2040/
[link-w5100s]: https://docs.wiznet.io/Product/iEthernet/W5100S/overview
[link-w5500]: https://docs.wiznet.io/Product/iEthernet/W5500/overview
[link-raspberry_pi_pico]: https://www.raspberrypi.org/products/raspberry-pi-pico/

[link-wiznet_ethernet_hat]: https://docs.wiznet.io/Product/Open-Source-Hardware/wiznet_ethernet_hat

[link-w5100s-evb-pico]: https://docs.wiznet.io/Product/iEthernet/W5100S/w5100s-evb-pico

[link-w5500-evb-pico]: https://docs.wiznet.io/Product/iEthernet/W5500/w5500-evb-pico

[link-iolibrary_driver]: https://github.com/Wiznet/ioLibrary_Driver
[link-pico_sdk]: https://github.com/raspberrypi/pico-sdk
[link-pico_extras]: https://github.com/raspberrypi/pico-extras
