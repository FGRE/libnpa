#ifndef NSB_MAGIC_HPP
#define NSB_MAGIC_HPP

#define SWAP_UINT16(x) (((x) >> 8) | ((x) << 8))

// TODO: Sort by value
enum /* Magic */
{
    MAGIC_TEXT  = SWAP_UINT16(0xD800), // XML formatted text/voice
    MAGIC_PARAM = SWAP_UINT16(0xD000), // STRING/INT
    MAGIC_UNK0  = SWAP_UINT16(0x9400), // Begin chapter
    MAGIC_IF    = SWAP_UINT16(0x9800), // label.if (not if, but label, also below)
    MAGIC_ENDIF = SWAP_UINT16(0xCF00), // label.if.end
    MAGIC_CALL  = SWAP_UINT16(0xCE00), // Script-defined function call

    // Magic-only
    MAGIC_UNK1  = SWAP_UINT16(0xC600), // Logical condition
    MAGIC_UNK2  = SWAP_UINT16(0xC700), // Logical condition
    MAGIC_UNK3  = SWAP_UINT16(0xC800), // label epilogue?
    MAGIC_UNK4  = SWAP_UINT16(0xC900), // Logical == ?
    MAGIC_UNK5  = SWAP_UINT16(0x6800), // Empty variable/parameter?
    MAGIC_UNK6  = SWAP_UINT16(0x8E00), // Flush commands? (return control to game) Cleanup params? (epilogue)
    MAGIC_UNK7  = SWAP_UINT16(0x5B00),
    MAGIC_UNK8  = SWAP_UINT16(0xC100),
    MAGIC_CONCAT = SWAP_UINT16(0xA500), // Concats above two params into a param

