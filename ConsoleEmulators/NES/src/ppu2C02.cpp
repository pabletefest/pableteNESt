#include "ppu2C02.h"

namespace nes
{
    constexpr uint16_t baseNameTableAddresses[4] = { 0x2000, 0x2400, 0x2800, 0x2C00 };

    PPU::PPU()
        : pixelsFrameBufer(std::vector<Pixel>(341 * 262, Pixel()))
    {
        init();

        nesPalToRGBAPalArray[0x00] = Pixel{ 255, 84, 84, 84 };
        nesPalToRGBAPalArray[0x01] = Pixel{ 255, 0, 30, 116 };
        nesPalToRGBAPalArray[0x02] = Pixel{ 255, 8, 16, 144 };
        nesPalToRGBAPalArray[0x03] = Pixel{ 255, 48, 0, 136 };
        nesPalToRGBAPalArray[0x04] = Pixel{ 255, 68, 0, 100 };
        nesPalToRGBAPalArray[0x05] = Pixel{ 255, 92, 0, 48 };
        nesPalToRGBAPalArray[0x06] = Pixel{ 255, 84, 4, 0 };
        nesPalToRGBAPalArray[0x07] = Pixel{ 255, 60, 24, 0 };
        nesPalToRGBAPalArray[0x08] = Pixel{ 255, 32, 42, 0 };
        nesPalToRGBAPalArray[0x09] = Pixel{ 255, 8, 58, 0 };
        nesPalToRGBAPalArray[0x0A] = Pixel{ 255, 0, 64, 0 };
        nesPalToRGBAPalArray[0x0B] = Pixel{ 255, 0, 60, 0 };
        nesPalToRGBAPalArray[0x0C] = Pixel{ 255, 0, 50, 60 };
        nesPalToRGBAPalArray[0x0D] = Pixel{ 255, 0, 0, 0 };
        nesPalToRGBAPalArray[0x0E] = Pixel{ 255, 0, 0, 0 };
        nesPalToRGBAPalArray[0x0F] = Pixel{ 255, 0, 0, 0 };

        nesPalToRGBAPalArray[0x10] = Pixel{ 255, 152, 150, 152 };
        nesPalToRGBAPalArray[0x11] = Pixel{ 255, 8, 76, 196 };
        nesPalToRGBAPalArray[0x12] = Pixel{ 255, 48, 50, 236 };
        nesPalToRGBAPalArray[0x13] = Pixel{ 255, 92, 30, 228 };
        nesPalToRGBAPalArray[0x14] = Pixel{ 255, 136, 20, 176 };
        nesPalToRGBAPalArray[0x15] = Pixel{ 255, 160, 20, 100 };
        nesPalToRGBAPalArray[0x16] = Pixel{ 255, 152, 34, 32 };
        nesPalToRGBAPalArray[0x17] = Pixel{ 255, 120, 60, 0 };
        nesPalToRGBAPalArray[0x18] = Pixel{ 255, 84, 90, 0 };
        nesPalToRGBAPalArray[0x19] = Pixel{ 255, 40, 114, 0 };
        nesPalToRGBAPalArray[0x1A] = Pixel{ 255, 8, 124, 0 };
        nesPalToRGBAPalArray[0x1B] = Pixel{ 255, 0, 118, 40 };
        nesPalToRGBAPalArray[0x1C] = Pixel{ 255, 0, 102, 120 };
        nesPalToRGBAPalArray[0x1D] = Pixel{ 255, 0, 0, 0 };
        nesPalToRGBAPalArray[0x1E] = Pixel{ 255, 0, 0, 0 };
        nesPalToRGBAPalArray[0x1F] = Pixel{ 255, 0, 0, 0 };

        nesPalToRGBAPalArray[0x20] = Pixel{ 255, 236, 238, 236 };
        nesPalToRGBAPalArray[0x21] = Pixel{ 255, 76, 154, 236 };
        nesPalToRGBAPalArray[0x22] = Pixel{ 255, 120, 124, 236 };
        nesPalToRGBAPalArray[0x23] = Pixel{ 255, 176, 98, 236 };
        nesPalToRGBAPalArray[0x24] = Pixel{ 255, 228, 84, 236 };
        nesPalToRGBAPalArray[0x25] = Pixel{ 255, 236, 88, 180 };
        nesPalToRGBAPalArray[0x26] = Pixel{ 255, 236, 106, 100 };
        nesPalToRGBAPalArray[0x27] = Pixel{ 255, 212, 136, 32 };
        nesPalToRGBAPalArray[0x28] = Pixel{ 255, 160, 170, 0 };
        nesPalToRGBAPalArray[0x29] = Pixel{ 255, 116, 196, 0 };
        nesPalToRGBAPalArray[0x2A] = Pixel{ 255, 76, 208, 32 };
        nesPalToRGBAPalArray[0x2B] = Pixel{ 255, 56, 204, 108 };
        nesPalToRGBAPalArray[0x2C] = Pixel{ 255, 56, 180, 204 };
        nesPalToRGBAPalArray[0x2D] = Pixel{ 255, 60, 60, 60 };
        nesPalToRGBAPalArray[0x2E] = Pixel{ 255, 0, 0, 0 };
        nesPalToRGBAPalArray[0x2F] = Pixel{ 255, 0, 0, 0 };

        nesPalToRGBAPalArray[0x30] = Pixel{ 255, 236, 238, 236 };
        nesPalToRGBAPalArray[0x31] = Pixel{ 255, 168, 204, 236 };
        nesPalToRGBAPalArray[0x32] = Pixel{ 255, 188, 188, 236 };
        nesPalToRGBAPalArray[0x33] = Pixel{ 255, 212, 178, 236 };
        nesPalToRGBAPalArray[0x34] = Pixel{ 255, 236, 174, 236 };
        nesPalToRGBAPalArray[0x35] = Pixel{ 255, 236, 174, 212 };
        nesPalToRGBAPalArray[0x36] = Pixel{ 255, 236, 180, 176 };
        nesPalToRGBAPalArray[0x37] = Pixel{ 255, 228, 196, 144 };
        nesPalToRGBAPalArray[0x38] = Pixel{ 255, 204, 210, 120 };
        nesPalToRGBAPalArray[0x39] = Pixel{ 255, 180, 222, 120 };
        nesPalToRGBAPalArray[0x3A] = Pixel{ 255, 168, 226, 144 };
        nesPalToRGBAPalArray[0x3B] = Pixel{ 255, 152, 226, 180 };
        nesPalToRGBAPalArray[0x3C] = Pixel{ 255, 160, 214, 228 };
        nesPalToRGBAPalArray[0x3D] = Pixel{ 255, 160, 162, 160 };
        nesPalToRGBAPalArray[0x3E] = Pixel{ 255, 0, 0, 0 };
        nesPalToRGBAPalArray[0x3F] = Pixel{ 255, 0, 0, 0 };
    }

