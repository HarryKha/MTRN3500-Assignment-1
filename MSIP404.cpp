#include "MSIP404.h"

//#define base_addr 0x200
#define BASEINDEX 0x800
#define SIZE 32
#define SIZEINDEX 8

EmbeddedDevice::MSIP404::MSIP404(EmbeddedOperations* eops, uint32_t base_addr) //Constructor
{
	if (eops->ioperm(base_addr, SIZE, 1) != 0) { //Requesting access to the Encoder
		std::cout << "Permission Failed accessing MSI-404" << std::endl;
		exit(1);
	}

	if (eops->ioperm(BASEINDEX, 8, 1) != 0) {
		std::cout << "Permission Failed accessing MSI-404 BASE INDEX" << std::endl;
		exit(1);
	}

	this->base_addr = base_addr;
	this->eops = eops;
}

EmbeddedDevice::MSIP404::~MSIP404() //Destructor
{
	this->base_addr = 0;
	this->eops = 0;
}

void EmbeddedDevice::MSIP404::resetChannel(uint8_t channel) //Resets the selected channel
{

	eops->outb(0, base_addr + channel);
	return;
}

int32_t EmbeddedDevice::MSIP404::readChannel(uint8_t channel) //Reads in the selected channel and returns int32_t
{

	int channelLSB = 4 * channel;
	uint8_t msb = eops->inb(base_addr + channelLSB + 3);
	uint8_t smsb = eops->inb(base_addr + channelLSB + 2);
	uint8_t tmsb = eops->inb(base_addr + channelLSB + 1);
	uint8_t lsb = eops->inb(base_addr + channelLSB);
	int32_t count = lsb + (tmsb << 8) + (smsb << 16) + (msb << 24); //Left shift to get correct values
	return count;
}

bool EmbeddedDevice::MSIP404::readIndex(uint8_t channel) //Reads in the Index of the selected channel and returns true/false
{

	int i = 0;
	int channelIndex = 0;
	int j = 0;

	for (i = 0, channelIndex = 0, j = 0; i < channel; i++, j++) { //Selects which bit to mask
		if (j == 1) {
			j = 0;
			channelIndex++;
		}
	}

	if ((eops->inb(BASEINDEX + channelIndex) & (1 << 7)) && (j == 0)) { //Test for bit 7
		return true;
	}
	else if ((eops->inb(BASEINDEX + channelIndex) & (1 << 5)) && (j == 1)) { //Test for bit 5
		return true;
	}
	return false;
}

bool EmbeddedDevice::MSIP404::operator!() //Resets all the channels
{

	for (int i = 0; i < 8; i++) {
		eops->outb(1, base_addr + i);
	}
	return true;
}
