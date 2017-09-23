#ifndef STM32_RTC_H
#define STM32_RTC_H

#include "stm32_inc.h"

enum class ERTCFormat
{
    BIN,
    BCD,
};

typedef struct
{
  uint8_t Hours;            /*!< Specifies the RTC Time Hour.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected  */

  uint8_t Minutes;          /*!< Specifies the RTC Time Minutes.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

  uint8_t Seconds;          /*!< Specifies the RTC Time Seconds.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

  uint8_t TimeFormat;       /*!< Specifies the RTC AM/PM Time.
                                 This parameter can be a value of @ref RTC_AM_PM_Definitions */

  uint32_t SubSeconds;     /*!< Specifies the RTC_SSR RTC Sub Second register content.
                                 This parameter corresponds to a time unit range between [0-1] Second
                                 with [1 Sec / SecondFraction +1] granularity */

  uint32_t SecondFraction;  /*!< Specifies the range or granularity of Sub Second register content
                                 corresponding to Synchronous pre-scaler factor value (PREDIV_S)
                                 This parameter corresponds to a time unit range between [0-1] Second
                                 with [1 Sec / SecondFraction +1] granularity.
                                 This field will be used only by HAL_RTC_GetTime function */

  uint32_t DayLightSaving;  /*!< Specifies DayLight Save Operation.
                                 This parameter can be a value of @ref RTC_DayLightSaving_Definitions */

  uint32_t StoreOperation;  /*!< Specifies RTC_StoreOperation value to be written in the BCK bit
                                 in CR register to store the operation.
                                 This parameter can be a value of @ref RTC_StoreOperation_Definitions */
} STM32_RTC_Time;

typedef struct
{
  uint8_t WeekDay;  /*!< Specifies the RTC Date WeekDay.
                         This parameter can be a value of @ref RTC_WeekDay_Definitions */

  uint8_t Month;    /*!< Specifies the RTC Date Month (in BCD format).
                         This parameter can be a value of @ref RTC_Month_Date_Definitions */

  uint8_t Date;     /*!< Specifies the RTC Date.
                         This parameter must be a number between Min_Data = 1 and Max_Data = 31 */

  uint8_t Year;     /*!< Specifies the RTC Date Year.
                         This parameter must be a number between Min_Data = 0 and Max_Data = 99 */

} STM32_RTC_Date;

#define RTC_MONTH_JANUARY              ((uint8_t)0x01U)
#define RTC_MONTH_FEBRUARY             ((uint8_t)0x02U)
#define RTC_MONTH_MARCH                ((uint8_t)0x03U)
#define RTC_MONTH_APRIL                ((uint8_t)0x04U)
#define RTC_MONTH_MAY                  ((uint8_t)0x05U)
#define RTC_MONTH_JUNE                 ((uint8_t)0x06U)
#define RTC_MONTH_JULY                 ((uint8_t)0x07U)
#define RTC_MONTH_AUGUST               ((uint8_t)0x08U)
#define RTC_MONTH_SEPTEMBER            ((uint8_t)0x09U)
#define RTC_MONTH_OCTOBER              ((uint8_t)0x10U)
#define RTC_MONTH_NOVEMBER             ((uint8_t)0x11U)
#define RTC_MONTH_DECEMBER             ((uint8_t)0x12U)

#define RTC_WEEKDAY_MONDAY             ((uint8_t)0x01U)
#define RTC_WEEKDAY_TUESDAY            ((uint8_t)0x02U)
#define RTC_WEEKDAY_WEDNESDAY          ((uint8_t)0x03U)
#define RTC_WEEKDAY_THURSDAY           ((uint8_t)0x04U)
#define RTC_WEEKDAY_FRIDAY             ((uint8_t)0x05U)
#define RTC_WEEKDAY_SATURDAY           ((uint8_t)0x06U)
#define RTC_WEEKDAY_SUNDAY             ((uint8_t)0x07U)

class STM32_RTC
{
public:
    static uint32_t init();
    static uint32_t deinit();

    static uint32_t set_time(STM32_RTC_Time *time, ERTCFormat format);
    static uint32_t get_time(STM32_RTC_Time *time, ERTCFormat format);

    static uint32_t set_date(STM32_RTC_Date *date, ERTCFormat format);
    static uint32_t get_date(STM32_RTC_Date *date, ERTCFormat format);
private:
    static inline void write_protect_disable() { RTC->WPR = 0xCAU; RTC->WPR = 0x53U; }
    static inline void write_protect_enable() { RTC->WPR = 0xFFU; }

    static uint32_t enter_init_mode();
    static uint32_t wait_for_synchro();

    /// TODO
    /// HAL_RTC_SetAlarm
    /// HAL_RTC_SetAlarm_IT
    /// HAL_RTC_DeactivateAlarm
    /// HAL_RTC_GetAlarm
    /// HAL_RTC_AlarmIRQHandler
    /// HAL_RTC_PollForAlarmAEvent
};

#endif // STM32_RTC_H
