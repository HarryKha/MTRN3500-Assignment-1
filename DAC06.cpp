#include "DAC06.h"

//#define BASE 0x320
#define SIZE 12

EmbeddedDevice::DAC06::DAC06(EmbeddedOperations* eops, uint32_t base_addr) //Constructor
{
	if (eops->ioperm(base_addr, SIZE, 1) != 0) { //Requesting access to DAC
		std::cout << "Permission failed accessing DAC06!" << std::endl;
		exit(1);
	}

	this->eops = eops;
	this->base_addr = base_addr;
}

EmbeddedDevice::DAC06::~DAC06() //Destructor
{
	this->base_addr = 0;
	this->eops = 0;
}

void EmbeddedDevice::DAC06::analogOutputRaw(uint8_t channel, uint16_t value) //Outputs a value which corresponds to voltage to a selected channel
{

	int channelAddr = 2 * channel;
	uint8_t lowByte = value & 0xFF;
	uint8_t highByte = (value >> 8) & 0x0F;
	eops->outb(lowByte, base_addr + channelAddr);
	eops->outb(highByte, base_addr + channelAddr + 1);
	return;
}

void EmbeddedDevice::DAC06::analogOutputVoltage(uint8_t channel, double desired_voltage) //Outputs the desired voltage to a selected channel
{

	if ((desired_voltage > 5) || (desired_voltage < -5)) {
		std::cout << "Voltage out of range! Input between -5 and 5!" << std::endl;
		exit(1);
	}
	
	int channelAddr = 2 * channel;
	double voltage = ((819 / 2) * desired_voltage) + 2047.5;
	uint8_t lowByte = uint16_t(voltage) & 0xFF;
	uint8_t highByte = (uint16_t(voltage) >> 8) & 0x0F;
	eops->outb(lowByte, base_addr + channelAddr);
	eops->outb(highByte, base_addr + channelAddr + 1);
	return;
}
