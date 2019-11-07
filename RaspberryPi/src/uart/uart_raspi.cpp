#include "uart_raspi.h"

/**
 * Function that opens up the UART connection
 */
void uartConnection::connect() {
    try {
        // open the uart tty device
        const char *device = "/dev/ttyS0";
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
		IGNBRK Ignores Break conditions
			Break condition = if the Sender sends 0 for more than 1 Frame (0 = dominant)
		BRKINT Singal Interrupt on break
			if IGNBRK=0 and BRKINT=1 => output and input buffer will be flashed and 
			it will cause a SIGINT in the controlling Terminal
		IGNCR ignores CR on input
		ICRNL Maps CR to NL(newline) (CR = /r; NL = /025) on input
			unless IGNCR is set
		IGNPAR Ignores characters with parity error
		INLCR Maps NL to CR (NL = /025; CR = /r) on input
		INPCK erlaubt den parity check
		ISTRIP Strip off the 8th bit
		PARMRK Mark parity errors
		IXON Enables start/stop output control, 
			 if enalbed -> recieving stop caracter will suspend the output
		IXOFF Enables start/stop input control,
			 if enabled -> sends stop signal, to not overflow queue  
	*/
	config.c_iflag = IGNPAR;
	/*
	CONTROL MODES:
		CLOACL ignore modem control line CD is Carrier Detection (if another device is found)
		CREAD enables Reciver
		CSIZE Character size mask. Values are CS5, CS6, CS7 or CS8 
			The size of the transmission and reception
		CSTOPB Sets 2 stop bits rather than one 
			for minimizing error if baudrate is very high
		HUPCL lower modem control line if connection is cut
		PARENB Enalbe Parity generation on output and parity checking on input
		PARODD if set parity check for input and output is odd else even
	*/
	config.c_cflag = PARODD | PARENB | CS8 | CREAD | CLOCAL;
	return config;
}
