#include "cpu6502.h"
#include "nesBus.h"

CPU::CPU(NESBusSystem* nesBus) : bus(nesBus)
{
}

void CPU::writeData(uint16_t address, uint8_t data)
{
	bus->writeData(address, data);
}

uint8_t CPU::readData(uint16_t address)
{
	return bus->readData(address);
}

void CPU::reset()
{
	A = 0;
	X = 0;
	Y = 0;
	status = 0x00 | U; //Initially all zeros except unused bit 5 which is always 1
}

void CPU::clock()
{
}

bool CPU::isLastCompleted() const
{
	return cyclesInstruction == 0;
}


// ----- ADDRESSING MODES -----

uint8_t CPU::IMP()
{
	return uint8_t();
}

uint8_t CPU::IMM()
{
	return uint8_t();
}

uint8_t CPU::ZP0()
{
	return uint8_t();
}

uint8_t CPU::ZPX()
{
	return uint8_t();
}

uint8_t CPU::ZPY()
{
	return uint8_t();
}

uint8_t CPU::REL()
{
	return uint8_t();
}

uint8_t CPU::ABS()
{
	return uint8_t();
}

uint8_t CPU::ABX()
{
	return uint8_t();
}

uint8_t CPU::ABY()
{
	return uint8_t();
}

uint8_t CPU::IND()
{
	return uint8_t();
}

uint8_t CPU::IZX()
{
	return uint8_t();
}

uint8_t CPU::IZY()
{
	return uint8_t();
}

// ----- INSTRUCTIONS -----

uint8_t CPU::ADC()
{
	return uint8_t();
}

uint8_t CPU::AND()
{
	return uint8_t();
}

uint8_t CPU::ASL()
{
	return uint8_t();
}

uint8_t CPU::BCC()
{
	return uint8_t();
}

uint8_t CPU::BCS()
{
	return uint8_t();
}

uint8_t CPU::BEQ()
{
	return uint8_t();
}

uint8_t CPU::BIT()
{
	return uint8_t();
}

uint8_t CPU::BMI()
{
	return uint8_t();
}

uint8_t CPU::BNE()
{
	return uint8_t();
}

uint8_t CPU::BPL()
{
	return uint8_t();
}

uint8_t CPU::BRK()
{
	return uint8_t();
}

uint8_t CPU::BVC()
{
	return uint8_t();
}

uint8_t CPU::BVS()
{
	return uint8_t();
}

uint8_t CPU::CLC()
{
	return uint8_t();
}

uint8_t CPU::CLD()
{
	return uint8_t();
}

uint8_t CPU::CLI()
{
	return uint8_t();
}

uint8_t CPU::CLV()
{
	return uint8_t();
}

uint8_t CPU::CMP()
{
	return uint8_t();
}

uint8_t CPU::CPX()
{
	return uint8_t();
}

uint8_t CPU::CPY()
{
	return uint8_t();
}

uint8_t CPU::DEC()
{
	return uint8_t();
}

uint8_t CPU::DEX()
{
	return uint8_t();
}

uint8_t CPU::DEY()
{
	return uint8_t();
}

uint8_t CPU::EOR()
{
	return uint8_t();
}

uint8_t CPU::INC()
{
	return uint8_t();
}

uint8_t CPU::INX()
{
	return uint8_t();
}

uint8_t CPU::INY()
{
	return uint8_t();
}

uint8_t CPU::JMP()
{
	return uint8_t();
}

uint8_t CPU::JSR()
{
	return uint8_t();
}

uint8_t CPU::LDA()
{
	return uint8_t();
}

uint8_t CPU::LDX()
{
	return uint8_t();
}

uint8_t CPU::LDY()
{
	return uint8_t();
}

uint8_t CPU::LSR()
{
	return uint8_t();
}

uint8_t CPU::NOP()
{
	return uint8_t();
}

uint8_t CPU::ORA()
{
	return uint8_t();
}

uint8_t CPU::PHA()
{
	return uint8_t();
}

uint8_t CPU::PHP()
{
	return uint8_t();
}

uint8_t CPU::PLA()
{
	return uint8_t();
}

uint8_t CPU::PLP()
{
	return uint8_t();
}

uint8_t CPU::ROL()
{
	return uint8_t();
}

uint8_t CPU::ROR()
{
	return uint8_t();
}

uint8_t CPU::RTI()
{
	return uint8_t();
}

uint8_t CPU::RTS()
{
	return uint8_t();
}

uint8_t CPU::SBC()
{
	return uint8_t();
}

uint8_t CPU::SEC()
{
	return uint8_t();
}

uint8_t CPU::SED()
{
	return uint8_t();
}

uint8_t CPU::SEI()
{
	return uint8_t();
}

uint8_t CPU::STA()
{
	return uint8_t();
}

uint8_t CPU::STX()
{
	return uint8_t();
}

uint8_t CPU::STY()
{
	return uint8_t();
}

uint8_t CPU::TAX()
{
	return uint8_t();
}

uint8_t CPU::TAY()
{
	return uint8_t();
}

uint8_t CPU::TSX()
{
	return uint8_t();
}

uint8_t CPU::TXA()
{
	return uint8_t();
}

uint8_t CPU::TXS()
{
	return uint8_t();
}

uint8_t CPU::TYA()
{
	return uint8_t();
}
