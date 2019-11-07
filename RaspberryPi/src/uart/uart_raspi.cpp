#include "uart_raspi.h"

/**
 * Function that opens up the UART connection
 */
void uartConnection::connect() {
    try {
        // open the uart tty device
        const char *device = "/dev/ttyAMA0";
        fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1) {
            throw "failed to open";
        }
        // check if it was a tty device, not just a file
        if(!isatty(fd)) {
            throw "fd is no tty device";
        }

		  termios config = configSetup(fd);

        close(fd);
    }
    catch(std::string e) {
        std::cout << e << std::endl;
        close(fd);
    }
    catch(...) {
        std::cout << "error has been thrown" << std::endl;
        close(fd);
    }
}

/**
 * This function sets up and initializes the termios object with the UART Protocol
 * 
 * @param fd file descriptor of the uart tty device
 */
termios uartConnection::configSetup(int fd) {
	termios config;
	if(tcgetattr(fd, &config) < 0) {
		throw "could not get the current configuration of the tty device";
	}
	/*
	INPUT MODES:
		BRKINT Singal Interrupt on break
		IGNBRK Ignores Break conditions, if set generates SIGINT to Processgroup
		ICRNL Maps CR to NL (CR = /r; NL = /025) on input
		IGNCR ignores CR 
		IGNPAR Ignores characters with parity error
		INLCR Maps NL to CR (NL = /025; CR = /r) on input
		INPCK erlaubt den parity check
		ISTRIP Strip Character
		PARMRK Mark parity errors
		IXON Enables start/stop output control, 
			 if enalbed -> recieving stop caracter will suspend the output
		IXOFF Enables start/stop input control,
			 if enabled -> sends stop signal, to not overflow queue  
	*/
	config.c_iflag &= ~(IGNBRK | IXON | IXOFF);
	/*
	CONTROL MODES:

	*/
	config.c_cflag = PARODD | PARENB | CS8 | CREAD | CLOCAL;
	return config;
}
