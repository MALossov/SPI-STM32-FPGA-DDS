/*
 * @Description:
 * @Author: MALossov
 * @Date: 2023-02-05 11:01:29
 * @LastEditTime: 2023-02-05 12:26:46
 * @LastEditors: MALossov
 * @Reference:
 */
#include "menu.h"
#include "gpio.h"
#include "main.h"
#include "menu.h"
#include "string.h"
#include "ssd1306.h"

MenuList Menu;

char* WaveMsgs[3] = { "Sine", "Square", "Triangle" };
char* Titles[4] = { "Wave","Freq", "Step","Ampli" };
char* ButtonMsgs[5] = { "SW1_Pressed","SW2_Pressed","Enc_Pressed","Enc_Clock","Enc_AntiClock" };

int Button_x(char* ButtonMsg) {
    int len = strlen(ButtonMsg) * 6;
    return 64 - len / 2;
}

int Title_x(char* Title)
{
    int len = strlen(Title) * 7;
    return 40 - len;
}

int Index_y(uint8_t row) {
    return row * 11 + 12;
}

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
    ssd1306_SetCursor(25, 55);
    ssd1306_WriteString(ButtonMsg, Font_6x8, Black);
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

}

void callBackingBtns(void)
{
    if (Menu.encFlg) {
        switch (Menu.Switches.Encoder)
        {
        case ClockWise:
            updateButtonMsgs(Menu.Switches.ButtonMsgs[3]);
            break;

        case AntiClockWise:
            updateButtonMsgs(Menu.Switches.ButtonMsgs[4]);
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
        }
        else if (Menu.Switches.SW2 == Switch_Pressed) {
            updateButtonMsgs(Menu.Switches.ButtonMsgs[1]);
            Menu.Switches.SW2 = IDLE;
        }
        else if (Menu.Switches.ESW == Switch_Pressed) {
            updateButtonMsgs(Menu.Switches.ButtonMsgs[2]);
            Menu.Switches.ESW = IDLE;
        }
        Menu.switchFlg = 0;
    }
    else { return; }

    ssd1306_UpdateScreen();

}


void Menu_Init(void)
{
    //Init Values
    Menu.WaveForm.freq = 1000;
    Menu.WaveForm.amp = 50;
    Menu.WaveForm.wave = Sine;
    Menu.Switches.SW1 = IDLE;
    Menu.Switches.SW2 = IDLE;
    Menu.Switches.ESW = IDLE;
    Menu.Switches.Encoder = IDLE;

    //Init Strings to display
    Menu.Label[0].Title = Titles[0];
    Menu.Label[1].Title = Titles[1];
    Menu.Label[2].Title = Titles[2];
    Menu.Label[3].Title = Titles[3];

    Menu.WaveList[0] = WaveMsgs[0];
    Menu.WaveList[1] = WaveMsgs[1];
    Menu.WaveList[2] = WaveMsgs[2];

    Menu.Switches.ButtonMsgs[0] = ButtonMsgs[0];
    Menu.Switches.ButtonMsgs[1] = ButtonMsgs[1];
    Menu.Switches.ButtonMsgs[2] = ButtonMsgs[2];
    Menu.Switches.ButtonMsgs[3] = ButtonMsgs[3];
    Menu.Switches.ButtonMsgs[4] = ButtonMsgs[4];


    //Begin 2 draw what's demanded

    ssd1306_Fill(Black);

    // Main Title
    ssd1306_FillRectangle(0, 0, 128, 11, White);
    ssd1306_SetCursor(25, 1);
    ssd1306_WriteString("ICE40_DDS", Font_7x10, Black);
    ssd1306_Line(42, 12, 42, 55, White);

    for (int i = 0; i < 4; i++) {
        updateLabels(Menu.Label[i], i);
    }
    //Button Msg
    updateButtonMsgs((ButtonMsg*)"Button waiting");

    ssd1306_UpdateScreen();
}


