#include "PCM3718.h"

//#define base_addr 0x300

EmbeddedDevice::PCM3718::PCM3718(EmbeddedOperations* eops, uint32_t base_addr)
{
	if (eops->ioperm(base_addr, 16, 1) != 0) {
		std::cout << "Permission failed accessing PCM3718" << std::endl;
		exit(1);
	}

	this->eops = eops;
	this->base_addr = base_addr;
	analog_range = 0;
	eops->outb(analog_range, base_addr);
}

EmbeddedDevice::PCM3718::PCM3718(EmbeddedOperations* eops, uint32_t base_addr, uint8_t analog_range)
{
	if (eops->ioperm(base_addr, 16, 1) != 0) {
		std::cout << "Permission failed accessing PCM3718" << std::endl;
		exit(1);
	}

	this->eops = eops;
	this->base_addr = base_addr;
	this->analog_range = analog_range;
	eops->outb(this->analog_range, base_addr);
}

EmbeddedDevice::PCM3718::~PCM3718()
{
	this->eops = NULL;
	this->analog_range = NULL;
	this->base_addr = NULL;
}

uint16_t EmbeddedDevice::PCM3718::digitalInput()
{

	uint8_t highByte = eops->inb(base_addr + 0xb);
	uint8_t lowByte = eops->inb(base_addr + 0x3);
	uint16_t Combined = lowByte | (highByte << 8);
	return Combined;
}

uint8_t EmbeddedDevice::PCM3718::digitalByteInput(bool high_byte)
{

	uint8_t lowByte = eops->inb(base_addr + 0x3);
	uint8_t highByte = eops->inb(base_addr + 0xb);

	if (high_byte) {
		return highByte;
	}
	return lowByte;
}

bool EmbeddedDevice::PCM3718::digitalBitInput(uint8_t bit)
{

	uint8_t lowByte = eops->inb(base_addr + 0x3);
	uint8_t highByte = eops->inb(base_addr + 0xb);
	uint16_t Combined = lowByte + (highByte << 8);
	return Combined & (1 << bit); //Double check that it has been left shifted properly
}

void EmbeddedDevice::PCM3718::digitalOutput(uint16_t value)
{

	uint8_t lowByte = (value & 0xFF);
	uint8_t highByte = (value >> 8) & 0xFF;
	eops->outb(lowByte, base_addr + 0x3);
	eops->outb(highByte, base_addr + 0xb);
	return;
}

void EmbeddedDevice::PCM3718::digitalByteOutput(bool high_byte, uint8_t value)
{

	uint8_t lowByte = value;
	uint8_t highByte = value;
	if (high_byte) {
		eops->outb(highByte, base_addr + 0xb);
		return;
	}
	eops->outb(lowByte, base_addr + 0x3);
	return;
}

void EmbeddedDevice::PCM3718::setRange(uint8_t new_analog_range)// need to test
{

	analog_range = new_analog_range;
	eops->outb(new_analog_range, base_addr + 1);
	return; 
}

double EmbeddedDevice::PCM3718::analogInput(uint8_t channel) const// need to test
{

 	eops->outb(0, base_addr + 8);
	eops->outb(channel | (channel << 4), base_addr + 2);
	eops->outb(analog_range, base_addr + 1);
	//usleep(100000); uncomment this when testing analogInput
	eops->outb(0, base_addr);
	while (eops->inb(base_addr + 8) & 0x80);

	int lowByte = eops->inb(base_addr);
	int highByte = eops->inb(base_addr + 1);
	int combined = ((lowByte >> 4) | (highByte << 4));
	double lowerBound;
	double upperBound;

	if (analog_range & 4) { //unipolar
		lowerBound = 0;
		upperBound = 10;
		for (int i = 0; i < (analog_range & 3); i++) {
			upperBound = upperBound / 10;
		}
	}
	else { //bipolar
		if (analog_range & 8) { //+-10 to +-0.01
			lowerBound = -10;
			upperBound = 10;
			for (int i = 0; i < (analog_range & 3); i++) {
				upperBound = upperBound / 10;
				lowerBound = lowerBound / 10;
			}
		}
		else
		{ //+-5 to +-0.005
			lowerBound = -5;
			upperBound = 5;
			for (int i = 0; i < (analog_range & 3); i++) {
				upperBound = upperBound / 10;
				lowerBound = lowerBound / 10;
			}
		}
	}

	return ((combined*(upperBound - lowerBound))/4095) + lowerBound;
}
