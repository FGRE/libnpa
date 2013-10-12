#ifndef NSB_FILE_HPP
#define NSB_FILE_HPP

#include <string>

#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

enum OpenMode
{
    NSB_COMPILED    = 1,
    NSB_PARSED      = 2
};

enum /* Magic */
{
    MAGIC_TEXT  = SWAP_UINT32(0xD8000300), // XML formatted text
    MAGIC_DATA  = SWAP_UINT32(0xD0000200), // STRING/INT
    MAGIC_BEGIN = SWAP_UINT32(0x94000100), // Begining of .nsb
    MAGIC_IF    = SWAP_UINT32(0x98000100),
    MAGIC_ENDIF = SWAP_UINT32(0xCF000100),

    MAGIC_UNK1  = SWAP_UINT32(0xCE000100), // SkipOut/GameMainSet/SystemInit/InitSG/SetText/TypeBegin/TextBoxDelete
    MAGIC_UNK2  = SWAP_UINT32(0xCE000200), // SetCharaColor/PhoneAutoOperation/PhoneOperation/PrintBG2
    MAGIC_UNK3  = SWAP_UINT32(0xCE000300), // DeleteAllSt/CreatePlainSP/CreateProcessSG
    MAGIC_UNK4  = SWAP_UINT32(0xCE000400), // FadeSt/DeleteSt/PosSt/BgCopy/BgLoad
    MAGIC_UNK5  = SWAP_UINT32(0xCE000500), // FadeDelete/SoundPlay
    MAGIC_UNK6  = SWAP_UINT32(0xCE000600), // St/ColorOut/SePlay

    MAGIC_UNK7  = SWAP_UINT32(0xB0000100),
    MAGIC_UNK8  = SWAP_UINT32(0xD1000100),

    // Magic-only
    MAGIC_UNK9  = SWAP_UINT32(0xC8000000),

};

class NsbFile
{
public:
	NsbFile(std::string Name, OpenMode Mode);

private:
    void ReadFromSource(std::string& Name);
    void ReadFromBinary(std::string& Name);
};

#endif
