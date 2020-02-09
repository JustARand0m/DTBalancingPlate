#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <cstdio>
#include <cstring>

const int MESSAGE_SIZE = 9;
const int MESSAGE_ID_PLATE = 0;
const int MESSAGE_ID_SERVO = 1;
const int MESSAGE_ID_SIZE = 2;

enum messageState{
	START_BIT, DATA, DONE
};

class messageDecoding {
	public:
		messageDecoding();
		void startDecoding(const uint8_t* message, const int amountOfData);
		bool isDone();
		std::vector<int> getData();
		std::string encodeMessage(int angle0, int angle1, int angle2);

	private:
		std::string decodedData;
		std::vector<int> data;
		bool done;
		messageState mState;

		bool decodeMessage(const uint8_t* message, const int amountOfData);
		uint16_t crc_ccitt(uint8_t *data, size_t dataSize);
		void convertPlateMessage();
};

