/*
 * @Description:
 * @Author: MALossov
 * @Date: 2023-02-05 11:01:29
 * @LastEditTime: 2023-02-05 23:53:16
 * @LastEditors: MALossov
 * @Reference:
 */
#include "menu.h"
#include "gpio.h"
#include "main.h"
#include "menu.h"
#include "string.h"
#include "ssd1306.h"
#include "stdio.h"


MenuList Menu;

char* Titles[4] = { "Wave","Freq", "Step","Ampli" };
char* ButtonMsgs[5] = { "SW1_Pressed","SW2_Pressed","Enc_Pressed","Enc_Clock","Enc_AntiClock" };
char STATE[3] = { 'P','W','M' };


//! Menu Item location cal funcs

int Button_x(char* ButtonMsg) {
    int len = strlen(ButtonMsg) * 6;
    return 64 - len / 2;
}

int Title_x(char* Title)
{
    int len = strlen(Title) * 7;
    return TITLE_OFFSET - len;
}

int Index_y(uint8_t row) {
    return row * 11 + 12;
}

//! Menu Item update funcs(Without screen display)

void updateLabels(LabelText Label, uint8_t row)
{
    Label.pos_x = Title_x(Label.Title);
    Label.pos_y = Index_y(row);
    ssd1306_SetCursor(Label.pos_x, Label.pos_y);
    ssd1306_WriteString(Label.Title, Font_7x10, White);
}

void updateButtonMsgs(ButtonMsg* ButtonMsg)
{
    uint8_t pos_x = Button_x(ButtonMsg);

    ssd1306_FillRectangle(0, 54, 128, 64, White);
    ssd1306_SetCursor(pos_x, 55);
    ssd1306_WriteString(ButtonMsg, Font_6x8, Black);
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

}

void updateSTATE(DDS_State DDSSTATE) {
    ssd1306_SetCursor(120, 1);
    ssd1306_WriteChar(STATE[DDSSTATE], Font_7x10, White);
};


void updateStatusChoose(uint8_t row, StatusChoose status) {
    SSD1306_COLOR Color = status == Choose ? White : Black;
    ssd1306_FillCircle(4, Index_y(row) + 4, 3, Color);
    ssd1306_Line(0, Index_y(row) + 10, 128, Index_y(row) + 10, Color);
}

#define APPEND_X 112
#define DATA_X TITLE_OFFSET+3
void updateTheData(DataText Data, uint8_t row) {
    Data.pos_x = DATA_X;
    Data.pos_y = Index_y(row);
    ssd1306_SetCursor(Data.pos_x, Data.pos_y);
    ssd1306_FillRectangle(DATA_X, Data.pos_y, 128, Data.pos_y + 9, Black);
    ssd1306_WriteString(Data.Data, Font_7x10, White);
    ssd1306_SetCursor(APPEND_X, Data.pos_y);
    ssd1306_WriteString(Data.appendex, Font_7x10, White);
}

//! Function Callback Funcs, called by IO
void callBackingBtns(void)
{
    updateSTATE(DDS_Menu_Modifying);
    if (Menu.encFlg) {
        switch (Menu.Switches.Encoder)
        {
        case ClockWise:
            updateButtonMsgs(Menu.Switches.ButtonMsgs[3]);
            Menu.Switches.SwitchAction = Increase;
            break;

        case AntiClockWise:
            updateButtonMsgs(Menu.Switches.ButtonMsgs[4]);
            Menu.Switches.SwitchAction = Decrease;
            break;
        default:
            updateButtonMsgs("Fault!");
            break;
        }
        Menu.Switches.Encoder = IDLE;
        Menu.encFlg = 0;
    }
    else if (Menu.switchFlg) {
        if (Menu.Switches.SW1 == Switch_Pressed) {
            updateButtonMsgs(Menu.Switches.ButtonMsgs[0]);
            Menu.Switches.SW1 = IDLE;
            Menu.Switches.SwitchAction = Reset;
        }
        else if (Menu.Switches.SW2 == Switch_Pressed) {
            updateButtonMsgs(Menu.Switches.ButtonMsgs[1]);
            Menu.Switches.SW2 = IDLE;
            Menu.Switches.SwitchAction = ChangeSelection;
        }
        else if (Menu.Switches.ESW == Switch_Pressed) {
            updateButtonMsgs(Menu.Switches.ButtonMsgs[2]);
            Menu.Switches.SwitchAction = PauseOrPlay;
            Menu.Switches.ESW = IDLE;
        }
        Menu.switchFlg = 0;
    }
    else { return; }

    ssd1306_UpdateScreen();

}

