/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.cpp
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>

#include <cstdint>
#include <cstdio>

#include "24l01.h"
#include "framework.h"
#include "led.h"
#include "mmc_sd.h"
#include "piclib.h"
#include "remote.h"
#include "scene.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// #define MAIN 1
// #define CALC 2
// #define CHAT_SELECT 3
// #define CHAT_SCENE 4
// #define PHOTO 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
bool scan_key();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
string RX_DATA;
char uLength = 0;
extern unsigned char rxBuffer[1024];
extern bool rx_flag;
pii touch;
bool button_click[8];
// LED leddev = LED();  // led_dev
char *tmp = new char[1];
vtext *choosed;
uint8_t EVENT[16];
bool fly = false;
string users[3];
chat_scene_storage *chat_sc_store[3];
int selected_chat;
int SCENE = MAIN;
int trans = 1;
u8 key;
u32 check_online_cnt = 0;
u32 on_check[3] = {0,0,0};
u16 t = 0;
u8 tmp_buf[33] = "STM32F103C8T6@ALIENTEK";
u8 rx_buf[33] = {0};
u8 emoji_number = 0;
u8 send_emoji = 0;

int user_code = 0;

RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef DateToUpdate = {0};
string remote_press;
u8 user_status[3] = {0,0,0};\
u8 to_user = 0;
int from = 0;
bool sliding = false;
pii slide_start_pos;
pii slide_prev_pos;


void find_to_user(){
  if (selected_chat == 0) {
      if (user_code == 0) {
          to_user = 1;
      } else {
          to_user = 0;
      }
  } else if (selected_chat == 1) {
    if (user_code == 2) {
      to_user = 1;
    }else {
      to_user = 2;
    }
  } else if (selected_chat == 2) {
      to_user = 3;//group_chat
  }
}

void send_invite(){// format in:from:to
  NRF24L01_TX_Mode();
  find_to_user();
  string tmp = "in:" + to_string(user_code) + ":" + to_string(to_user);
  for (int i = 0; i < tmp.length(); i++) 
  {
      key = tmp.c_str()[i];
      tmp_buf[i] = key;
  }
  for(int i = tmp.length(); i <= 32; i++)
  {
      tmp_buf[i] = 0;
  }
  printf("%s", tmp_buf);
  NRF24L01_TxPacket(tmp_buf);
  NRF24L01_RX_Mode(); 
  printf("send invite\n");

}



