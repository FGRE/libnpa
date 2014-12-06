namespace Nsb
{
    enum Request
    {
        STOP,
        PLAY,
        DISUSED,
        ADD_RENDER,
        LOCK,
        ERASE,
        UN_CLICKABLE,
        UN_LOCK,
        START,
        ENTER,
        PASSIVE,
        SMOOTHING,
        NO_LOG,
        PUSH_TEXT,
        NO_ICON,
        HIDEABLE,
        PAUSE,
        RESUME,
        ENTRUST_SUSPEND,
        COMPULSORY_SUSPEND,
        SUB_RENDER
    };

    enum Tempo
    {
        AXL_1,
        AXL_2,
        AXL_3,
        DXL_1,
        DXL_2,
        DXL_3,
        AXL_DXL,
        DXL_AXL,
        AXL_AUTO,
        DXL_AUTO
    };

    bool IsValidConstant(const std::string& String);
    int32_t ConstantToValue(const std::string& String);
}
