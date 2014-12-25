namespace Nsb
{
    enum PhoneMode
    {
        PHONE_MODE_DEFAULT,
        PHONE_MODE_MAIL_MENU,
        PHONE_MODE_SEND_BOX,
        PHONE_MODE_RECEIVE_BOX,
        PHONE_MODE_SEND_MAIL,
        PHONE_MODE_RECEIVED_NEW_MAIL,
        PHONE_MODE_ADDRESS_BOOK,
        PHONE_MODE_ADDRESS_CONFIRM,
        PHONE_MODE_SENDING,
        PHONE_MODE_RINGING,
        PHONE_MODE_ENGAGE_NORMAL,
        PHONE_MODE_ENGAGE_VISUAL,
        PHONE_MODE_OPTION,
        PHONE_MODE_CHANGE_WALLPAPER,
        PHONE_MODE_CHANGE_RINGING_MELODY,
        PHONE_MODE_CHANGE_RECEIVEING_MELODY,
        PHONE_MODE_CHANGE_RINGING_VIBRATION,
        PHONE_MODE_CHANGE_RECEIVING_VIBRATION,
        PHONE_MODE_MAIL_SUB_MENU,
        PHONE_MODE_DEFAULT_OPERATABLE,
        PHONE_MODE_SEND_MAIL_EDIT,
        PHONE_MODE_DIALING_OPERATABLE,
        PHONE_MODE_COMPLETE_SEND_MAIL,
        PHONE_MODE_RECEIVING_MAIL,
        PHONE_MODE_COMPLETE_RECEIVE_MAIL,
        PHONE_MODE_END,
        PHONE_MODE_RECEIVED_MAIL,
        PHONE_MODE_CALLING,
        PHONE_MODE_DIALOG_SEND_MAILEDIT,
        PHONE_MODE_ADDRESS_CONFIRM_MAIL,
        PHONE_MODE_ADDRESS_CONFIRM_DIAL,
        PHONE_MODE_POWER_OFF
    };

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
        SUB_RENDER,
        FADE_ENTER,
        INCREMENT,
        DECREMENT,
        NEXT,
        PREV,
        TYPING,
        CLICKABLE,
        MUL_RENDER,
        OVERLAY_RENDER,
        NORMAL_RENDER,
        OPEN
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

    enum Shade
    {
        EXTRALIGHT,
        LIGHT,
        MEDIUM,
        HEAVY,
        SEMIHEAVY,
        EXTRAHEAVY,
        NOMORE
    };

    bool IsValidConstant(const std::string& String);
    int32_t ConstantToValue(const std::string& String);
}