    void PPU::connectCartridge(const std::shared_ptr<Cartridge>& cart)
    {
        this->cartridge = cart;
    }

    void PPU::reset()
    {
        PPUCTRL.controlReg = 0x00;
        PPUMASK.maskReg = 0x00;
    }

    void PPU::clock()
    {
        //if (!PPUMASK.showBackground && !PPUMASK.showSprites) // Rendering disabled
        //    return;

        if (cycle == 1 && scanline == -1)
        {
            PPUSTATUS.verticalBlank = 0;
            PPUSTATUS.spriteZeroHit = 0;
            PPUSTATUS.spriteOverflow = 0;
        }

        if (cycle == 1 && scanline == 241)
        {
            PPUSTATUS.verticalBlank = 1;

            if (PPUCTRL.enableNMI)
                nmi = true;
        }


        // ----- TESTING PORPOSE -----
        if (scanline == -1) scanline++;

        pixelsFrameBufer[scanline * 341 + cycle] = nesPalToRGBAPalArray[rand() % 0x3F];

        // ----- END OF TESTING ------

        cycle++;

        if (cycle >= 341)
        {
            cycle = 0;
            scanline++;

            if (scanline >= 261)
            {
                scanline = -1;
                frameCompleted = true;
                framesElapsed++;
            }
        }

        totalCycles++;
    }