//! Util Funcs

char* value2char(uint32_t value) {
    // 将数字转换为字符串，并且三位数字中添加一个”,“号
    static char str[10];
    int i = 0;
    int j = 0;
    int k = 0;
    int len = 0;
    int temp = 0;
    char temp_str[10];
    memset(str, 0, sizeof(str));
    memset(temp_str, 0, sizeof(temp_str));
    sprintf(temp_str, "%d", value);
    len = strlen(temp_str);
    for (i = len - 1; i >= 0; i--) {
        str[j++] = temp_str[i];
        k++;
        if (k == 3 && i != 0) {
            str[j++] = ',';
            k = 0;
        }
    }
    str[j] = '\0';
    len = strlen(str);
    for (i = 0; i < len / 2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

void update2Data(DataText* Data, uint32_t value, char* appendex, uint8_t row) {
    Data->Data = value2char(value);
    Data->appendex = appendex;
    updateTheData(*Data, row);
}

void update2Wave(DataText* WavData, WaveList Wav) {
    switch (Wav)
    {
    case Sine:
        WavData->Data = "Sine";
        break;
    case Square:
        WavData->Data = "Square";
        break;
    case Triangle:
        WavData->Data = "Triangle";
        break;
    default:
        WavData->Data = "Sine";
        break;
    }
    WavData->appendex = "";
    updateTheData(*WavData, 0);
}

void Menu_Init(void)
{
    if (Menu.Inited == 0) {
        //Init Values
        Menu.WaveForm.freq = 1000;
        Menu.WaveForm.amp = 50;
        Menu.WaveForm.step = 100;
        Menu.WaveForm.wave = Sine;
        Menu.Choose = WaveFormat;
        Menu.Switches.SW1 = IDLE;
        Menu.Switches.SW2 = IDLE;
        Menu.Switches.ESW = IDLE;
        Menu.Switches.Encoder = IDLE;
        Menu.Switches.SwitchAction = Waiting;
        Menu.DDS_STATE = DDS_Pause;

        //Init Strings to display
        Menu.Label[0].Title = Titles[0];
        Menu.Label[1].Title = Titles[1];
        Menu.Label[2].Title = Titles[2];
        Menu.Label[3].Title = Titles[3];

        Menu.Switches.ButtonMsgs[0] = ButtonMsgs[0];
        Menu.Switches.ButtonMsgs[1] = ButtonMsgs[1];
        Menu.Switches.ButtonMsgs[2] = ButtonMsgs[2];
        Menu.Switches.ButtonMsgs[3] = ButtonMsgs[3];
        Menu.Switches.ButtonMsgs[4] = ButtonMsgs[4];
        //Begin 2 draw what's demanded

        Menu.Inited = 1;
    }
    ssd1306_Fill(Black);

    update2Wave(&Menu.Data[0], Menu.WaveForm.wave);
    update2Data(&Menu.Data[1], Menu.WaveForm.freq, "Hz", 1);
    update2Data(&Menu.Data[3], Menu.WaveForm.amp, "mV", 3);
    update2Data(&Menu.Data[2], Menu.WaveForm.step, "Hz", 2);

    updateStatusChoose((uint8_t)Menu.Choose, Choose);

    // Main Title
    ssd1306_FillRectangle(0, 0, 128, 11, White);
    ssd1306_SetCursor(25, 1);
    ssd1306_WriteString("ICE40_DDS", Font_7x10, Black);
    ssd1306_Line(TITLE_OFFSET + 1, 12, TITLE_OFFSET + 1, 55, White);

    for (int i = 0; i < 4; i++) {
        updateLabels(Menu.Label[i], i);
    }
    //Button Msg
    updateButtonMsgs((ButtonMsg*)"Button waiting");

    ssd1306_UpdateScreen();
}