    MAGIC_SET   = SWAP_UINT16(0xB000), // Set variable value? $GameName/$GameContinue
    MAGIC_GET   = SWAP_UINT16(0xD100), // Special parameter (request for var?) $GameStart/$MOVIEDATA
    MAGIC_UNK12 = SWAP_UINT16(0xD400), // End chapter
    MAGIC_UNK13 = SWAP_UINT16(0x9500), // Begin scene
    MAGIC_UNK14 = SWAP_UINT16(0xA300), // $MainGameName
    MAGIC_UNK15 = SWAP_UINT16(0xD500), // End scene
    MAGIC_START_ANIMATION = SWAP_UINT16(0x2000), // AnimationStart?
    MAGIC_SET_DISPLAY_STATE = SWAP_UINT16(0x2300), // TODO: rename to SetState() Sets movie/audio (Play, Start, Pause, Resume)/BGBUF (AddRender, Passive) state
    MAGIC_DESTROY = SWAP_UINT16(0x2400), // Deallocate/Destroy/Remove
    MAGIC_DISPLAY = SWAP_UINT16(0x2500), // flushes buffer? (shows image/movie on screen?)
    MAGIC_UNK20 = SWAP_UINT16(0x3900), // wait?? - Deprecated/broken!
    MAGIC_SLEEP_MS = SWAP_UINT16(0x3A00), // wait (milisecond)??
    MAGIC_SET_NULL_PARAM = SWAP_UINT16(0x9100), // nullify parameter?
    MAGIC_UNK23 = SWAP_UINT16(0xD200),
    MAGIC_SET_AUDIO_STATE = SWAP_UINT16(0x4300), // SetAudioState(STRING handle, INT num_seconds, INT volume, STRING tempo);
    MAGIC_UNK25 = SWAP_UINT16(0x2800),
    MAGIC_UNK26 = SWAP_UINT16(0xDE00), // Retrieve from array (STRING array, INT index) ??
    MAGIC_UNK27 = SWAP_UINT16(0xE400),
    MAGIC_UNK28 = SWAP_UINT16(0x1400),
    MAGIC_UNK29 = SWAP_UINT16(0x2B00), // Local to global?
    MAGIC_LOAD_MOVIE = SWAP_UINT16(0x1200), // Load movie??
    MAGIC_UNK31 = SWAP_UINT16(0x2C00),
    MAGIC_UNK32 = SWAP_UINT16(0x2D00),
    MAGIC_UNK33 = SWAP_UINT16(0x1B00),
    MAGIC_UNK34 = SWAP_UINT16(0x3000),
    MAGIC_UNK35 = SWAP_UINT16(0xC000),
    MAGIC_BEGIN = SWAP_UINT16(0x9700), // Begin function
    MAGIC_END   = SWAP_UINT16(0xD700), // End function
    MAGIC_UNK38 = SWAP_UINT16(0x0401),
    MAGIC_UNK39 = SWAP_UINT16(0x0501),
    MAGIC_UNK40 = SWAP_UINT16(0x7500),
    MAGIC_LOAD_TEXTURE = SWAP_UINT16(0x0D00), // LoadImage/Texture?
    MAGIC_UNK42 = SWAP_UINT16(0x6100),
    MAGIC_UNK43 = SWAP_UINT16(0xA800),
    MAGIC_UNK44 = SWAP_UINT16(0xA700),
    MAGIC_UNK45 = SWAP_UINT16(0xAC00), // Negate(!) previous parameter?
    MAGIC_UNK46 = SWAP_UINT16(0x9900),
    MAGIC_GET_MOVIE_TIME = SWAP_UINT16(0x5E00), // GetMovieDuration/TimeLeft??
    MAGIC_UNK48 = SWAP_UINT16(0x9D00),
    MAGIC_UNK49 = SWAP_UINT16(0xC200),
    MAGIC_UNK50 = SWAP_UINT16(0x0B00),
    MAGIC_UNK51 = SWAP_UINT16(0x0700),
    MAGIC_UNK52 = SWAP_UINT16(0x2F00),
    MAGIC_UNK53 = SWAP_UINT16(0xFC00),
    MAGIC_UNK54 = SWAP_UINT16(0xBE00),
    MAGIC_UNK55 = SWAP_UINT16(0xC400),
    MAGIC_UNK56 = SWAP_UINT16(0x3800),
    MAGIC_UNK57 = SWAP_UINT16(0xC300),
    MAGIC_UNK58 = SWAP_UINT16(0xA600),
    MAGIC_UNK59 = SWAP_UINT16(0xEB00), // FM_From_DAR0203 (.ogg)
    MAGIC_UNK60 = SWAP_UINT16(0xEA00),
    MAGIC_UNK61 = SWAP_UINT16(0xB100),
    MAGIC_UNK62 = SWAP_UINT16(0xFB00),
    MAGIC_UNK63 = SWAP_UINT16(0xB200),
    MAGIC_UNK64 = SWAP_UINT16(0xC500),
    MAGIC_UNK65 = SWAP_UINT16(0xDF00),
    MAGIC_UNK66 = SWAP_UINT16(0x7A00),
    MAGIC_UNK67 = SWAP_UINT16(0x1C00),
    MAGIC_UNK68 = SWAP_UINT16(0x1900),
    MAGIC_UNK69 = SWAP_UINT16(0x7200), // create array? variable # of args...see: st, achievements...
    MAGIC_UNK70 = SWAP_UINT16(0x7300), // called right after 69, same params
    MAGIC_UNK71 = SWAP_UINT16(0x1600),
    MAGIC_UNK72 = SWAP_UINT16(0x6200),
    MAGIC_UNK73 = SWAP_UINT16(0x7400),
    MAGIC_SET_FONT_ATTRIBUTES = SWAP_UINT16(0x4E00), // SetFontAttributes(STRING font, INT size, STRING color1, STRING color2, INT unk, STRING unk)
    MAGIC_UNK75 = SWAP_UINT16(0x1300),
    MAGIC_CREATE_COLOR = SWAP_UINT16(0x0A00), // CreateColor(STRING handle, INT priority, INT unk, INT unk, INT width, INT height, STRING color);
    MAGIC_UNK77 = SWAP_UINT16(0x0800),
    MAGIC_UNK78 = SWAP_UINT16(0x1800),
    MAGIC_UNK79 = SWAP_UINT16(0x3500),
    MAGIC_UNK80 = SWAP_UINT16(0x1C01),
    MAGIC_UNK81 = SWAP_UINT16(0xA900),
    MAGIC_UNK82 = SWAP_UINT16(0x6E00),
    MAGIC_UNK83 = SWAP_UINT16(0x6500),
    MAGIC_UNK84 = SWAP_UINT16(0x8A00),
    MAGIC_UNK85 = SWAP_UINT16(0x9A00),
    MAGIC_CALL_SCRIPT = SWAP_UINT16(0xA200), // has path to nss/somefile.nsb as param
    MAGIC_UNK87 = SWAP_UINT16(0x9B00),
    MAGIC_UNK88 = SWAP_UINT16(0xDD00),
    MAGIC_UNK89 = SWAP_UINT16(0xE000),
    MAGIC_UNK90 = SWAP_UINT16(0x0101),
    MAGIC_UNK91 = SWAP_UINT16(0x5400),
    MAGIC_UNK92 = SWAP_UINT16(0x5500),
    MAGIC_UNK93 = SWAP_UINT16(0x6B00),
    MAGIC_UNK94 = SWAP_UINT16(0x0001),
    MAGIC_UNK95 = SWAP_UINT16(0x0900),
    MAGIC_UNK96 = SWAP_UINT16(0x2600),
    MAGIC_UNK97 = SWAP_UINT16(0x6C00),
    MAGIC_UNK98 = SWAP_UINT16(0x3400),
    MAGIC_UNK99 = SWAP_UINT16(0x9C00),
    MAGIC_UNK100 = SWAP_UINT16(0xF700),
    MAGIC_UNK101 = SWAP_UINT16(0xF800),
    MAGIC_CALLBACK = SWAP_UINT16(0x5800),
    MAGIC_UNK103 = SWAP_UINT16(0x4F00),
    MAGIC_UNK104 = SWAP_UINT16(0x5100),
    MAGIC_UNK105 = SWAP_UINT16(0x5F00),
    MAGIC_UNK106 = SWAP_UINT16(0x4C00),
    MAGIC_UNK107 = SWAP_UINT16(0x4A00),
    MAGIC_UNK108 = SWAP_UINT16(0x5200),
    MAGIC_UNK109 = SWAP_UINT16(0x4000),
    MAGIC_UNK110 = SWAP_UINT16(0xBF00),
    MAGIC_UNK111 = SWAP_UINT16(0x4B00),
    MAGIC_UNK112 = SWAP_UINT16(0x3200),
    MAGIC_UNK113 = SWAP_UINT16(0x4100),
    MAGIC_UNK114 = SWAP_UINT16(0x5C00),
    MAGIC_UNK115 = SWAP_UINT16(0x1801),
    MAGIC_UNK116 = SWAP_UINT16(0xAA00),
    MAGIC_UNK117 = SWAP_UINT16(0xE800),
    MAGIC_UNK118 = SWAP_UINT16(0x1301),
    MAGIC_UNK119 = SWAP_UINT16(0x1D01),
    MAGIC_UNK120 = SWAP_UINT16(0x1201),
    MAGIC_UNK121 = SWAP_UINT16(0x1101),
    MAGIC_UNK122 = SWAP_UINT16(0xF100),
    MAGIC_UNK123 = SWAP_UINT16(0x1601),
    MAGIC_UNK124 = SWAP_UINT16(0xE500),
    MAGIC_UNK125 = SWAP_UINT16(0xE600),
    MAGIC_UNK126 = SWAP_UINT16(0xF600),
    MAGIC_UNK127 = SWAP_UINT16(0xED00),
    MAGIC_UNK128 = SWAP_UINT16(0xFA00),
    MAGIC_UNK129 = SWAP_UINT16(0x0F01),
    MAGIC_UNK130 = SWAP_UINT16(0xE700),
    MAGIC_UNK131 = SWAP_UINT16(0x5D00),
    MAGIC_UNK132 = SWAP_UINT16(0xE900),
    MAGIC_UNK133 = SWAP_UINT16(0x0C01),
    MAGIC_UNK134 = SWAP_UINT16(0x0D01),
    MAGIC_UNK135 = SWAP_UINT16(0x1701),
    MAGIC_UNK136 = SWAP_UINT16(0x0701),
    MAGIC_UNK137 = SWAP_UINT16(0xF300),
    MAGIC_UNK138 = SWAP_UINT16(0xF000),
    MAGIC_UNK139 = SWAP_UINT16(0x1901),
    MAGIC_UNK140 = SWAP_UINT16(0x1001),
    MAGIC_UNK141 = SWAP_UINT16(0xF500),
    MAGIC_UNK142 = SWAP_UINT16(0xEC00),
    MAGIC_UNK143 = SWAP_UINT16(0xF900),
    MAGIC_UNK144 = SWAP_UINT16(0x0A01),
    MAGIC_UNK145 = SWAP_UINT16(0xFE00),
    MAGIC_UNK146 = SWAP_UINT16(0xAE00),
    MAGIC_UNK147 = SWAP_UINT16(0x1401),
    MAGIC_UNK148 = SWAP_UINT16(0x1501),
    MAGIC_UNK149 = SWAP_UINT16(0xB900),
    MAGIC_UNK150 = SWAP_UINT16(0xAB00),
    MAGIC_UNK151 = SWAP_UINT16(0x0E00),
    MAGIC_UNK152 = SWAP_UINT16(0x0600), // LoadSt($nut, $立画優先度, $StPlaceX, $StPlaceY, 128, $St画像, false);
    MAGIC_UNK153 = SWAP_UINT16(0x0F00),
    MAGIC_UNK154 = SWAP_UINT16(0x3100),
    MAGIC_LOAD_AUDIO = SWAP_UINT16(0x1100), // LoadAudio(STRING handle, STRING type, STRING file); type is SE/BGM/VOICE
    MAGIC_SET_AUDIO_RANGE = SWAP_UINT16(0x4700), // SetAudioPlayRange(STRING handle, INT start_ms, INT end_ms);
    MAGIC_UNK157 = SWAP_UINT16(0x4500), // Audio related (STRING handle, INT unk, INT speed, STRING unk)
    MAGIC_UNK158 = SWAP_UINT16(0x4400), // Audio related (STRING handle, INT unk, STRING direction, STRING unk)
    MAGIC_SET_AUDIO_LOOP = SWAP_UINT16(0x4600), // SetAudioLoop(STRING handle, BOOL loop)
    MAGIC_UNK160 = SWAP_UINT16(0x3700),
    MAGIC_UNK161 = SWAP_UINT16(0x7000),
    MAGIC_UNK162 = SWAP_UINT16(0x6400),
    MAGIC_SET_TEXTBOX_ATTRIBUTES = SWAP_UINT16(0xE01), // SetTextboxAttributes(STRING handle, INT unk, STRING font, INT unk, STRING color1, STRING color2, INT unk, STRING unk)
    MAGIC_UNK164 = SWAP_UINT16(0x1D00),
    MAGIC_UNK165 = SWAP_UINT16(0x4200),
    MAGIC_UNK166 = SWAP_UINT16(0x4900),
    MAGIC_UNK167 = SWAP_UINT16(0x5900),
    MAGIC_UNK168 = SWAP_UINT16(0x6000),
    MAGIC_UNK169 = SWAP_UINT16(0x300),
    MAGIC_UNK170 = SWAP_UINT16(0x3C00),
    MAGIC_UNK171 = SWAP_UINT16(0x5300),
    MAGIC_UNK172 = SWAP_UINT16(0x6F00),
    MAGIC_UNKMAX = 173
};

#endif