    uint8_t PPU::cpuRead(uint16_t address)
    {
        uint8_t dataRead = 0x00;

        switch (address)
        {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            dataRead = PPUSTATUS.statusReg;
            //dataRedd = (PPUSTATUS.statusReg & 0xE0) | (internalReadBuffer & 0x1F); Put lower 5 bits of noise into the read data, check if needed later
            PPUSTATUS.verticalBlank = 0;
            addressLatchToggle = false; //Reset
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            if (loopyV.vramAddrPtr <= 0x3EFF)
            {
                dataRead = internalReadBuffer;
                internalReadBuffer = ppuRead(loopyV.vramAddrPtr);
            }
            else if (loopyV.vramAddrPtr >= 0x3F00)
            {
                dataRead = ppuRead(loopyV.vramAddrPtr);
            }

            loopyV.vramAddrPtr += PPUCTRL.vramAddrInc ? 32 : 1;
            break;
        }

        return dataRead;
    }

    void PPU::cpuWrite(uint16_t address, uint8_t data)
    {
        switch (address)
        {
        case 0x0000: // Control
            PPUCTRL.controlReg = data;
            loopyT.coarseXScroll = PPUCTRL.nametableX;
            loopyT.coarseYScroll = PPUCTRL.nametableY;
            break;
        case 0x0001: // Mask
            PPUMASK.maskReg = data;
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            if (!addressLatchToggle) // XXXXXxxx, Top 5 bits become XXXXX of "T", low 3 bits become the fine X scroll(updated immediately).
            {
                loopyT.coarseXScroll = ((data & 0xF8) >> 3) & 0x001F; // Top 5 bits of data become the lower 5 bits of T
                fineXScroll = data & 0x07; // Low 3 bits of data become fine X scroll
            }
            else // YYYYYyyy, Top 5 bits become YYYYY of "T", low 3 bits become yyy of "T" (fine Y scroll)
            {
                loopyT.coarseYScroll = ((data & 0xF8) >> 3) & 0x001F;
                loopyT.fineYScroll = data & 0x07;
            }

            addressLatchToggle = !addressLatchToggle;
            break;
        case 0x0006: // PPU Address
            if (!addressLatchToggle) // 00yyNNYY, sets the top 8 bits in "T".The top bit of fine Y scroll gets corrupted to 0 here, so we do a $2005 second write to make up for that
                // We treat the addr as big endian (i.e. hi_byte is provided first)
                loopyT.vramAddrPtr = (data << 8) & 0x3F00; // High bit of fine Y scroll is corrupted to 0. We emulate this hardware behaviour
            else // YYYXXXXX, Sets the low 8 bits of "T", which includes the coarse X, and low 3 bits of coarse y. After doing this, "T" is copied to "V", allowing mid-screen vertical scrolling
            {
                loopyT.vramAddrPtr |= data & 0x00FF;
                loopyV = loopyT;
            }

            addressLatchToggle = !addressLatchToggle; // Invert latch
            break;
        case 0x0007: // PPU Data
            ppuWrite(loopyV.vramAddrPtr, data);
            loopyV.vramAddrPtr += PPUCTRL.vramAddrInc ? 32 : 1;
            break;
        }
    }

    uint8_t PPU::ppuRead(uint16_t address)
    {
        uint8_t dataRead = 0x00;
        address &= 0x3FFF; // Masking address to avoid going beyond the PPU addressable range

        if (cartridge->ppuRead(address, dataRead))
        {

        }
        else if (address >= 0x3F00)
        {
            address &= 0x001F;
            dataRead = paletteRam[address];
        }

        return dataRead;
    }