/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* USER CODE BEGIN 1 */
        
    /* USER CODE END 1 */

    /* MCU
     * Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the
     * Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    delay_ms(100);
    // MX_I2C1_Init();

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_I2C1_Init();
    MX_TIM3_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();
    MX_SPI1_Init();
    MX_RTC_Init();
    /* USER CODE BEGIN 2 */
    //   printf("LCD shape %u %u", lcddev.height, lcddev.width);
    // leddev.Init();
    LCD_Init();
    LCD_Clear(WHITE);
    HAL_TIM_Base_Start_IT(&htim3);
    NRF24L01_Init();

    tp_dev.init();
    // leddev.append(BLINK_BOTH);
    // leddev.append(BLINK_BOTH);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    dpo canvas = dpo("canvas", {lcddev.width / 2, lcddev.height / 2},
                     {lcddev.width, lcddev.height});
    printf("here");

    printf("Done Creating\n");
    bar *bottom_bar = new bar("bar1", {0, 140}, {lcddev.width, 40});
    dpo *window_view = new dpo("window_view", {0, -20}, {lcddev.width, 280});

    calc_main *cal_sc = nullptr;
    chat_select_main *chat_sel_sc = nullptr;
    // emoji_scene_main *emoji_sc = nullptr;
    chat_scene_main *chat_sc = nullptr;
    album_scene_main *album_sc = nullptr;
    main_scene *main_sc = nullptr;

    canvas.add_son(bottom_bar);
    canvas.add_son(window_view);
    // window_view.add_son(&cal_sc);
    // cal_sc.setVisbility(false);
    printf("Start\n");
    // window_view.add_son(&emoji_sc);
    // emoji_sc.setVisbility(false);
    // window_view.add_son(&chat_sc);
    // chat_sc.setVisbility(true);

    HAL_UART_Receive_IT(&huart1, (uint8_t *)rxBuffer, 1);
    u32 total, free;

    mem_init();
    // while (SD_Init())  // �?测不到SD�?
    // {
    //     LCD_ShowString(30, 150, 200, 16, 16, (uint8_t *)"SD Card Error!");
    //     delay_ms(5000);
    //     LCD_ShowString(30, 150, 200, 16, 16, (uint8_t *)"Please Check! ");
    //     delay_ms(5000);
    // }
    piclib_init();
    exfuns_init();
    Remote_Init();

    f_mount(fs[0], "0:", 1);  // 挂载SD�?
    f_mount(fs[1], "1:", 1);  // 挂载FLASH.
    SCENE = MAIN;
    NRF24L01_RX_Mode();
    while (1) {
        // LCD_ShowString(2, 2, 160, 16, 16, (uint8_t *)"Mem");
        tp_dev.scan(0);
        if (scan_key()) {
            EVENT[REMOTE_KEY] = 1;
        } else {
            EVENT[REMOTE_KEY] = 0;
        }
        touch = {(int)tp_dev.x[0], (int)tp_dev.y[0]};
        fly = equal_pii(touch, {65535, 65535});
        canvas.update(nullptr, {0, 0});
        POINT_COLOR = BLUE;  // 设置字体为蓝�?
        // LCD_ShowString(30, 150, 200, 16, 16, (uint8_t *)"FATFS OK!");
        // LCD_ShowString(30, 170, 200, 16, 16,
        //                (uint8_t *)"SD Total Size:     MB");
        // LCD_ShowString(30, 190, 200, 16, 16,
        //                (uint8_t *)"SD  Free Size:     MB");

        // exf_getfree((uint8_t *)"0:", &total, &free);
        // LCD_ShowNum(30 + 8 * 14, 170, total >> 10, 5, 16);  //
        // 显示SD卡�?�容�? MB LCD_ShowNum(30 + 8 * 14, 190, free >> 10, 5,
        //             16);  // 显示SD卡剩余容�? MB

        if (!fly)
            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
        if (!fly) {
            if (!sliding) {
                sliding = true;
                slide_start_pos = touch;
            } else {
                slide_prev_pos = touch;
            }
        } else {
            if (sliding) {
                sliding = false;
                int x_delta = slide_prev_pos.first - slide_start_pos.first;
                if (abs(x_delta) > 30) {
                    if (x_delta < 0) {
                        EVENT[SLIDE_LEFT] = 1;
                    } else {
                        EVENT[SLIDE_RIGHT] = 1;
                    }
                }
            }
        }
        if (EVENT[SLIDE_LEFT]) {
            printf("[EVENT] Slide Left\n");
        }
        if (EVENT[SLIDE_RIGHT]) {
            printf("[EVENT] Slide Right\n");
        }
        if (EVENT[RETURN_BACK]) {
            printf("[EVENT] Press Back\n");
            if (chat_sc != nullptr)
                SCENE = CHAT_SELECT;
            else
                SCENE = MAIN;
            EVENT[RETURN_BACK] = 0;
        }
        if (EVENT[RETURN_HOME]) {
            printf("[EVENT] Press Home\n");
            SCENE = MAIN;
            EVENT[RETURN_HOME] = 0;
        }
        if (EVENT[CHAT_SELECTED]) {
            printf("[EVENT] Chat Select\n");
            SCENE = CHAT_SELECT;
            EVENT[CHAT_SELECTED] = 0;
        }
        if (EVENT[CALC_SELECTED]) {
            printf("[EVENT] Calc Select\n");
            SCENE = CALC;
            EVENT[CALC_SELECTED] = 0;
        }
        if (EVENT[PHOTO_SELECTED]) {
            printf("[EVENT] Photo Select\n");
            SCENE = ALBUM_SCENE;
            EVENT[PHOTO_SELECTED] = 0;
        }

        if (EVENT[CREAT_CHAT]) {
            printf("[EVENT] Create Chat\n");
            send_invite();
            SCENE = CHAT_SCENE;
            EVENT[CREAT_CHAT] = 0;
        }
        if (EVENT[REMOTE_KEY]) {
            printf("[EVENT] Remote Key %s\n", remote_press.c_str());
        }
        if (SCENE == MAIN) {
            window_view->clear_display();
            canvas.need_render = true;
            if (chat_sc != nullptr) {
                chat_sc_store[selected_chat] = new chat_scene_storage(
                    chat_sc->page_cnt, chat_sc->now_page, chat_sc->pageMessage,
                    chat_sc->pageEmoji, chat_sc->showPage.cnt);
                delete (chat_sc), chat_sc = nullptr;
            }
            if (chat_sel_sc != nullptr)
                delete (chat_sel_sc), chat_sel_sc = nullptr;
            if (cal_sc != nullptr) delete (cal_sc), cal_sc = nullptr;
            if (album_sc != nullptr) delete (album_sc), album_sc = nullptr;
            if (main_sc == nullptr) {
                printf("MAIN\n");
                main_sc =
                    new main_scene("main_scene", {0, 0}, {lcddev.width, 280});
                main_sc->userName.update_str("User" + to_string(user_code), 24,
                                             BLACK, WHITE);
                window_view->sub_object_cnt = 0;
                memset(window_view->sub_object, 0,
                       sizeof(window_view->sub_object));
                window_view->add_son(main_sc);
                main_sc->setVisbility(true);
            }
            SCENE = NONE_SCENE;
        }
        if (SCENE == CALC) {
            window_view->clear_display();
            canvas.need_render = true;
            if (main_sc != nullptr) delete (main_sc), main_sc = nullptr;
            if (chat_sc != nullptr) {
                chat_sc_store[selected_chat] = new chat_scene_storage(
                    chat_sc->page_cnt, chat_sc->now_page, chat_sc->pageMessage,
                    chat_sc->pageEmoji, chat_sc->showPage.cnt);
                delete (chat_sc), chat_sc = nullptr;
            }
            if (chat_sel_sc != nullptr)
                delete (chat_sel_sc), chat_sel_sc = nullptr;
            if (album_sc != nullptr) delete (album_sc), album_sc = nullptr;
            if (cal_sc == nullptr) {
                cal_sc =
                    new calc_main("calc_main", {0, 0}, {lcddev.width, 280});
                window_view->sub_object_cnt = 0;
                memset(window_view->sub_object, 0,
                       sizeof(window_view->sub_object));
                window_view->add_son(cal_sc);
                cal_sc->setVisbility(true);
            }

            SCENE = NONE_SCENE;
        }

        if (SCENE == CHAT_SELECT) {
            window_view->clear_display();
            canvas.need_render = true;
            if (main_sc != nullptr) delete (main_sc), main_sc = nullptr;
            if (chat_sc != nullptr) {
                chat_sc_store[selected_chat] = new chat_scene_storage(
                    chat_sc->page_cnt, chat_sc->now_page, chat_sc->pageMessage,
                    chat_sc->pageEmoji, chat_sc->showPage.cnt);
                delete (chat_sc), chat_sc = nullptr;
            }
            if (cal_sc != nullptr) delete (cal_sc), cal_sc = nullptr;
            if (album_sc != nullptr) delete (album_sc), album_sc = nullptr;
            if (chat_sel_sc == nullptr) {
                chat_sel_sc = new chat_select_main("chat_select_main", {0, 0},
                                                   {lcddev.width, 280});
                if (user_code == 0) {
                    chat_sel_sc->userName[0].update_str("User1", 16, BLACK,
                                                         WHITE);
                    chat_sel_sc->userName[1].update_str("User2", 16, BLACK,
                                                         WHITE);
                    if (user_status[1] == 1) {
                        chat_sel_sc->status[0].update_str("Online", 16, BLACK, WHITE);
                    }else if (user_status[2] == 1) {
                        chat_sel_sc->status[1].update_str("Online", 16, BLACK, WHITE);
                    
                    }
                } else if (user_code == 1) {
                    chat_sel_sc->userName[0].update_str("User0", 16, BLACK,
                                                         WHITE);
                    chat_sel_sc->userName[1].update_str("User2", 16, BLACK,
                                                         WHITE);
                    if (user_status[0] == 1) {
                        chat_sel_sc->status[0].update_str("Online", 16, BLACK, WHITE);
                    }else if (user_status[2] == 1) {
                        chat_sel_sc->status[1].update_str("Online", 16, BLACK, WHITE);
                    
                    }
                } else if (user_code == 2) {
                    chat_sel_sc->userName[0].update_str("User0", 16, BLACK,
                                                         WHITE);
                    chat_sel_sc->userName[1].update_str("User1", 16, BLACK,
                                                         WHITE);
                    if (user_status[1] == 1) {
                        chat_sel_sc->status[0].update_str("Online", 16, BLACK, WHITE);
                    }else if (user_status[2] == 1) {
                        chat_sel_sc->status[1].update_str("Online", 16, BLACK, WHITE);
                    
                    }
                }

                window_view->sub_object_cnt = 0;
                memset(window_view->sub_object, 0,
                       sizeof(window_view->sub_object));
                window_view->add_son(chat_sel_sc);
                chat_sel_sc->setVisbility(true);
            }
            SCENE = NONE_SCENE;
        }

        if (SCENE == CHAT_SCENE) {
            window_view->clear_display();
            canvas.need_render = true;
            if (main_sc != nullptr) delete (main_sc), main_sc = nullptr;
            if (chat_sel_sc != nullptr)
                delete (chat_sel_sc), chat_sel_sc = nullptr;
            if (cal_sc != nullptr) delete (cal_sc), cal_sc = nullptr;
            if (album_sc != nullptr) delete (album_sc), album_sc = nullptr;
            if (chat_sc == nullptr) {
                if (chat_sc_store[selected_chat] == nullptr) {
                    chat_sc = new chat_scene_main("chat_scene_main", {0, 0},
                                                  {lcddev.width, 280}, users);
                } else {
                    chat_sc = new chat_scene_main("chat_scene_main", {0, 0},
                                                  {lcddev.width, 280}, users,
                                                  chat_sc_store[selected_chat]);
                }
                window_view->sub_object_cnt = 0;
                memset(window_view->sub_object, 0,
                       sizeof(window_view->sub_object));
                window_view->add_son(chat_sc);
                printf("Done creating\n");
                chat_sc->setVisbility(true);
            }

            SCENE = NONE_SCENE;
        }
        if (main_sc != nullptr && check_online_cnt % 500 == 2) {
          HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
          HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD);\
          //需要将十六进制转成十进制
          int hour = ((sTime.Hours & 0xf0) >> 4) * 10 + (sTime.Hours & 0x0f);
          int min = ((sTime.Minutes & 0xf0) >> 4) * 10 + (sTime.Minutes & 0x0f);
          int sec = ((sTime.Seconds & 0xf0) >> 4) * 10 + (sTime.Seconds & 0x0f);
          main_sc->time.update_str(to_string(hour) + ":" + to_string(min) + ":" + to_string(sec), 16, BLACK, WHITE);  
        }
        if (trans) {
            
        }
        if (SCENE == ALBUM_SCENE) {
            window_view->clear_display();
            canvas.need_render = true;
            if (main_sc != nullptr) delete (main_sc), main_sc = nullptr;
            if (chat_sc != nullptr) {
                chat_sc_store[selected_chat] = new chat_scene_storage(
                    chat_sc->page_cnt, chat_sc->now_page, chat_sc->pageMessage,
                    chat_sc->pageEmoji, chat_sc->showPage.cnt);
                delete (chat_sc), chat_sc = nullptr;
            }
            if (chat_sel_sc != nullptr)
                delete (chat_sel_sc), chat_sel_sc = nullptr;
            if (cal_sc != nullptr) delete (cal_sc), cal_sc = nullptr;
            if (album_sc == nullptr) {
                album_sc = new album_scene_main("album_scene_main", {0, 0},
                                                {lcddev.width, 280});
                window_view->sub_object_cnt = 0;
                memset(window_view->sub_object, 0,
                       sizeof(window_view->sub_object));
                window_view->add_son(album_sc);
                album_sc->setVisbility(true);
            }
            SCENE = NONE_SCENE;
        }
        if (rx_flag && chat_sc != nullptr) { //显示并同步发送
            //在信息前面加上header 格式为 from(e/m)to:msg
            find_to_user();
            string tmp = to_string(user_code) + "m" + to_string(to_user) + ":";
            tmp += RX_DATA;
            chat_sc->addMessageToPage(RX_DATA, user_code);
            canvas.need_render = true;
            NRF24L01_TX_Mode();
            key = ' ';
            // NRF24L01_TxPacket(tmp_buf);
            for (int i = 0; i < tmp.length(); i++) 
            {
                key = tmp.c_str()[i];
                tmp_buf[i] = key;
            }
            for(int i = tmp.length(); i <= 32; i++)
            {
                tmp_buf[i] = 0;
            }
            NRF24L01_TxPacket(tmp_buf);
            RX_DATA = "";
            rx_flag = 0;
            NRF24L01_RX_Mode();
        }else if (send_emoji && chat_sc!=nullptr) {
          find_to_user();
          string tmp = to_string(user_code) + "e" + to_string(to_user) + ":" + to_string(emoji_number);
          NRF24L01_TX_Mode();
          key = ' ';
          // NRF24L01_TxPacket(tmp_buf);
          for (int i = 0; i < tmp.length(); i++) 
          {
              key = tmp.c_str()[i];
              tmp_buf[i] = key;
          }
          for(int i = tmp.length(); i <= 32; i++)
          {
              tmp_buf[i] = 0;
          }
          NRF24L01_TxPacket(tmp_buf);
          send_emoji = 0;
          NRF24L01_RX_Mode();
        }
        else if(rx_flag){
            RX_DATA = "";
            rx_flag = 0;
        }
        
        if (check_online_cnt % 10000 == 0) {
            NRF24L01_TX_Mode();
            tmp_buf[0] = 'o';
            tmp_buf[1] = 'n';
            tmp_buf[2] = user_code + '0';
            for(int i = 3; i <= 32; i++)
            {
                tmp_buf[i] = 0;
            }
            printf("%s", tmp_buf);
            NRF24L01_TxPacket(tmp_buf);
            NRF24L01_RX_Mode(); 
            printf("send online\n");
            
        }
        for(int i = 0; i<3;i++){
          on_check[i]++;
          if (on_check[i] > 50000 && user_status[i] == 1 && i != user_code) {
            user_status[i] = 0;
            string tmp = "User" +to_string(i) + " Offline!";
            LCD_ShowString(2, 230, 240, 16, 16, (uint8_t *)tmp.c_str());
            HAL_Delay(1500);
            LCD_Fill(2, 230, 240, 230 + 16, WHITE);
            if (chat_sel_sc!=nullptr) {
                if (user_code == 0 && i == 1) {
                    chat_sel_sc->status[0].update_str("Offline", 16, BLACK, WHITE);
                    user_status[1] = 0;
                } else if (user_code == 0 && i == 2) {
                    chat_sel_sc->status[1].update_str("Offline", 16, BLACK, WHITE);
                    user_status[2] = 0;
                } else if (user_code == 1 && i == 0) {
                    chat_sel_sc->status[0].update_str("Offline", 16, BLACK, WHITE);
                    user_status[0] = 0;
                } else if (user_code == 1 && i == 2) {
                    chat_sel_sc->status[1].update_str("Offline", 16, BLACK, WHITE);
                    user_status[2] = 0;
                } else if (user_code == 2 && i == 0) {
                    chat_sel_sc->status[0].update_str("Offline", 16, BLACK, WHITE);
                    user_status[0] = 0;
                } else if (user_code == 2 && i == 1) {
                    chat_sel_sc->status[1].update_str("Offline", 16, BLACK, WHITE);
                    user_status[1] = 0;
                }
            }
            canvas.need_render = true;
          
          }
        }
        check_online_cnt>10001?check_online_cnt=0:check_online_cnt++;
        if (NRF24L01_RxPacket(rx_buf) == 0)  // 一旦接收到信息,则显示出来.
        { 
          if (rx_buf[0] == 'o' && rx_buf[1] == 'n') {
            on_check[rx_buf[2] - '0'] = 0;
          }
          if (rx_buf[0] == 'o' && rx_buf[1] == 'n' && user_status[rx_buf[2] - '0'] == 0) {
            user_status[rx_buf[2] - '0'] = 1;
            printf("status in chat_sc\n");
            string tmp = "User" +to_string(rx_buf[2] - '0') + " Online!";
            LCD_ShowString(2, 200, 240, 16, 16, (uint8_t *)tmp.c_str());
            HAL_Delay(1500);
            LCD_Fill(2, 200, 240, 230 + 16, WHITE);
            if (chat_sel_sc!=nullptr) {
                if (user_code == 0 && rx_buf[2] == '1') {
                    chat_sel_sc->status[0].update_str("Online", 16, BLACK, WHITE);
                    user_status[1] = 1;
                } else if (user_code == 0 && rx_buf[2] == '2') {
                    chat_sel_sc->status[1].update_str("Online", 16, BLACK, WHITE);
                    user_status[2] = 1;
                } else if (user_code == 1 && rx_buf[2] == '0') {
                    chat_sel_sc->status[0].update_str("Online", 16, BLACK, WHITE);
                    user_status[0] = 1;
                } else if (user_code == 1 && rx_buf[2] == '2') {
                    chat_sel_sc->status[1].update_str("Online", 16, BLACK, WHITE);
                    user_status[2] = 1;
                } else if (user_code == 2 && rx_buf[2] == '0') {
                    chat_sel_sc->status[0].update_str("Online", 16, BLACK, WHITE);
                    user_status[0] = 1;
                } else if (user_code == 2 && rx_buf[2] == '1') {
                    chat_sel_sc->status[1].update_str("Online", 16, BLACK, WHITE);
                    user_status[1] = 1;
                }
            }
            canvas.need_render = true;
          }
          if (rx_buf[0] == 'i' && rx_buf[1] == 'n' && rx_buf[5] == user_code + '0') {
            printf("invite in chat_sc\n");
            if (chat_sc == nullptr) {
              string tmp = "User" +to_string(rx_buf[3] - '0') + " invite you to chat!";
              LCD_ShowString(2, 200, 240, 16, 16, (uint8_t *)tmp.c_str());
              HAL_Delay(1500);
              LCD_Fill(2, 200, 240, 230 + 16, WHITE);
              canvas.need_render = true;
            }
          }
          if (rx_buf[0] == 'i' && rx_buf[1] == 'n' && rx_buf[5] == '3') {
            if (chat_sc == nullptr || selected_chat != 2) {
              string tmp = "User" +to_string(rx_buf[3] - '0') + " invite you to have a group chat!";
              LCD_ShowString(2, 200, 240, 16, 16, (uint8_t *)tmp.c_str());
              HAL_Delay(1500);
              LCD_Fill(2, 200, 240, 230 + 16, WHITE);
              canvas.need_render = true;
            }
          }          
          if(chat_sc != nullptr && (rx_buf[2] - '0' == user_code || (rx_buf[2] - '0' == 3 && selected_chat == 2))){
            if (rx_buf[1] == 'm')
            {
              rx_buf[32] = 0;  // 加入字符串结束符
              from = rx_buf[0] - '0';
              for(int i = 0; i < 29; i++)
              {
                  rx_buf[i] = rx_buf[i + 4];
              }
              rx_buf[32] = 0;
              string tmp = string((char*)rx_buf);
              chat_sc->addMessageToPage(tmp, from);
            }
            else if (rx_buf[1] == 'e')
            {
              rx_buf[32] = 0;  // 加入字符串结束符
              from = rx_buf[0] - '0';
              chat_sc->addImageToPage(rx_buf[4]-'0', from);
            }
          }
        }
        if (NRF24L01_Check()) {
            printf("NRF24L01 error\n");
        }
       

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        
    }


    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {
    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void) {
    /* USER CODE BEGIN RTC_Init 0 */

    /* USER CODE END RTC_Init 0 */

    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef DateToUpdate = {0};


    /* USER CODE END RTC_Init 1 */

    /** Initialize RTC Only
     */
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
    if (HAL_RTC_Init(&hrtc) != HAL_OK) {
        Error_Handler();
    }

    /* USER CODE BEGIN Check_RTC_BKUP */

    /* USER CODE END Check_RTC_BKUP */

    /** Initialize RTC and set the Time and Date
     */
    sTime.Hours = 0x0;
    sTime.Minutes = 0x0;
    sTime.Seconds = 0x0;

    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
        Error_Handler();
    }
    DateToUpdate.WeekDay = RTC_WEEKDAY_FRIDAY;
    DateToUpdate.Month = RTC_MONTH_DECEMBER;
    DateToUpdate.Date = 0x21;
    DateToUpdate.Year = 0x0;

    if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN RTC_Init 2 */

    /* USER CODE END RTC_Init 2 */
}

