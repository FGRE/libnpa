#ifndef NSB_CONSTANTS_H
#define NSB_CONSTANTS_H

#include <string>
#include <cstdint>
#include <map>
using namespace std;

namespace Nsb
{
	namespace Constant
	{
		bool IsValidConstant(const string& String);
	}

    enum PhoneMail
    {
        //...
    };

    enum ePhoneMode
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
        PHONE_MODE_DIALOG_SEND_MAIL_EDIT,
        PHONE_MODE_ADDRESS_CONFIRM_MAIL,
        PHONE_MODE_ADDRESS_CONFIRM_DIAL,
        PHONE_MODE_POWER_OFF,

        NUM_MODES
    };

    enum PhID
    {
        PHID_MAY,
        PHID_DAR,
        PHID_RUK,
        PHID_OVEN,
        PHID_MOE,
        PHID_JOHN,
        PHID_CRS_0,
        PHID_CRS,
        PHID_SUZ,
        PHID_FEI,
        PHID_MASTER,
        PHID_OKA,
        PHID_SUZ_0
    };

    enum Null
    {
        NSB_NULL = -1
    };

    enum Boolean
    {
        NSB_TRUE = 1,
        NSB_FALSE = 0
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

    enum Color
    {
        BLACK = 0xFF000000,
        WHITE = 0xFFFFFFFF,
        BLUE = 0xFF0000FF,
        RED = 0xFFFF0000,
        GREEN = 0xFF00FF00
    };

    enum Tone
    {
        PLAIN,
        NEGA_POSI,
        MONOCHROME,
        SEPIA,
        KITANO_BLUE,
        DROP,
        RAIN
    };

    template <class T>
    struct ConstantHolder
    {
        static map<string, int32_t> Constants;
    };

    template <class T>
    bool IsValidConstant(const string& String)
    {
        return ConstantHolder<T>::Constants.find(String) != ConstantHolder<T>::Constants.end();
    }

	// non template version
	bool IsValidConstant(const string& String);

    template <class T>
    int32_t ConstantToValue(const string& String)
    {
        auto iter = ConstantHolder<T>::Constants.find(String);
        if (iter != ConstantHolder<T>::Constants.end())
            return iter->second;
        return NSB_NULL;
    }
}

#endif