    void PPU::ppuWrite(uint16_t address, uint8_t data)
    {
        address &= 0x3FFF; // Masking address to avoid going beyond the PPU addressable range

        if (cartridge->ppuWrite(address, data))
        {

        }
        else if (address >= 0x2000 && address <= 0x2FFF)
        {
            if (cartridge->getNTMirroring() == Cartridge::Mirroring::VERTICAL)
            {
                if (address >= 0x2800 && address <= 0x2BFF)
                    address &= 0x23FF;
                else if (address >= 0x2C00 && address <= 0x2FFF)
                    address &= 0x27FF;

            }
            else if (cartridge->getNTMirroring() == Cartridge::Mirroring::HORIZONTAL)
            {
                if (address <= 0x27FF)
                    address &= 0x23FF;
                else if (address >= 0x2C00)
                    address &= 0x2BFF;
            }

            nameTables[(address & 0x0F00) >> 16][address & 0x03FF] = data;
        }
        else if (address >= 0x3F00) // Palette RAM indexes
        {
            address &= 0x001F;

            if ((address & 0x000F) % 0x04 == 0) // Multiples of 4 are mirrors of 0x3F00 (last colour of a palette mirrors to background colour at 0x3F00)
                address &= 0x000F; // Addresses with 0x3F1X multiples of 4 are mirrors of 0x3F0X equivalents (i.e. 10, 14, 18, 1C)

            paletteRam[address] = data;
        }
    }

    PPU::Pixel PPU::getRGBAFromNesPalette(uint8_t paletteIndex, uint8_t pixelIndex)
    {
        uint8_t nesColour = ppuRead(0x3F00 + (paletteIndex * 4 + pixelIndex));
        return nesPalToRGBAPalArray[nesColour];
    }

    std::vector<PPU::Pixel> PPU::getPatternTableBuffer(uint8_t patternIndex, uint8_t paletteIndex)
    {
        std::vector<PPU::Pixel> buffer = std::vector<PPU::Pixel>(128 * 128); // 128 by 128 pixels wide

        // Iterate over sprites indeces
        for (int indexTileY = 0; indexTileY < 16; indexTileY++)
        {
            for (int indexTileX = 0; indexTileX < 16; indexTileX++)
            {
                int tileOffset = indexTileY * 256 + indexTileX * 16;

                // Iterate over sprite rows
                for (int sprRow = 0; sprRow < 8; sprRow++)
                {
                    //Each tile is 16 bytes, each row is 2 bytes separated each one in two blocks of 8 bytes (LSB and MSB, each one separated 8 bytes)
                    uint8_t LSB = ppuRead(patternIndex * 0x1000 + tileOffset + sprRow + 0x0000);
                    uint8_t MSB = ppuRead(patternIndex * 0x1000 + tileOffset + sprRow + 0x0008);

                    // Iterate over sprite column from a row using LSB and MSB
                    for (int sprCol = 0; sprCol < 8; sprCol++)
                    {
                        uint8_t lsb = (LSB & 0x80) >> 7;
                        uint8_t msb = (MSB & 0x80) >> 7;

                        uint8_t pixel = (msb << 1) | lsb;

                        uint16_t x = indexTileX * 8 + (sprCol);
                        uint16_t y = indexTileY * 8 + sprRow;

                        buffer[y * 128 + x] = getRGBAFromNesPalette(0, pixel);

                        MSB <<= 1;
                        LSB <<= 1;
                    }
                }
            }
        }

        return buffer;
    }

    void* PPU::getNametable(int8_t selectNT)
    {
        return nameTables[selectNT];
    }

    void PPU::init()
    {
        PPUCTRL.controlReg = 0x00;
        PPUMASK.maskReg = 0x00;
        PPUSTATUS.statusReg = 0xA0;
    }

}