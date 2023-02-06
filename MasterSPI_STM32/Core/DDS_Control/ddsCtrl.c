#include "ddsCtrl.h"
#include "stdlib.h"
#include "stdio.h"
#include "menu.h"
#include "tim.h"
#include "spi_lib.h"
#include "ssd1306.h"

extern MenuList Menu;
extern uint32_t dds_rd[3];

//! SPI Funcs
uint8_t no_param[3] = { 0x0, 0x0, 0x0 };
uint8_t spi_status = 0;
uint8_t data_read[3];
uint8_t val_inv[3] = { 0x38, 0xAE, 0x3B };
uint8_t val_led_yellow[3] = { 0x0, 0x0, 0x3 };
uint8_t val_led_blue[3] = { 0x0, 0x0, 0x4 };
uint8_t val_led_red[3] = { 0x0,0x0,0x1 };
uint8_t val_led_green[3] = { 0x0,0x0,0x2 };

//TODO: TEST_LED
uint8_t* led_ptr = &(val_led_yellow[0]);



//! Publish 2 DDS func
void Publish2DDS() {
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
    spi_send(SPI_INIT, no_param, NULL);
    spi_send(SPI_SET_LED, led_ptr, &spi_status); // led change
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

    if (Menu.DDS_STATE == DDS_Play) {
        do {
            spi_write_dds(Menu.WaveForm.freq, Menu.WaveForm.amp, Menu.WaveForm.wave);
            printf("Write2DDS:freq: %d,amp: %d,wav: %d\n", Menu.WaveForm.freq, Menu.WaveForm.amp, (uint8_t)Menu.WaveForm.wave);

            spi_dump_dds();
            printf("ReadDDS:freq: %d,wav: %d,amp: %d\n", dds_rd[0], dds_rd[1], dds_rd[2]);
        } while (dds_rd[0] != Menu.WaveForm.freq || (uint8_t)dds_rd[1] != (uint8_t)Menu.WaveForm.wave
            || (uint16_t)dds_rd[2] != Menu.WaveForm.amp);
        HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
        spi_send(SPI_INIT, no_param, NULL);
        spi_send(SPI_CONF_DDS, led_ptr, &spi_status); // led change
        HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
        printf("DDS_Gend!\n==============\n");
    }


}

//! util funcs
void WaveFormatter(Action act) {
    switch (act)
    {
    case Increase:
        if (Menu.WaveForm.wave == Sine) {
            Menu.WaveForm.wave = Triangle;
        }
        else if (Menu.WaveForm.wave == Triangle) {
            Menu.WaveForm.wave = Square;
        }
        else if (Menu.WaveForm.wave == Square) {
            Menu.WaveForm.wave = Sine;
        }
        break;
    case Decrease:
        if (Menu.WaveForm.wave == Sine) {
            Menu.WaveForm.wave = Square;
        }
        else if (Menu.WaveForm.wave == Triangle) {
            Menu.WaveForm.wave = Sine;
        }
        else if (Menu.WaveForm.wave == Square) {
            Menu.WaveForm.wave = Triangle;
        }
        break;
    default:
        break;
    }
    update2Wave(&Menu.Data[0], Menu.WaveForm.wave);
}

void FrequencyFormatter(Action act) {
    uint32_t tmpFreq = Menu.WaveForm.freq;
    tmpFreq += act == Increase ? (int)Menu.WaveForm.step : -1 * (int)Menu.WaveForm.step;
    if (tmpFreq > 0 && tmpFreq <= 10000000) {
        Menu.WaveForm.freq = tmpFreq;
        update2Data(&Menu.Data[1], Menu.WaveForm.freq, "Hz", 1);
    }
}

