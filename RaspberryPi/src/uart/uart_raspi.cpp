#include "uart_raspi.h"

uartConnection::uartConnection() {
	try {
		uartConnection::isListening = false;
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

		tcflush(fd, TCIFLUSH);
		if(tcsetattr(fd, TCSANOW, &config) == -1){
			throw "couldn't set termios config";
		}
	}
	catch(std::string e) {
		std::cout << e << std::endl;
	}
	catch(...) {
		std::cout << "error has been thrown" << std::endl;
	}
}

void uartConnection::startSending() {
}

/**
 * This function starts to listen for new messages.
 * It can be canceld by changeing isListening to false. 
 */
void uartConnection::startListening() {
	//start polling for input (wait for event on file descriptor)
	pollfd srcPoll;
	srcPoll.fd = fd;
	srcPoll.events = POLLIN;
	srcPoll.revents = 0;
	uartConnection::isListening = true;

	// Main loop that is polling for new messages
	char buf[BUFFERSIZE];
	int res = 0;
	while(isListening) {
		int check = poll(&srcPoll, 1, -1);
		if(check != 1) {
			std::cout << "package lost" << std::endl;
		}
		res = read(fd, buf, BUFFERSIZE);
		// use buf for something
	}
}

void uartConnection::stopListening() {
	uartConnection::isListening = false;
}

/**
 * This function sets up and initializes the termios object with the UART Protocol.
 * It will return the termios config struct that contains the settings for uart.
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
	config.c_iflag = IGNBRK;

	/*
		CONTROL MODES:
		CBAUD a number of Bauds given by some constants
		for example B50 for 50 baud, B110, B150, ..., B9600, B19200, B25600
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
	config.c_cflag = B19200 | PARODD | PARENB | CS8 | CREAD | CLOCAL;

	/*
		OUTPUT MODES:
		OPOST	Perform output processing
		OLCUC	Map lower case to upper on output a => A
		ONLCR	Map NL to CR-NL on output
		OCRNL	Map CR to NL on output
		ONOCR	No CR output at column 0
		ONLRET NL performs CR function
		OFILL	Use fill characters for delay
		OFDEL	Fill is DEL else NUL.
		NLDLY	Select new-line delays: NL0 NL1
		CRDLY	Select carriage-return delays: CR0 CR1 CR2 CR3
		TABDLY Select horizontal-tab delays: TAB0 TAB1 TAB2
		XTABS	Expand tabs to spaces
		BSDLY	Select backspace delays: BS0 BS1
		VTDLY	Select vertical tab delays: VT0 VT1
		FFDLY	Select form-feed delays: FF0 FF1	
	*/	
	config.c_oflag = 0;

	/*
		LOCAL MODES
		ECHO	Enable echo
		ECHOE	Echo ERASE as an error-correcting backspace
		ECHOK	Echo KILL
		ECHONL	Echo \n
		ICANON	Canonical input (erase and kill processing)
		IEXTEN	Enable extended functions
		ISIG	Enable signals
		NOFLSH	Disable flush after interrupt, quit or suspend
		TOSTOP	Send SIGTTOU for background output
	*/
	config.c_lflag = 0;

	// time between caracters in 10th of sec
	config.c_cc[VTIME] = 0;
	// blocking read() until x byte are recieved
	config.c_cc[VMIN] = 3;

	return config;
}

/**
 * Deconstructor to close the opened tty device
 */
uartConnection::~uartConnection() {
	close(fd);
}
