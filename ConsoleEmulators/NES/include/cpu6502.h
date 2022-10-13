#pragma once
//#include <cstdint>

class NESBusSystem;

class CPU
{
public:
	CPU(NESBusSystem* nesBus);
	~CPU() = default;

	void writeData(uint16_t address, uint8_t data);
	uint8_t readData(uint16_t address);

	void reset();
	void clock();

public:
	// Registers of 6502 core (public to examine and debug later on)
	uint8_t A;
	uint8_t X;
	uint8_t Y;
	uint16_t PC; // Program Counter
	uint8_t SP; // Stack Pointer

	enum StatusFlags
	{

	};

private:
	NESBusSystem* bus;
};