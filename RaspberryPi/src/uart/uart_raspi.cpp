#include "uart_raspi.h"
#include "../../include/exceptionHandling.h"

/**
 * The class constructer, needs the tty device as string, dont use 2 classes with
 * the same discriptor.
 * This function sets up uart.
 *
 * @param device The string for the device to be opend
 */
uartConnection::uartConnection(std::string device): BUFFERSIZE(1024) {
	try {
		isListening = false;
		// open the uart tty device
		fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd == -1) {
			throw exceptionHandling("failed to open the tty device");
		}
		// check if it was a tty device, not just a file
		if(!isatty(fd)) {
			throw exceptionHandling("fd is no tty device");
		}

		termios config = configSetup(fd);

		// This flushes the input buffer so messages that would be in queue before the init get deleted
		tcflush(fd, TCIFLUSH);
		// This sets the new config
		if(tcsetattr(fd, TCSANOW, &config) == -1){
			throw exceptionHandling("couldn't set termios config");
		}
	}
	catch(exceptionHandling &e) {
		std::cout << e.what() << std::endl;
	}
	catch(...) {
		std::cout << "error has been thrown" << std::endl;
	}
}

void uartConnection::writeData(std::string input) {
	// clears messages that couldnt be send or are still in the buffer
	tcflush(fd, TCOFLUSH);
	// write new data
	write (fd, input.c_str(), input.size() + 1);
}

/**
 * This function is a wrapper function around the listingThread function.
 * It will create a Thread that listens for input over UART and can be terminated
 * by setting the isListening flag to flase.
 */
void uartConnection::startListening() {
	std::thread t(listeningThread, std::ref(fd), std::ref(isListening), BUFFERSIZE);
	t.join();
}

/**
 * This static function is the function that is executed in an extra thread when calling
 * startListening(). It has a loop that looks for input over the uart connection.
 *
 * @param fdescr is the file descriptor passed as reference, to be read from
 * @param isListen is a bool flag that can cancel the loop.
 * @param BUFFSZ is the size of the buffer in which is written.
 */
void uartConnection::listeningThread(int &fdescr, bool &isListen, const int BUFFSZ) {
	// start polling for input (wait for event on file descriptor)
	pollfd srcPoll;
	srcPoll.fd = fdescr;
	srcPoll.events = POLLIN;
	srcPoll.revents = 0;
	isListen = true;

	// Main loop that is polling for new messages
	char buf[BUFFSZ];
	int res = 0;
	while(isListen) {
		int check = poll(&srcPoll, 1, -1);
		if(isListen == false) {
			break;
		}
		if(check != 1) {
			std::cout << "package lost" << std::endl;
		}
		// @todo maybe sleep here, cause data is trasmitted bitwise, wait until all data arrives?
		
		res = read(fdescr, buf, BUFFSZ);

		// @todo use buf for something
	}
}

void uartConnection::stopListening() {
	isListening = false;
	// stop for poll to give up control
	write(fd, "stop", 5);
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
		throw exceptionHandling("could not get the current configuration of the tty device");
	}
	memset(&config, 0, sizeof(config));
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
	config.c_cflag = PARODD | PARENB | CS8 | CREAD | CLOCAL;

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

	if(cfsetispeed(&config, B19200)) {
		throw exceptionHandling("speed couldnt be set");
	}

	return config;
}

/**
 * Deconstructor to close the opened tty device
 */
uartConnection::~uartConnection() {
	close(fd);
}