/**
 * @brief SDIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_SDIO_SD_Init(void) {
    /* USER CODE BEGIN SDIO_Init 0 */

    /* USER CODE END SDIO_Init 0 */

    /* USER CODE BEGIN SDIO_Init 1 */

    /* USER CODE END SDIO_Init 1 */
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_4B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 36;
    /* USER CODE BEGIN SDIO_Init 2 */

    /* USER CODE END SDIO_Init 2 */
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {
    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {
    /* USER CODE BEGIN TIM3_Init 0 */

    /* USER CODE END TIM3_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 7199;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 9;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) !=
        HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */

    /* USER CODE END TIM3_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {
    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    /* DMA2_Channel4_5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, CS_Pin | CE_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : IRQ_Pin */
    GPIO_InitStruct.Pin = IRQ_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IRQ_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : KEY_WK_Pin */
    GPIO_InitStruct.Pin = KEY_WK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(KEY_WK_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : CS_Pin CE_Pin */
    GPIO_InitStruct.Pin = CS_Pin | CE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PA3 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : KEY0_Pin */
    GPIO_InitStruct.Pin = KEY0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : LED0_Pin */
    GPIO_InitStruct.Pin = LED0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

bool scan_key() {
    u8 key = Remote_Scan();
    static u8 last_key = 0;
    if (key) {
        switch (key) {
            case 0:
                remote_press = "ERROR";
                break;
            case 162:
                remote_press = "POWER";
                break;
            case 98:
                remote_press = "UP";
                break;
            case 2:
                remote_press = "PLAY";
                break;
            case 226:
                remote_press = "ALIENTEK";
                break;
            case 194:
                remote_press = "RIGHT";
                break;
            case 34:
                remote_press = "LEFT";
                break;
            case 224:
                remote_press = "VOL-";
                break;
            case 168:
                remote_press = "DOWN";
                break;
            case 144:
                remote_press = "VOL+";
                break;
            case 104:
                remote_press = "1";
                break;
            case 152:
                remote_press = "2";
                break;
            case 176:
                remote_press = "3";
                break;
            case 48:
                remote_press = "4";
                break;
            case 24:
                remote_press = "5";
                break;
            case 122:
                remote_press = "6";
                break;
            case 16:
                remote_press = "7";
                break;
            case 56:
                remote_press = "8";
                break;
            case 90:
                remote_press = "9";
                break;
            case 66:
                remote_press = "0";
                break;
            case 82:
                remote_press = "DELETE";
                break;
        }
        if (key && key != last_key) {
            last_key = key;
            return true;
        }
    }
    last_key = key;
    return false;
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
