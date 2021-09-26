# Overview
## Intent
Show a way of structuring a project like an engineer.

There are many good "Getting Started..." guides that get you from parts in a box
to your first blinking lights; that's not this project's goal.

## Checklist
While this list will vary depending on the requirements of the project, I feel
like this is a good minimum:

- [x] CMake
  with [Modern Structure](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html)
- [x] Platform specific build (Pico in this project)
- [x] Desktop build (testing and analysis)
- [x] Testing framework
	- [x] Desktop
	- [ ] Platform
- [x] Build scripts
	- [x] Docker to abstract cross-compile
- [x] Documentation
	- [x] Readme
	- [x] structure

# Workflow
## Checkout
Don't forget to init the submodules! `git submodule update --init`

## Building
### Scripts
Building on a Linux desktop is the easiest:
	cd scripts && ./build

# Programming Pico
## Prerequisites

1. OpenOCD (link)
2. Wire up two Picos

## Commands

    openocd -f tcl/interface/picoprobe.cfg -f tcl/target/rp2040.cfg -c "program <path>/pico-blink.elf verify reset exit"

### Troubleshooting
If you get an error like:

```
...
Info : RP2040 Flash Bank Command
Error: libusb_open() failed with LIBUSB_ERROR_ACCESS
Error: Failed to open or find the device
Error: Can't find a picoprobe device! Please check device connections and permissions.
Error: No Valid JTAG Interface Configured.
```

Make sure that udev rules are updated.

1. Copy the rules file `openocd/contrib/60-openocd.rules` to `/etc/udev/rules.d/`
2. Disconnect and reconnect the board.

If that does not work, make sure that the following rule is in the file:

```
# Raspberry Pi Picoprobe
ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="0004", MODE="660", GROUP="plugdev", TAG+="uaccess"
```

There is currently an open PR with this
fix: [Add picoprobe to contrib udev rules #5](https://github.com/raspberrypi/openocd/pull/5/commits/14e328ee76a02f1ab28a5f08b59d80112c9ea692)



