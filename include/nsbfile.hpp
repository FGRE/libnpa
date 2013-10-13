#ifndef NSB_FILE_HPP
#define NSB_FILE_HPP

#include <string>
#include <vector>

#define SWAP_UINT16(x) (((x) >> 8) | ((x) << 8))

enum OpenMode
{
    NSB_COMPILED    = 1,
    NSB_PARSED      = 2
};

enum /* Magic */
{
    MAGIC_TEXT  = SWAP_UINT16(0xD800), // XML formatted text/voice
    MAGIC_PARAM = SWAP_UINT16(0xD000), // STRING/INT
    MAGIC_BEGIN = SWAP_UINT16(0x9400), // Begining of .nsb
    MAGIC_IF    = SWAP_UINT16(0x9800), // label.if
    MAGIC_ENDIF = SWAP_UINT16(0xCF00), // label.if.end
    MAGIC_CALL  = SWAP_UINT16(0xCE00), // Function call

    // Magic-only
    MAGIC_UNK1  = SWAP_UINT16(0xC600),
    MAGIC_UNK2  = SWAP_UINT16(0xC700),
    MAGIC_UNK3  = SWAP_UINT16(0xC800),
    MAGIC_UNK4  = SWAP_UINT16(0xC900),
    MAGIC_UNK5  = SWAP_UINT16(0x6800),
    MAGIC_UNK6  = SWAP_UINT16(0x8E00),
    MAGIC_UNK7  = SWAP_UINT16(0x5B00),
    MAGIC_UNK8  = SWAP_UINT16(0xC100),
    MAGIC_UNK9  = SWAP_UINT16(0xA500),

    MAGIC_UNK10 = SWAP_UINT16(0xB000), // $GameName/$GameContinue
    MAGIC_UNK11 = SWAP_UINT16(0xD100), // $GameStart/#SYSTEM
    MAGIC_UNK12 = SWAP_UINT16(0xD400),
    MAGIC_UNK13 = SWAP_UINT16(0x9500), // scene.sg00_01.nss/scene.sg00_01.nss_MAIN
    MAGIC_UNK14 = SWAP_UINT16(0xA300), // $MainGameName
    MAGIC_UNK15 = SWAP_UINT16(0xD500), // scene.sg00_01.nss_MAIN
    MAGIC_UNK16 = SWAP_UINT16(0x2000),
    MAGIC_UNK17 = SWAP_UINT16(0x2300),
    MAGIC_UNK18 = SWAP_UINT16(0x2400),
    MAGIC_UNK19 = SWAP_UINT16(0x2500),
    MAGIC_UNK20 = SWAP_UINT16(0x3900),
    MAGIC_UNK21 = SWAP_UINT16(0x3A00),
    MAGIC_UNK22 = SWAP_UINT16(0x9100),
    MAGIC_UNK23 = SWAP_UINT16(0xD200),
    MAGIC_UNK24 = SWAP_UINT16(0x4300),
    MAGIC_UNK25 = SWAP_UINT16(0x2800),
    MAGIC_UNK26 = SWAP_UINT16(0xDE00),
    MAGIC_UNK27 = SWAP_UINT16(0xE400),
    MAGIC_UNK28 = SWAP_UINT16(0x1400),
    MAGIC_UNK29 = SWAP_UINT16(0x2B00),
    MAGIC_UNK30 = SWAP_UINT16(0x1200),
    MAGIC_UNK31 = SWAP_UINT16(0x2C00),
    MAGIC_UNK32 = SWAP_UINT16(0x2D00),
    MAGIC_UNK33 = SWAP_UINT16(0x1B00),
    MAGIC_UNK34 = SWAP_UINT16(0x3000),
    MAGIC_UNK35 = SWAP_UINT16(0xC000),
    MAGIC_UNK36 = SWAP_UINT16(0x9700),
    MAGIC_UNK37 = SWAP_UINT16(0xD700), // Ending of .nsb
};

struct Line
{
    uint16_t Magic;
    std::vector<std::string> Params;
};

class NsbFile
{
public:
	NsbFile(const std::string& Name, OpenMode Mode);

    static const char* StringifyMagic(uint16_t Magic);
    Line* GetNextLine();
    std::string GetName();

private:
    void ReadFromSource();
    void ReadFromBinary();

    uint32_t SourceIter;
    std::vector<Line> Source;
    std::string Name;
};

#endif