void SteplintFormatter(Action act) {
    uint32_t stepList[] = { 1,10,100,1000,10000,100000,1000000 };
    uint32_t tmpStep = Menu.WaveForm.step;
    if (act == Increase) {
        for (int i = 0; i < 7; i++) {
            if (stepList[i] > tmpStep) {
                tmpStep = stepList[i];
                break;
            }
        }
    }
    else if (act == Decrease) {
        for (int i = 6; i >= 0; i--) {
            if (stepList[i] < tmpStep) {
                tmpStep = stepList[i];
                break;
            }
        }
    }
    Menu.WaveForm.step = tmpStep;
    update2Data(&Menu.Data[2], Menu.WaveForm.step, "Hz", 2);
}

void AmplitudeFormatter(Action act) {
    uint32_t tmpAmp = Menu.WaveForm.amp;
    tmpAmp += act == Increase ? 50 : -50;
    if (tmpAmp >= 100 && tmpAmp <= 1000) {
        Menu.WaveForm.amp = tmpAmp;
        update2Data(&Menu.Data[3], Menu.WaveForm.amp, "mV", 3);
    }

}

//! Impls of modifing the DDS
void Increase_impl() {
    switch (Menu.Choose)
    {
    case WaveFormat:
        WaveFormatter(Increase);
        break;
    case Frequency:
        FrequencyFormatter(Increase);
        break;
    case Steplint:
        SteplintFormatter(Increase);
        break;
    case Amplitude:
        AmplitudeFormatter(Increase);
        break;
    default:
        break;
    }
}

void Decrease_impl() {
    switch (Menu.Choose)
    {
    case WaveFormat:
        WaveFormatter(Decrease);
        break;
    case Frequency:
        FrequencyFormatter(Decrease);
        break;
    case Steplint:
        SteplintFormatter(Decrease);
        break;
    case Amplitude:
        AmplitudeFormatter(Decrease);
        break;
    default:
        break;
    }
}

void PauseOrPlay_impl() {
    if (Menu.DDS_STATE == DDS_Pause) {
        Menu.DDS_STATE = DDS_Play;
    }
    else {
        Menu.DDS_STATE = DDS_Pause;
    }
}

void Reset_impl() {
    ssd1306_Init();
    Menu_Init();
}

void ChangeSelection_impl() {
    switch (Menu.Choose)
    {
    case WaveFormat:
        updateStatusChoose((uint8_t)Menu.Choose, UnChoose);
        Menu.Choose = Frequency;
        updateStatusChoose((uint8_t)Menu.Choose, Choose);
        break;
    case Frequency:
        updateStatusChoose((uint8_t)Menu.Choose, UnChoose);
        Menu.Choose = Steplint;
        updateStatusChoose((uint8_t)Menu.Choose, Choose);
        break;
    case Steplint:
        updateStatusChoose((uint8_t)Menu.Choose, UnChoose);
        Menu.Choose = Amplitude;
        updateStatusChoose((uint8_t)Menu.Choose, Choose);
        break;
    case Amplitude:
        updateStatusChoose((uint8_t)Menu.Choose, UnChoose);
        Menu.Choose = WaveFormat;
        updateStatusChoose((uint8_t)Menu.Choose, Choose);
        break;

    default:
        break;
    }

}



//! Main Selection
void ModifieDDS(void) {
    switch (Menu.Switches.SwitchAction)
    {
    case Increase:
        Increase_impl();
        led_ptr = &val_led_blue[0];
        break;
    case Decrease:
        Decrease_impl();
        led_ptr = &val_led_green[0];
        break;
    case PauseOrPlay:
        PauseOrPlay_impl();
        led_ptr = &no_param[0];
        break;
    case Reset:
        Reset_impl();
        break;
    case ChangeSelection:
        ChangeSelection_impl();
        led_ptr = &val_led_red[0];
        break;
    default:
        break;
    }
    Menu.Switches.SwitchAction = Waiting;
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM14) {
        updateSTATE(Menu.DDS_STATE);
        if (Menu.DDS_STATE == DDS_Play) {
            led_ptr = val_led_yellow;
        }
        Publish2DDS();
        ssd1306_UpdateScreen();
    }
}

