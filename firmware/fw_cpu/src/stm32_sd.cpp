#include "stm32_sd.h"

#define DATA_BLOCK_SIZE                  ((uint32_t)(9U << 4U))

#define SDIO_CLOCK_EDGE_RISING               ((uint32_t)0x00000000U)
#define SDIO_CLOCK_EDGE_FALLING              SDIO_CLKCR_NEGEDGE

#define SDIO_CLOCK_BYPASS_DISABLE             ((uint32_t)0x00000000U)
#define SDIO_CLOCK_BYPASS_ENABLE              SDIO_CLKCR_BYPASS

#define SDIO_CLOCK_POWER_SAVE_DISABLE         ((uint32_t)0x00000000U)
#define SDIO_CLOCK_POWER_SAVE_ENABLE          SDIO_CLKCR_PWRSAV

#define SDIO_BUS_WIDE_1B                      ((uint32_t)0x00000000U)
#define SDIO_BUS_WIDE_4B                      SDIO_CLKCR_WIDBUS_0
#define SDIO_BUS_WIDE_8B                      SDIO_CLKCR_WIDBUS_1

#define SDIO_HARDWARE_FLOW_CONTROL_DISABLE    ((uint32_t)0x00000000U)
#define SDIO_HARDWARE_FLOW_CONTROL_ENABLE     SDIO_CLKCR_HWFC_EN

#define SDIO_RESPONSE_NO                    ((uint32_t)0x00000000U)
#define SDIO_RESPONSE_SHORT                 SDIO_CMD_WAITRESP_0
#define SDIO_RESPONSE_LONG                  SDIO_CMD_WAITRESP

#define SDIO_WAIT_NO                        ((uint32_t)0x00000000U)
#define SDIO_WAIT_IT                        SDIO_CMD_WAITINT
#define SDIO_WAIT_PEND                      SDIO_CMD_WAITPEND

#define SDIO_CPSM_DISABLE                   ((uint32_t)0x00000000U)
#define SDIO_CPSM_ENABLE                    SDIO_CMD_CPSMEN

#define SDIO_RESP1                          ((uint32_t)0x00000000U)
#define SDIO_RESP2                          ((uint32_t)0x00000004U)
#define SDIO_RESP3                          ((uint32_t)0x00000008U)
#define SDIO_RESP4                          ((uint32_t)0x0000000CU)

#define SDIO_DATABLOCK_SIZE_1B               ((uint32_t)0x00000000U)
#define SDIO_DATABLOCK_SIZE_2B               SDIO_DCTRL_DBLOCKSIZE_0
#define SDIO_DATABLOCK_SIZE_4B               SDIO_DCTRL_DBLOCKSIZE_1
#define SDIO_DATABLOCK_SIZE_8B               ((uint32_t)0x00000030U)
#define SDIO_DATABLOCK_SIZE_16B              SDIO_DCTRL_DBLOCKSIZE_2
#define SDIO_DATABLOCK_SIZE_32B              ((uint32_t)0x00000050U)
#define SDIO_DATABLOCK_SIZE_64B              ((uint32_t)0x00000060U)
#define SDIO_DATABLOCK_SIZE_128B             ((uint32_t)0x00000070U)
#define SDIO_DATABLOCK_SIZE_256B             SDIO_DCTRL_DBLOCKSIZE_3
#define SDIO_DATABLOCK_SIZE_512B             ((uint32_t)0x00000090U)
#define SDIO_DATABLOCK_SIZE_1024B            ((uint32_t)0x000000A0U)
#define SDIO_DATABLOCK_SIZE_2048B            ((uint32_t)0x000000B0U)
#define SDIO_DATABLOCK_SIZE_4096B            ((uint32_t)0x000000C0U)
#define SDIO_DATABLOCK_SIZE_8192B            ((uint32_t)0x000000D0U)
#define SDIO_DATABLOCK_SIZE_16384B           ((uint32_t)0x000000E0U)

#define SDIO_TRANSFER_DIR_TO_CARD            ((uint32_t)0x00000000U)
#define SDIO_TRANSFER_DIR_TO_SDIO            SDIO_DCTRL_DTDIR

#define SDIO_TRANSFER_MODE_BLOCK             ((uint32_t)0x00000000U)
#define SDIO_TRANSFER_MODE_STREAM            SDIO_DCTRL_DTMODE

#define SDIO_DPSM_DISABLE                    ((uint32_t)0x00000000U)
#define SDIO_DPSM_ENABLE                     SDIO_DCTRL_DTEN

#define SDIO_READ_WAIT_MODE_DATA2             ((uint32_t)0x00000000U)
#define SDIO_READ_WAIT_MODE_CLK               ((uint32_t)0x00000001U)

#define SDIO_IT_CCRCFAIL                    SDIO_STA_CCRCFAIL
#define SDIO_IT_DCRCFAIL                    SDIO_STA_DCRCFAIL
#define SDIO_IT_CTIMEOUT                    SDIO_STA_CTIMEOUT
#define SDIO_IT_DTIMEOUT                    SDIO_STA_DTIMEOUT
#define SDIO_IT_TXUNDERR                    SDIO_STA_TXUNDERR
#define SDIO_IT_RXOVERR                     SDIO_STA_RXOVERR
#define SDIO_IT_CMDREND                     SDIO_STA_CMDREND
#define SDIO_IT_CMDSENT                     SDIO_STA_CMDSENT
#define SDIO_IT_DATAEND                     SDIO_STA_DATAEND
#define SDIO_IT_STBITERR                    SDIO_STA_STBITERR
#define SDIO_IT_DBCKEND                     SDIO_STA_DBCKEND
#define SDIO_IT_CMDACT                      SDIO_STA_CMDACT
#define SDIO_IT_TXACT                       SDIO_STA_TXACT
#define SDIO_IT_RXACT                       SDIO_STA_RXACT
#define SDIO_IT_TXFIFOHE                    SDIO_STA_TXFIFOHE
#define SDIO_IT_RXFIFOHF                    SDIO_STA_RXFIFOHF
#define SDIO_IT_TXFIFOF                     SDIO_STA_TXFIFOF
#define SDIO_IT_RXFIFOF                     SDIO_STA_RXFIFOF
#define SDIO_IT_TXFIFOE                     SDIO_STA_TXFIFOE
#define SDIO_IT_RXFIFOE                     SDIO_STA_RXFIFOE
#define SDIO_IT_TXDAVL                      SDIO_STA_TXDAVL
#define SDIO_IT_RXDAVL                      SDIO_STA_RXDAVL
#define SDIO_IT_SDIOIT                      SDIO_STA_SDIOIT
#define SDIO_IT_CEATAEND                    SDIO_STA_CEATAEND

#define SDIO_FLAG_CCRCFAIL                  SDIO_STA_CCRCFAIL
#define SDIO_FLAG_DCRCFAIL                  SDIO_STA_DCRCFAIL
#define SDIO_FLAG_CTIMEOUT                  SDIO_STA_CTIMEOUT
#define SDIO_FLAG_DTIMEOUT                  SDIO_STA_DTIMEOUT
#define SDIO_FLAG_TXUNDERR                  SDIO_STA_TXUNDERR
#define SDIO_FLAG_RXOVERR                   SDIO_STA_RXOVERR
#define SDIO_FLAG_CMDREND                   SDIO_STA_CMDREND
#define SDIO_FLAG_CMDSENT                   SDIO_STA_CMDSENT
#define SDIO_FLAG_DATAEND                   SDIO_STA_DATAEND
#define SDIO_FLAG_STBITERR                  SDIO_STA_STBITERR
#define SDIO_FLAG_DBCKEND                   SDIO_STA_DBCKEND
#define SDIO_FLAG_CMDACT                    SDIO_STA_CMDACT
#define SDIO_FLAG_TXACT                     SDIO_STA_TXACT
#define SDIO_FLAG_RXACT                     SDIO_STA_RXACT
#define SDIO_FLAG_TXFIFOHE                  SDIO_STA_TXFIFOHE
#define SDIO_FLAG_RXFIFOHF                  SDIO_STA_RXFIFOHF
#define SDIO_FLAG_TXFIFOF                   SDIO_STA_TXFIFOF
#define SDIO_FLAG_RXFIFOF                   SDIO_STA_RXFIFOF
#define SDIO_FLAG_TXFIFOE                   SDIO_STA_TXFIFOE
#define SDIO_FLAG_RXFIFOE                   SDIO_STA_RXFIFOE
#define SDIO_FLAG_TXDAVL                    SDIO_STA_TXDAVL
#define SDIO_FLAG_RXDAVL                    SDIO_STA_RXDAVL
#define SDIO_FLAG_SDIOIT                    SDIO_STA_SDIOIT
#define SDIO_FLAG_CEATAEND                  SDIO_STA_CEATAEND

#define SDIO_STATIC_FLAGS                   (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_DCRCFAIL | \
                                             SDIO_FLAG_CTIMEOUT | SDIO_FLAG_DTIMEOUT | \
                                             SDIO_FLAG_TXUNDERR | SDIO_FLAG_RXOVERR | \
                                             SDIO_FLAG_CMDREND | SDIO_FLAG_CMDSENT | \
                                             SDIO_FLAG_DATAEND | SDIO_FLAG_DBCKEND)

#define CLKCR_CLEAR_MASK         ((uint32_t)(SDIO_CLKCR_CLKDIV  | SDIO_CLKCR_PWRSAV |\
                                             SDIO_CLKCR_BYPASS  | SDIO_CLKCR_WIDBUS |\
                                             SDIO_CLKCR_NEGEDGE | SDIO_CLKCR_HWFC_EN))

#define DCTRL_CLEAR_MASK         ((uint32_t)(SDIO_DCTRL_DTEN    | SDIO_DCTRL_DTDIR |\
                                             SDIO_DCTRL_DTMODE  | SDIO_DCTRL_DBLOCKSIZE))

#define CMD_CLEAR_MASK           ((uint32_t)(SDIO_CMD_CMDINDEX | SDIO_CMD_WAITRESP |\
                                             SDIO_CMD_WAITINT  | SDIO_CMD_WAITPEND |\
                                             SDIO_CMD_CPSMEN   | SDIO_CMD_SDIOSUSPEND))

#define SDIO_RESP_ADDR           ((uint32_t)(SDIO_BASE + 0x14U))

#define SDIO_INIT_CLK_DIV ((uint8_t)0x76U)

#define SDIO_TRANSFER_CLK_DIV ((uint8_t)0x00U)

#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0U)   /*!< Resets the SD memory card.                                                               */
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1U)   /*!< Sends host capacity support information and activates the card's initialization process. */
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2U)   /*!< Asks any card connected to the host to send the CID numbers on the CMD line.             */
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3U)   /*!< Asks the card to publish a new relative address (RCA).                                   */
#define SD_CMD_SET_DSR                             ((uint8_t)4U)   /*!< Programs the DSR of all cards.                                                           */
#define SD_CMD_SDIO_SEN_OP_COND                    ((uint8_t)5U)   /*!< Sends host capacity support information (HCS) and asks the accessed card to send its
                                                                       operating condition register (OCR) content in the response on the CMD line.              */
#define SD_CMD_HS_SWITCH                           ((uint8_t)6U)   /*!< Checks switchable function (mode 0) and switch card function (mode 1).                   */
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7U)   /*!< Selects the card by its own relative address and gets deselected by any other address    */
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8U)   /*!< Sends SD Memory Card interface condition, which includes host supply voltage information
                                                                       and asks the card whether card supports voltage.                                         */
#define SD_CMD_SEND_CSD                            ((uint8_t)9U)   /*!< Addressed card sends its card specific data (CSD) on the CMD line.                       */
#define SD_CMD_SEND_CID                            ((uint8_t)10U)  /*!< Addressed card sends its card identification (CID) on the CMD line.                      */
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11U)  /*!< SD card doesn't support it.                                                              */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12U)  /*!< Forces the card to stop transmission.                                                    */
#define SD_CMD_SEND_STATUS                         ((uint8_t)13U)  /*!< Addressed card sends its status register.                                                */
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14U)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15U)  /*!< Sends an addressed card into the inactive state.                                         */
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16U)  /*!< Sets the block length (in bytes for SDSC) for all following block commands
                                                                       (read, write, lock). Default block length is fixed to 512 Bytes. Not effective
                                                                       for SDHS and SDXC.                                                                       */
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17U)  /*!< Reads single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of
                                                                       fixed 512 bytes in case of SDHC and SDXC.                                                */
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18U)  /*!< Continuously transfers data blocks from card to host until interrupted by
                                                                       STOP_TRANSMISSION command.                                                               */
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19U)  /*!< 64 bytes tuning pattern is sent for SDR50 and SDR104.                                    */
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20U)  /*!< Speed class control command.                                                             */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23U)  /*!< Specify block count for CMD18 and CMD25.                                                 */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24U)  /*!< Writes single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of
                                                                       fixed 512 bytes in case of SDHC and SDXC.                                                */
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25U)  /*!< Continuously writes blocks of data until a STOP_TRANSMISSION follows.                    */
#define SD_CMD_PROG_CID                            ((uint8_t)26U)  /*!< Reserved for manufacturers.                                                              */
#define SD_CMD_PROG_CSD                            ((uint8_t)27U)  /*!< Programming of the programmable bits of the CSD.                                         */
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28U)  /*!< Sets the write protection bit of the addressed group.                                    */
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29U)  /*!< Clears the write protection bit of the addressed group.                                  */
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30U)  /*!< Asks the card to send the status of the write protection bits.                           */
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32U)  /*!< Sets the address of the first write block to be erased. (For SD card only).              */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33U)  /*!< Sets the address of the last write block of the continuous range to be erased.           */
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35U)  /*!< Sets the address of the first write block to be erased. Reserved for each command
                                                                       system set by switch function command (CMD6).                                            */
#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36U)  /*!< Sets the address of the last write block of the continuous range to be erased.
                                                                       Reserved for each command system set by switch function command (CMD6).                  */
#define SD_CMD_ERASE                               ((uint8_t)38U)  /*!< Reserved for SD security applications.                                                   */
#define SD_CMD_FAST_IO                             ((uint8_t)39U)  /*!< SD card doesn't support it (Reserved).                                                   */
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40U)  /*!< SD card doesn't support it (Reserved).                                                   */
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42U)  /*!< Sets/resets the password or lock/unlock the card. The size of the data block is set by
                                                                       the SET_BLOCK_LEN command.                                                               */
#define SD_CMD_APP_CMD                             ((uint8_t)55U)  /*!< Indicates to the card that the next command is an application specific command rather
                                                                       than a standard command.                                                                 */
#define SD_CMD_GEN_CMD                             ((uint8_t)56U)  /*!< Used either to transfer a data block to the card or to get a data block from the card
                                                                       for general purpose/application specific commands.                                       */
#define SD_CMD_NO_CMD                              ((uint8_t)64U)

/**
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD should be sent before sending these commands.
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6U)   /*!< (ACMD6) Defines the data bus width to be used for data transfer. The allowed data bus
                                                                       widths are given in SCR register.                                                          */
#define SD_CMD_SD_APP_STATUS                       ((uint8_t)13U)  /*!< (ACMD13) Sends the SD status.                                                              */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22U)  /*!< (ACMD22) Sends the number of the written (without errors) write blocks. Responds with
                                                                       32bit+CRC data block.                                                                      */
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41U)  /*!< (ACMD41) Sends host capacity support information (HCS) and asks the accessed card to
                                                                       send its operating condition register (OCR) content in the response on the CMD line.       */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42U)  /*!< (ACMD42) Connects/Disconnects the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card. */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51U)  /*!< Reads the SD Configuration Register (SCR).                                                 */
#define SD_CMD_SDIO_RW_DIRECT                      ((uint8_t)52U)  /*!< For SD I/O card only, reserved for security specification.                                 */
#define SD_CMD_SDIO_RW_EXTENDED                    ((uint8_t)53U)  /*!< For SD I/O card only, reserved for security specification.                                 */

/**
  * @brief Following commands are SD Card Specific security commands.
  *        SD_CMD_APP_CMD should be sent before sending these commands.
  */
#define SD_CMD_SD_APP_GET_MKB                      ((uint8_t)43U)  /*!< For SD card only */
#define SD_CMD_SD_APP_GET_MID                      ((uint8_t)44U)  /*!< For SD card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45U)  /*!< For SD card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46U)  /*!< For SD card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47U)  /*!< For SD card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48U)  /*!< For SD card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18U)  /*!< For SD card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25U)  /*!< For SD card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38U)  /*!< For SD card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49U)  /*!< For SD card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48U)  /*!< For SD card only */

#define STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000U)
#define STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001U)
#define HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002U)
#define MULTIMEDIA_CARD                       ((uint32_t)0x00000003U)
#define SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004U)
#define HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005U)
#define SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006U)
#define HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007U)

#define SDIO_CMD0TIMEOUT                      ((uint32_t)0x00010000U)

/**
  * @brief  Mask for errors Card Status R1 (OCR Register)
  */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000U)
#define SD_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000U)
#define SD_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000U)
#define SD_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000U)
#define SD_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000U)
#define SD_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000U)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000U)
#define SD_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000U)
#define SD_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000U)
#define SD_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000U)
#define SD_OCR_CC_ERROR                 ((uint32_t)0x00100000U)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000U)
#define SD_OCR_STREAM_READ_UNDERRUN     ((uint32_t)0x00040000U)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((uint32_t)0x00020000U)
#define SD_OCR_CID_CSD_OVERWRITE        ((uint32_t)0x00010000U)
#define SD_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000U)
#define SD_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000U)
#define SD_OCR_ERASE_RESET              ((uint32_t)0x00002000U)
#define SD_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008U)
#define SD_OCR_ERRORBITS                ((uint32_t)0xFDFFE008U)

/**
  * @brief  Masks for R6 Response
  */
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000U)
#define SD_R6_ILLEGAL_CMD               ((uint32_t)0x00004000U)
#define SD_R6_COM_CRC_FAILED            ((uint32_t)0x00008000U)

#define SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000U)
#define SD_HIGH_CAPACITY                ((uint32_t)0x40000000U)
#define SD_STD_CAPACITY                 ((uint32_t)0x00000000U)
#define SD_CHECK_PATTERN                ((uint32_t)0x000001AAU)

#define SD_VOLTAGE_WINDOW_WALID         ((uint32_t)0x80000000U)

#define SD_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFFU)
#define SD_ALLZERO                      ((uint32_t)0x00000000U)

#define SD_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000U)
#define SD_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000U)
#define SD_CARD_LOCKED                  ((uint32_t)0x02000000U)

#define SD_DATATIMEOUT                  ((uint32_t)0xFFFFFFFFU)
#define SD_0TO7BITS                     ((uint32_t)0x000000FFU)
#define SD_8TO15BITS                    ((uint32_t)0x0000FF00U)
#define SD_16TO23BITS                   ((uint32_t)0x00FF0000U)
#define SD_24TO31BITS                   ((uint32_t)0xFF000000U)
#define SD_MAX_DATA_LENGTH              ((uint32_t)0x01FFFFFFU)

#define SD_HALFFIFO                     ((uint32_t)0x00000008U)
#define SD_HALFFIFOBYTES                ((uint32_t)0x00000020U)

/**
  * @brief  Command Class Supported
  */
#define SD_CCCC_LOCK_UNLOCK             ((uint32_t)0x00000080U)
#define SD_CCCC_WRITE_PROT              ((uint32_t)0x00000040U)
#define SD_CCCC_ERASE                   ((uint32_t)0x00000020U)

/**
  * @brief  Following commands are SD Card Specific commands.
  *         SDIO_APP_CMD should be sent before sending these commands.
  */
#define SD_SDIO_SEND_IF_COND            ((uint32_t)SD_CMD_HS_SEND_EXT_CSD)

uint32_t STM32_SD::m_card_type;
uint32_t STM32_SD::m_RCA;
uint32_t STM32_SD::m_CSD[4];
uint32_t STM32_SD::m_CID[4];
SD_CardInfo STM32_SD::m_card_info;

uint32_t STM32_SD::init()
{
    init_gpio();
    ///TODO init_DMA();
    STM32_NVIC::enable_and_set_prior_IRQ(SDIO_IRQn, 0, 0);

    init_low(SDIO_CLOCK_EDGE_RISING, SDIO_CLOCK_BYPASS_DISABLE,
             SDIO_CLOCK_POWER_SAVE_DISABLE, SDIO_BUS_WIDE_1B,
             SDIO_HARDWARE_FLOW_CONTROL_DISABLE, SDIO_INIT_CLK_DIV);

    SD_ErrorTypedef errorstate;

    if ((errorstate = power_ON()) != SD_OK)
        return errorstate;

    if ((errorstate = initialize_cards()) != SD_OK)
        return errorstate;

    if ((errorstate = get_card_info()) == SD_OK)
        select_deselect(m_card_info.RCA << 16);

    init_low(SDIO_CLOCK_EDGE_RISING, SDIO_CLOCK_BYPASS_DISABLE,
             SDIO_CLOCK_POWER_SAVE_DISABLE, SDIO_BUS_WIDE_1B,
             SDIO_HARDWARE_FLOW_CONTROL_DISABLE, STM32_SDIO_CLOCK_DIV);

    return errorstate;
}

void STM32_SD::deinit()
{
    power_OFF();

    STM32_RCC::disable_clk_SDIO();

    deinit_gpio();

    //TODO deinit DMA

    STM32_NVIC::disable_IRQ(SDIO_IRQn);
}

SD_ErrorTypedef STM32_SD::wide_bus_config(uint32_t mode)
{
    SD_ErrorTypedef errorstate = SD_OK;

    /* MMC Card does not support this feature */
    if (m_card_type == MULTIMEDIA_CARD)
        return SD_UNSUPPORTED_FEATURE;
    else if ((m_card_type == STD_CAPACITY_SD_CARD_V1_1) ||
             (m_card_type == STD_CAPACITY_SD_CARD_V2_0) ||
             (m_card_type == HIGH_CAPACITY_SD_CARD))
    {
        if (mode == SDIO_BUS_WIDE_8B)
            errorstate = SD_UNSUPPORTED_FEATURE;
        else if (mode == SDIO_BUS_WIDE_4B)
            errorstate = wide_bus_enable();
        else if (mode == SDIO_BUS_WIDE_1B)
            errorstate = wide_bus_disable();
        else
            /* mode is not a valid argument*/
            errorstate = SD_INVALID_PARAMETER;

        if (errorstate == SD_OK)
        {
            /* Configure the SDIO peripheral */
            init_low(SDIO_CLOCK_EDGE_RISING, SDIO_CLOCK_BYPASS_DISABLE,
                     SDIO_CLOCK_POWER_SAVE_DISABLE, mode,
                     SDIO_HARDWARE_FLOW_CONTROL_DISABLE, STM32_SDIO_CLOCK_DIV);
        }
    }

    return errorstate;
}

SD_TransferStateTypedef STM32_SD::get_status()
{
    SD_CardStateTypedef cardstate =  SD_CARD_TRANSFER;

    cardstate = get_state();

    /* Find SD status according to card state*/
    if (cardstate == SD_CARD_TRANSFER)
        return SD_TRANSFER_OK;
    else if(cardstate == SD_CARD_ERROR)
        return SD_TRANSFER_ERROR;
    else
        return SD_TRANSFER_BUSY;
}

SD_ErrorTypedef STM32_SD::read_blocks(uint8_t *buf, uint64_t read_addr, uint32_t block_size, uint32_t blocks)
{
    SD_ErrorTypedef errorstate = SD_OK;
    uint32_t count = 0U, *tempbuff = (uint32_t*)buf;

    /* Initialize data control register */
    SDIO->DCTRL = 0U;

    if (m_card_type == HIGH_CAPACITY_SD_CARD)
    {
        block_size = 512U;
        read_addr /= 512U;
    }

    /* Set Block Size for Card */
    send_command(block_size, SD_CMD_SET_BLOCKLEN, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_SET_BLOCKLEN);

    if (errorstate != SD_OK)
        return errorstate;

    /* Configure the SD DPSM (Data Path State Machine) */
    data_config(SD_DATATIMEOUT, blocks * block_size, DATA_BLOCK_SIZE,
                SDIO_TRANSFER_DIR_TO_SDIO, SDIO_TRANSFER_MODE_BLOCK,
                SDIO_DPSM_ENABLE);

    if (blocks > 1U)
    {
        /* Send CMD18 READ_MULT_BLOCK with argument data address */
        send_command(read_addr, SD_CMD_READ_MULT_BLOCK, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
    }
    else
    {
        /* Send CMD17 READ_SINGLE_BLOCK */
        send_command(read_addr, SD_CMD_READ_SINGLE_BLOCK, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
    }

    /* Read block(s) in polling mode */
    if (blocks > 1U)
    {
        /* Check for error conditions */
        errorstate = cmd_resp1_error(SD_CMD_READ_MULT_BLOCK);

        if (errorstate != SD_OK)
            return errorstate;

        /* Poll on SDIO flags */
        #ifdef SDIO_STA_STBITERR
        while (!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND | SDIO_FLAG_STBITERR))
        #else /* SDIO_STA_STBITERR not defined */
        while (!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND))
        #endif /* SDIO_STA_STBITERR */
        {
            if (get_flag(SDIO_FLAG_RXFIFOHF))
            {
                /* Read data from SDIO Rx FIFO */
                for (count = 0U; count < 8U; count++)
                    *(tempbuff + count) = read_FIFO();

                tempbuff += 8U;
            }
        }
    }
    else
    {
    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_READ_SINGLE_BLOCK);

    if (errorstate != SD_OK)
        return errorstate;

    /* In case of single block transfer, no need of stop transfer at all */
    #ifdef SDIO_STA_STBITERR
    while (!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))
    #else /* SDIO_STA_STBITERR not defined */
    while (!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND))
    #endif /* SDIO_STA_STBITERR */
    {
        if (get_flag(SDIO_FLAG_RXFIFOHF))
        {
            /* Read data from SDIO Rx FIFO */
            for (count = 0U; count < 8U; count++)
                *(tempbuff + count) = read_FIFO();

            tempbuff += 8U;
        }
    }
    }

    /* Send stop transmission command in case of multiblock read */
    if (get_flag(SDIO_FLAG_DATAEND) && (blocks > 1U))
    {
        if ((m_card_type == STD_CAPACITY_SD_CARD_V1_1) ||
            (m_card_type == STD_CAPACITY_SD_CARD_V2_0) ||
            (m_card_type == HIGH_CAPACITY_SD_CARD))
            /* Send stop transmission command */
            errorstate = stop_transfer();
    }

    /* Get error state */
    if (get_flag(SDIO_FLAG_DTIMEOUT))
    {
        clear_flag(SDIO_FLAG_DTIMEOUT);
        return SD_DATA_TIMEOUT;
    }
    else if (get_flag(SDIO_FLAG_DCRCFAIL))
    {
        clear_flag(SDIO_FLAG_DCRCFAIL);
        return SD_DATA_CRC_FAIL;
    }
    else if (get_flag(SDIO_FLAG_RXOVERR))
    {
        clear_flag(SDIO_FLAG_RXOVERR);
        return SD_RX_OVERRUN;
    }
    #ifdef SDIO_STA_STBITERR
    else if (get_flag(SDIO_FLAG_STBITERR))
    {
        clear_flag(SDIO_FLAG_STBITERR);
        return SD_START_BIT_ERR;
    }
    #endif /* SDIO_STA_STBITERR */
    else
    {
        /* No error flag set */
    }

    count = SD_DATATIMEOUT;

    /* Empty FIFO if there is still any data */
    while ((get_flag(SDIO_FLAG_RXDAVL)) && (count > 0U))
    {
        *tempbuff = read_FIFO();
        tempbuff++;
        count--;
    }

    /* Clear all the static flags */
    clear_flag(SDIO_STATIC_FLAGS);

    return errorstate;
}

SD_ErrorTypedef STM32_SD::write_blocks(uint8_t *buf, uint64_t write_addr, uint32_t block_size, uint32_t blocks)
{
    SD_ErrorTypedef errorstate = SD_OK;
    uint32_t totalnumberofbytes = 0U, bytestransferred = 0U, count = 0U, restwords = 0U;
    uint32_t *tempbuff = (uint32_t *)buf;
    uint8_t cardstate  = 0U;

    /* Initialize data control register */
    SDIO->DCTRL = 0U;

    if (m_card_type == HIGH_CAPACITY_SD_CARD)
    {
        block_size = 512U;
        write_addr /= 512U;
    }

    /* Set Block Size for Card */
    send_command(block_size, SD_CMD_SET_BLOCKLEN, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_SET_BLOCKLEN);

    if (errorstate != SD_OK)
        return errorstate;

    if (blocks > 1U)
        /* Send CMD25 WRITE_MULT_BLOCK with argument data address */
        send_command(write_addr, SD_CMD_WRITE_MULT_BLOCK, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
    else
        /* Send CMD24 WRITE_SINGLE_BLOCK */
        send_command(write_addr, SD_CMD_WRITE_SINGLE_BLOCK, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    if (blocks > 1U)
        errorstate = cmd_resp1_error(SD_CMD_WRITE_MULT_BLOCK);
    else
        errorstate = cmd_resp1_error(SD_CMD_WRITE_SINGLE_BLOCK);

    if (errorstate != SD_OK)
        return errorstate;

    /* Set total number of bytes to write */
    totalnumberofbytes = blocks * block_size;

    /* Configure the SD DPSM (Data Path State Machine) */
    data_config(SD_DATATIMEOUT, blocks * block_size, SDIO_DATABLOCK_SIZE_512B,
                SDIO_TRANSFER_DIR_TO_CARD, SDIO_TRANSFER_MODE_BLOCK,
                SDIO_DPSM_ENABLE);

    /* Write block(s) in polling mode */
    if (blocks > 1U)
    {
        #ifdef SDIO_STA_STBITERR
        while (!get_flag(SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND | SDIO_FLAG_STBITERR))
        #else /* SDIO_STA_STBITERR not defined */
        while (!get_flag(SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND))
        #endif /* SDIO_STA_STBITERR */
        {
            if (get_flag(SDIO_FLAG_TXFIFOHE))
            {
                if ((totalnumberofbytes - bytestransferred) < 32U)
                {
                    restwords = ((totalnumberofbytes - bytestransferred) % 4U == 0U) ?
                                ((totalnumberofbytes - bytestransferred) / 4U) :
                                (( totalnumberofbytes -  bytestransferred) / 4U + 1U);

                    /* Write data to SDIO Tx FIFO */
                    for (count = 0U; count < restwords; count++)
                    {
                        write_FIFO(tempbuff);
                        tempbuff++;
                        bytestransferred += 4U;
                    }
                }
                else
                {
                    /* Write data to SDIO Tx FIFO */
                    for (count = 0U; count < 8U; count++)
                        write_FIFO(tempbuff + count);

                    tempbuff += 8U;
                    bytestransferred += 32U;
                }
            }
        }
    }
    else
    {
        /* In case of single data block transfer no need of stop command at all */
        #ifdef SDIO_STA_STBITERR
        while (!get_flag(SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))
        #else /* SDIO_STA_STBITERR not defined */
        while (!get_flag(SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND))
        #endif /* SDIO_STA_STBITERR */
        {
            if (get_flag(SDIO_FLAG_TXFIFOHE))
            {
                if ((totalnumberofbytes - bytestransferred) < 32U)
                {
                    restwords = ((totalnumberofbytes - bytestransferred) % 4U == 0U) ?
                                ((totalnumberofbytes - bytestransferred) / 4U) :
                                (( totalnumberofbytes -  bytestransferred) / 4U + 1U);

                    /* Write data to SDIO Tx FIFO */
                    for (count = 0U; count < restwords; count++)
                    {
                        write_FIFO(tempbuff);
                        tempbuff++;
                        bytestransferred += 4U;
                    }
                }
                else
                {
                    /* Write data to SDIO Tx FIFO */
                    for (count = 0U; count < 8U; count++)
                        write_FIFO(tempbuff + count);

                    tempbuff += 8U;
                    bytestransferred += 32U;
                }
            }
        }
    }

    /* Send stop transmission command in case of multiblock write */
    if (get_flag(SDIO_FLAG_DATAEND) && (blocks > 1U))
    {
        if ((m_card_type == STD_CAPACITY_SD_CARD_V1_1) || (m_card_type == STD_CAPACITY_SD_CARD_V2_0) ||\
            (m_card_type == HIGH_CAPACITY_SD_CARD))
            /* Send stop transmission command */
            errorstate = stop_transfer();
    }

    /* Get error state */
    if (get_flag(SDIO_FLAG_DTIMEOUT))
    {
        clear_flag(SDIO_FLAG_DTIMEOUT);
        return SD_DATA_TIMEOUT;
    }
    else if (get_flag(SDIO_FLAG_DCRCFAIL))
    {
        clear_flag(SDIO_FLAG_DCRCFAIL);
        return SD_DATA_CRC_FAIL;
    }
    else if (get_flag(SDIO_FLAG_TXUNDERR))
    {
        clear_flag(SDIO_FLAG_TXUNDERR);
        return SD_TX_UNDERRUN;
    }
    #ifdef SDIO_STA_STBITERR
    else if (get_flag(SDIO_FLAG_STBITERR))
    {
        clear_flag(SDIO_FLAG_STBITERR);
        return SD_START_BIT_ERR;
    }
    #endif /* SDIO_STA_STBITERR */
    else
    {
        /* No error flag set */
    }

    /* Clear all the static flags */
    clear_flag(SDIO_STATIC_FLAGS);

    /* Wait till the card is in programming state */
    errorstate = is_card_programming(&cardstate);

    while ((errorstate == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
        errorstate = is_card_programming(&cardstate);

    return errorstate;
}

SD_ErrorTypedef STM32_SD::erase(uint64_t start_addr, uint64_t end_addr)
{
    SD_ErrorTypedef errorstate = SD_OK;

    uint32_t delay         = 0U;
    __IO uint32_t maxdelay = 0U;
    uint8_t cardstate      = 0U;

    /* Check if the card command class supports erase command */
    if (((m_CSD[1U] >> 20U) & SD_CCCC_ERASE) == 0U)
        return SD_REQUEST_NOT_APPLICABLE;

    /* Get max delay value */
    maxdelay = 120000U / (((SDIO->CLKCR) & 0xFFU) + 2U);

    if ((get_response(SDIO_RESP1) & SD_CARD_LOCKED) == SD_CARD_LOCKED)
        return SD_LOCK_UNLOCK_FAILED;

    /* Get start and end block for high capacity cards */
    if (m_card_type == HIGH_CAPACITY_SD_CARD)
    {
        start_addr /= 512U;
        end_addr   /= 512U;
    }

    /* According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
    if ((m_card_type == STD_CAPACITY_SD_CARD_V1_1) ||
        (m_card_type == STD_CAPACITY_SD_CARD_V2_0) ||
        (m_card_type == HIGH_CAPACITY_SD_CARD))
    {
        /* Send CMD32 SD_ERASE_GRP_START with argument as addr  */
        send_command((uint32_t)start_addr, SD_CMD_SD_ERASE_GRP_START, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

        /* Check for error conditions */
        errorstate = cmd_resp1_error(SD_CMD_SD_ERASE_GRP_START);

        if (errorstate != SD_OK)
            return errorstate;

        /* Send CMD33 SD_ERASE_GRP_END with argument as addr  */
        send_command((uint32_t)end_addr, SD_CMD_SD_ERASE_GRP_END, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

        /* Check for error conditions */
        errorstate = cmd_resp1_error(SD_CMD_SD_ERASE_GRP_END);

        if (errorstate != SD_OK)
            return errorstate;
    }

    /* Send CMD38 ERASE */
    send_command(0, SD_CMD_ERASE, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_ERASE);

    if (errorstate != SD_OK)
        return errorstate;

    for (; delay < maxdelay; delay++) {}

    /* Wait until the card is in programming state */
    errorstate = is_card_programming(&cardstate);

    delay = SD_DATATIMEOUT;

    while ((delay > 0U) &&
           (errorstate == SD_OK) &&
           ((cardstate == SD_CARD_PROGRAMMING) ||
            (cardstate == SD_CARD_RECEIVING)))
    {
        errorstate = is_card_programming(&cardstate);
        delay--;
    }

    return errorstate;
}

SD_ErrorTypedef STM32_SD::high_speed()
{
    SD_ErrorTypedef errorstate = SD_OK;

    uint8_t SD_hs[64U]  = {0U};
    uint32_t SD_scr[2U] = {0U, 0U};
    uint32_t SD_SPEC    = 0U;
    uint32_t count = 0U, *tempbuff = (uint32_t *)SD_hs;

    /* Initialize the Data control register */
    SDIO->DCTRL = 0U;

    /* Get SCR Register */
    errorstate = find_SCR(SD_scr);

    if (errorstate != SD_OK)
        return errorstate;

    /* Test the Version supported by the card*/
    SD_SPEC = (SD_scr[1U]  & 0x01000000U) | (SD_scr[1U]  & 0x02000000U);

    if (SD_SPEC != SD_ALLZERO)
    {
        /* Set Block Size for Card */
        send_command(64U, SD_CMD_SET_BLOCKLEN, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

        /* Check for error conditions */
        errorstate = cmd_resp1_error(SD_CMD_SET_BLOCKLEN);

        if (errorstate != SD_OK)
            return errorstate;

        /* Configure the SD DPSM (Data Path State Machine) */
        data_config(SD_DATATIMEOUT, 64U, SDIO_DATABLOCK_SIZE_64B,
                    SDIO_TRANSFER_DIR_TO_SDIO, SDIO_TRANSFER_MODE_BLOCK,
                    SDIO_DPSM_ENABLE);

        /* Send CMD6 switch mode */
        send_command(0x80FFFF01U, SD_CMD_HS_SWITCH, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

        /* Check for error conditions */
        errorstate = cmd_resp1_error(SD_CMD_HS_SWITCH);

        if (errorstate != SD_OK)
            return errorstate;
        #ifdef SDIO_STA_STBITERR
        while (!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))
        #else /* SDIO_STA_STBITERR */
        while (!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND))
        #endif /* SDIO_STA_STBITERR */
        {
            if (get_flag(SDIO_FLAG_RXFIFOHF))
            {
                for (count = 0U; count < 8U; count++)
                    *(tempbuff + count) = read_FIFO();

                tempbuff += 8U;
            }
        }

        if (get_flag(SDIO_FLAG_DTIMEOUT))
        {
            clear_flag(SDIO_FLAG_DTIMEOUT);
            return SD_DATA_TIMEOUT;
        }
        else if (get_flag(SDIO_FLAG_DCRCFAIL))
        {
            clear_flag(SDIO_FLAG_DCRCFAIL);
            return SD_DATA_CRC_FAIL;
        }
        else if (get_flag(SDIO_FLAG_RXOVERR))
        {
            clear_flag(SDIO_FLAG_RXOVERR);
            return SD_RX_OVERRUN;
        }
        #ifdef SDIO_STA_STBITERR
        else if (get_flag(SDIO_FLAG_STBITERR))
        {
            clear_flag(SDIO_FLAG_STBITERR);
            return SD_START_BIT_ERR;
        }
        #endif /* SDIO_STA_STBITERR */
        else
        {
            /* No error flag set */
        }

        count = SD_DATATIMEOUT;

        while ((get_flag(SDIO_FLAG_RXDAVL)) && (count > 0U))
        {
            *tempbuff = read_FIFO();
            tempbuff++;
            count--;
        }

        /* Clear all the static flags */
        clear_flag(SDIO_STATIC_FLAGS);

        /* Test if the switch mode HS is ok */
        if ((SD_hs[13U]& 2U) != 2U)
            errorstate = SD_UNSUPPORTED_FEATURE;
    }

    return errorstate;
}

SD_ErrorTypedef STM32_SD::send_SD_status(uint32_t *status)
{
    SD_ErrorTypedef errorstate = SD_OK;
    uint32_t count = 0U;

    /* Check SD response */
    if ((get_response(SDIO_RESP1) & SD_CARD_LOCKED) == SD_CARD_LOCKED)
        return SD_LOCK_UNLOCK_FAILED;

    /* Set block size for card if it is not equal to current block size for card */
    send_command(64U, SD_CMD_SET_BLOCKLEN, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_SET_BLOCKLEN);

    if (errorstate != SD_OK)
        return errorstate;

    /* Send CMD55 */
    send_command(m_RCA << 16U, SD_CMD_APP_CMD, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_APP_CMD);

    if (errorstate != SD_OK)
        return errorstate;

    /* Configure the SD DPSM (Data Path State Machine) */
    data_config(SD_DATATIMEOUT, 64U, SDIO_DATABLOCK_SIZE_64B,
                SDIO_TRANSFER_DIR_TO_SDIO, SDIO_TRANSFER_MODE_BLOCK,
                SDIO_DPSM_ENABLE);

    /* Send ACMD13 (SD_APP_STATUS)  with argument as card's RCA */
    send_command(0, SD_CMD_SD_APP_STATUS, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_SD_APP_STATUS);

    if (errorstate != SD_OK)
        return errorstate;

    /* Get status data */
    #ifdef SDIO_STA_STBITERR
    while (!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))
    #else /* SDIO_STA_STBITERR not defined */
    while (!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND))
    #endif /* SDIO_STA_STBITERR */
    {
        if (get_flag(SDIO_FLAG_RXFIFOHF))
        {
            for (count = 0U; count < 8U; count++)
                *(status + count) = read_FIFO();

            status += 8U;
        }
    }

    if (get_flag(SDIO_FLAG_DTIMEOUT))
    {
        clear_flag(SDIO_FLAG_DTIMEOUT);
        return SD_DATA_TIMEOUT;
    }
    else if (get_flag(SDIO_FLAG_DCRCFAIL))
    {
        clear_flag(SDIO_FLAG_DCRCFAIL);
        return SD_DATA_CRC_FAIL;
    }
    else if (get_flag(SDIO_FLAG_RXOVERR))
    {
        clear_flag(SDIO_FLAG_RXOVERR);
        return SD_RX_OVERRUN;
    }
    #ifdef SDIO_STA_STBITERR
    else if (get_flag(SDIO_FLAG_STBITERR))
    {
        clear_flag(SDIO_FLAG_STBITERR);
        return SD_START_BIT_ERR;
    }
    #endif /* SDIO_STA_STBITERR */
    else
    {
        /* No error flag set */
    }

    count = SD_DATATIMEOUT;
    while ((get_flag(SDIO_FLAG_RXDAVL)) && (count > 0U))
    {
        *status = read_FIFO();
        status++;
        count--;
    }

    /* Clear all the static status flags*/
    clear_flag(SDIO_STATIC_FLAGS);

    return errorstate;
}

void STM32_SD::init_gpio()
{
    STM32_RCC::enable_clk_SDIO();

    gpioc.set_config(GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                     GPIO_PIN_11 | GPIO_PIN_12, GPIO_MODE_AF_PP,
                     GPIO_AF12_SDIO, GPIO_SPEED_FREQ_VERY_HIGH,
                     GPIO_NOPULL);

    gpiod.set_config(GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_AF12_SDIO,
                     GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);
}

void STM32_SD::deinit_gpio()
{
    gpioc.unset_config(GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                       GPIO_PIN_11 | GPIO_PIN_12);
    gpiod.unset_config(GPIO_PIN_2);
}

void STM32_SD::init_low(uint32_t clock_edge, uint32_t clock_bypass,
                        uint32_t clock_power_save, uint32_t bus_wide,
                        uint32_t hw_control, uint32_t clock_div)
{
    uint32_t tmpreg = clock_edge | clock_bypass | clock_power_save |
            bus_wide | hw_control | clock_div;
    MODIFY_REG(SDIO->CLKCR, CLKCR_CLEAR_MASK, tmpreg);
}

SD_ErrorTypedef STM32_SD::power_ON()
{
    disable_SDIO();
    power_state_ON();
    /* 1ms: required power up waiting time before starting the SD initialization sequence */
    STM32_SYSTICK::delay(1);
    enable_SDIO();

    /* CMD0: GO_IDLE_STATE */
    send_command(0, SD_CMD_GO_IDLE_STATE, SDIO_RESPONSE_NO,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
    SD_ErrorTypedef errorstate = cmd_error();
    if (errorstate != SD_OK)
    {
        clear_flag(SDIO_STATIC_FLAGS);
        return errorstate;
    }

    /* CMD8: SEND_IF_COND ------------------------------------------------------*/
    /* Send CMD8 to verify SD card interface operating condition */
    /* Argument: - [31:12]: Reserved (shall be set to '0')
    - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
    - [7:0]: Check Pattern (recommended 0xAA) */
    uint32_t sdtype = SD_STD_CAPACITY;
    send_command(SD_CHECK_PATTERN, SD_SDIO_SEND_IF_COND, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
    errorstate = cmd_resp7_error();
    if (errorstate == SD_OK)
    {
        /* SD Card 2.0 */
        m_card_type = STD_CAPACITY_SD_CARD_V2_0;
        sdtype = SD_HIGH_CAPACITY;
    }

    /* Send CMD55 */
    send_command(0, SD_CMD_APP_CMD, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
    /* If errorstate is Command Timeout, it is a MMC card */
    /* If errorstate is SD_OK it is a SD card: SD card 2.0 (voltage range mismatch)
        or SD card 1.x */
    uint32_t response = 0;
    errorstate = cmd_resp1_error(SD_CMD_APP_CMD);
    if (errorstate == SD_OK)
    {
        /* SD CARD */
        /* Send ACMD41 SD_APP_OP_COND with Argument 0x80100000 */
        uint32_t validvoltage = 0;
        uint32_t count = 0;
        while ((!validvoltage) && (count <SD_MAX_VOLT_TRIAL))
        {
            /* SEND CMD55 APP_CMD with RCA as 0 */
            send_command(0, SD_CMD_APP_CMD, SDIO_RESPONSE_SHORT,
                         SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
            errorstate = cmd_resp1_error(SD_CMD_APP_CMD);
            if (errorstate != SD_OK)
                return errorstate;

            /* Send CMD41 */
            send_command(SD_VOLTAGE_WINDOW_SD | sdtype, SD_CMD_SD_APP_OP_COND,
                         SDIO_RESPONSE_SHORT, SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
            errorstate = cmd_resp3_error();
            if (errorstate != SD_OK)
                return errorstate;

            /* Get command response */
            response = get_response(SDIO_RESP1);
            validvoltage = ((response & SD_VOLTAGE_WINDOW_WALID) == SD_VOLTAGE_WINDOW_WALID);

            ++count;
        }

        if (count >= SD_MAX_VOLT_TRIAL)
            return SD_INVALID_VOLTRANGE;

        if ((response & SD_HIGH_CAPACITY) == SD_HIGH_CAPACITY)
            m_card_type = HIGH_CAPACITY_SD_CARD;
    } /* else MMC Card */
    else
    {
    }

    return errorstate;
}

SD_ErrorTypedef STM32_SD::power_OFF()
{
    SDIO->POWER = 0x00000000U;
    return SD_OK;
}

SD_ErrorTypedef STM32_SD::send_status(uint32_t *card_status)
{
    SD_ErrorTypedef errorstate = SD_OK;

    if(card_status == nullptr)
        return SD_INVALID_PARAMETER;

    /* Send Status command */
    send_command((m_RCA << 16U), SD_CMD_SEND_STATUS, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_SEND_STATUS);

    if(errorstate != SD_OK)
        return errorstate;

    /* Get SD card status */
    *card_status = get_response(SDIO_RESP1);

    return errorstate;
}

SD_ErrorTypedef STM32_SD::initialize_cards()
{
    SD_ErrorTypedef errorstate = SD_OK;
    uint32_t sd_rca = 1;

    if (get_power_state() == 0)
        /* Power off */
        return SD_REQUEST_NOT_APPLICABLE;

    if (m_card_type != SECURE_DIGITAL_IO_CARD)
    {
        /* Send CMD2 ALL_SEND_CID */
        send_command(0, SD_CMD_ALL_SEND_CID, SDIO_RESPONSE_LONG,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
        if ((errorstate = cmd_resp2_error()) != SD_OK)
            return errorstate;

        m_CID[0] = get_response(SDIO_RESP1);
        m_CID[1] = get_response(SDIO_RESP2);
        m_CID[2] = get_response(SDIO_RESP3);
        m_CID[3] = get_response(SDIO_RESP4);
    }

    if ((m_card_type == STD_CAPACITY_SD_CARD_V1_1) || (m_card_type == STD_CAPACITY_SD_CARD_V2_0) ||
        (m_card_type == SECURE_DIGITAL_IO_COMBO_CARD) || (m_card_type == HIGH_CAPACITY_SD_CARD))
    {
        /* Send CMD3 SET_REL_ADDR with argument 0 */
        /* SD Card publishes its RCA. */
        send_command(0, SD_CMD_SET_REL_ADDR, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
        if ((errorstate = cmd_resp6_error(SD_CMD_SET_REL_ADDR, &sd_rca)) != SD_OK)
            return errorstate;
    }

    if (m_card_type !=SECURE_DIGITAL_IO_CARD)
    {
        /* Get the SD card RCA */
        m_RCA = sd_rca;

        /* Send CMD9 SEND_CSD with argument as card's RCA */
        send_command(m_RCA << 16, SD_CMD_SEND_CSD, SDIO_RESPONSE_LONG,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
        if ((errorstate = cmd_resp2_error()) != SD_OK)
            return errorstate;

        m_CSD[0] = get_response(SDIO_RESP1);
        m_CSD[1] = get_response(SDIO_RESP2);
        m_CSD[2] = get_response(SDIO_RESP3);
        m_CSD[3] = get_response(SDIO_RESP4);
    }

    return errorstate;
}

void STM32_SD::send_command(uint32_t arg, uint32_t cmd_idx,
                            uint32_t resp, uint32_t wait_IT,
                            uint32_t cpsm)
{
    SDIO->ARG = arg;
    MODIFY_REG(SDIO->CMD, CMD_CLEAR_MASK, (cmd_idx | resp |
                                           wait_IT | cpsm));
}

SD_ErrorTypedef STM32_SD::cmd_error()
{
    WAIT_TIMEOUT_EX((!get_flag(SDIO_FLAG_CMDSENT)), SDIO_CMD0TIMEOUT, SD_CMD_RSP_TIMEOUT);
    clear_flag(SDIO_STATIC_FLAGS);
    return SD_OK;
}

SD_ErrorTypedef STM32_SD::cmd_resp7_error()
{
    WAIT_TIMEOUT_EX((!get_flag(SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)),
                 SDIO_CMD0TIMEOUT, SD_CMD_RSP_TIMEOUT);

    if (get_flag(SDIO_FLAG_CTIMEOUT))
    {
        /* Card is not V2.0 compliant or card does not support the set voltage range */
        clear_flag(SDIO_FLAG_CTIMEOUT);
        return SD_CMD_RSP_TIMEOUT;
    }
    if (get_flag(SDIO_FLAG_CMDREND))
    {
        /* Card is SD V2.0 compliant */
        clear_flag(SDIO_FLAG_CMDREND);
        return SD_OK;
    }

    return SD_ERROR;
}

SD_ErrorTypedef STM32_SD::cmd_resp1_error(uint32_t SD_CMD)
{
    while (!get_flag(SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) {}

    if (get_flag(SDIO_FLAG_CTIMEOUT))
    {
        clear_flag(SDIO_FLAG_CTIMEOUT);
        return SD_CMD_RSP_TIMEOUT;
    }
    else if (get_flag(SDIO_FLAG_CCRCFAIL))
    {
        clear_flag(SDIO_FLAG_CCRCFAIL);
        return SD_CMD_CRC_FAIL;
    }

    /* Check response received is of desired command */
    if (get_cmd_response() != SD_CMD)
        return SD_ILLEGAL_CMD;

    /* Clear all the static flags */
    clear_flag(SDIO_STATIC_FLAGS);

    uint32_t response_r1 = get_response(SDIO_RESP1);

    if ((response_r1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
        return SD_OK;

    if ((response_r1 & SD_OCR_ADDR_OUT_OF_RANGE))
        return SD_ADDR_OUT_OF_RANGE;

    if ((response_r1 & SD_OCR_ADDR_MISALIGNED))
        return SD_ADDR_MISALIGNED;

    if ((response_r1 & SD_OCR_BLOCK_LEN_ERR))
        return SD_BLOCK_LEN_ERR;

    if ((response_r1 & SD_OCR_ERASE_SEQ_ERR))
        return SD_ERASE_SEQ_ERR;

    if ((response_r1 & SD_OCR_BAD_ERASE_PARAM))
        return SD_BAD_ERASE_PARAM;

    if ((response_r1 & SD_OCR_WRITE_PROT_VIOLATION))
        return SD_WRITE_PROT_VIOLATION;

    if ((response_r1 & SD_OCR_LOCK_UNLOCK_FAILED))
        return SD_LOCK_UNLOCK_FAILED;

    if ((response_r1 & SD_OCR_COM_CRC_FAILED))
        return SD_COM_CRC_FAILED;

    /*if ((response_r1 & SD_OCR_ILLEGAL_CMD))
        return SD_ILLEGAL_CMD;*/

    if ((response_r1 & SD_OCR_CARD_ECC_FAILED))
        return SD_CARD_ECC_FAILED;

    if ((response_r1 & SD_OCR_CC_ERROR))
        return SD_CC_ERROR;

    if ((response_r1 & SD_OCR_GENERAL_UNKNOWN_ERROR))
        return SD_GENERAL_UNKNOWN_ERROR;

    if ((response_r1 & SD_OCR_STREAM_READ_UNDERRUN))
        return SD_STREAM_READ_UNDERRUN;

    if ((response_r1 & SD_OCR_STREAM_WRITE_OVERRUN))
        return SD_STREAM_WRITE_OVERRUN;

    if ((response_r1 & SD_OCR_CID_CSD_OVERWRITE))
        return SD_CID_CSD_OVERWRITE;

    if ((response_r1 & SD_OCR_WP_ERASE_SKIP))
        return SD_WP_ERASE_SKIP;

    if ((response_r1 & SD_OCR_CARD_ECC_DISABLED))
        return SD_CARD_ECC_DISABLED;

    if ((response_r1 & SD_OCR_ERASE_RESET))
        return SD_ERASE_RESET;

    if ((response_r1 & SD_OCR_AKE_SEQ_ERROR))
        return SD_AKE_SEQ_ERROR;

    return SD_OK;
}

SD_ErrorTypedef STM32_SD::cmd_resp3_error()
{
    while (!get_flag(SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) {}

    if (get_flag(SDIO_FLAG_CTIMEOUT))
    {
        clear_flag(SDIO_FLAG_CTIMEOUT);
        return SD_CMD_RSP_TIMEOUT;
    }

    clear_flag(SDIO_STATIC_FLAGS);
    return SD_OK;
}

SD_ErrorTypedef STM32_SD::cmd_resp2_error()
{
    while (!get_flag(SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) {}

    if (get_flag(SDIO_FLAG_CTIMEOUT))
    {
        clear_flag(SDIO_FLAG_CTIMEOUT);
        return SD_CMD_RSP_TIMEOUT;
    }
    else if (get_flag(SDIO_FLAG_CCRCFAIL))
    {
        clear_flag(SDIO_FLAG_CCRCFAIL);
        return SD_CMD_CRC_FAIL;
    }

    clear_flag(SDIO_STATIC_FLAGS);
    return SD_OK;
}

SD_ErrorTypedef STM32_SD::cmd_resp6_error(uint8_t SD_CMD, uint32_t *pRCA)
{
    while (!get_flag(SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) {}

    if (get_flag(SDIO_FLAG_CTIMEOUT))
    {
        clear_flag(SDIO_FLAG_CTIMEOUT);
        return SD_CMD_RSP_TIMEOUT;
    }
    else if (get_flag(SDIO_FLAG_CCRCFAIL))
    {
        clear_flag(SDIO_FLAG_CCRCFAIL);
        return SD_CMD_CRC_FAIL;
    }

    if (get_cmd_response() !=SD_CMD)
        return SD_ILLEGAL_CMD;

    clear_flag(SDIO_STATIC_FLAGS);

    uint32_t response_r1 = get_response(SDIO_RESP1);

    if ((response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)) == SD_ALLZERO)
    {
        *pRCA = response_r1 >> 16;
        return SD_OK;
    }

    if (response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
        return SD_GENERAL_UNKNOWN_ERROR;
    if (response_r1 & SD_R6_ILLEGAL_CMD)
        return SD_ILLEGAL_CMD;
    if (response_r1 & SD_R6_COM_CRC_FAILED)
        return SD_COM_CRC_FAILED;

    return SD_OK;
}

SD_ErrorTypedef STM32_SD::data_config(uint32_t time_out, uint32_t dat_len,
                                      uint32_t block_size, uint32_t transf_dir,
                                      uint32_t transf_mode, uint32_t DPSM)
{
    SDIO->DTIMER = time_out;
    SDIO->DLEN = dat_len;
    uint32_t tmpreg = block_size | transf_dir | transf_mode | DPSM;
    MODIFY_REG(SDIO->DCTRL, DCTRL_CLEAR_MASK, tmpreg);
    return SD_OK;
}

SD_ErrorTypedef STM32_SD::stop_transfer()
{
    send_command(0, SD_CMD_STOP_TRANSMISSION, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    return cmd_resp1_error(SD_CMD_STOP_TRANSMISSION);
}

SD_ErrorTypedef STM32_SD::get_card_info()
{
    SD_ErrorTypedef errorstate = SD_OK;

    m_card_info.CardType = m_card_type;
    m_card_info.RCA = m_RCA;

    /* Byte 1 */
    uint32_t tmp = (m_CSD[0U] & 0x00FF0000U) >> 16U;
    m_card_info.SD_csd.TAAC = (uint8_t)tmp;

    /* Byte 2 */
    tmp = (m_CSD[0U] & 0x0000FF00U) >> 8U;
    m_card_info.SD_csd.NSAC = (uint8_t)tmp;

    /* Byte 3 */
    tmp = m_CSD[0U] & 0x000000FFU;
    m_card_info.SD_csd.MaxBusClkFrec = (uint8_t)tmp;

    /* Byte 4 */
    tmp = (m_CSD[1U] & 0xFF000000U) >> 24U;
    m_card_info.SD_csd.CardComdClasses = (uint16_t)(tmp << 4U);

    /* Byte 5 */
    tmp = (m_CSD[1U] & 0x00FF0000U) >> 16U;
    m_card_info.SD_csd.CardComdClasses |= (uint16_t)((tmp & 0xF0) >> 4U);
    m_card_info.SD_csd.RdBlockLen       = (uint8_t)(tmp & 0x0FU);

    /* Byte 6 */
    tmp = (m_CSD[1U] & 0x0000FF00U) >> 8U;
    m_card_info.SD_csd.PartBlockRead   = (uint8_t)((tmp & 0x80U) >> 7U);
    m_card_info.SD_csd.WrBlockMisalign = (uint8_t)((tmp & 0x40U) >> 6U);
    m_card_info.SD_csd.RdBlockMisalign = (uint8_t)((tmp & 0x20U) >> 5U);
    m_card_info.SD_csd.DSRImpl         = (uint8_t)((tmp & 0x10U) >> 4U);
    m_card_info.SD_csd.Reserved2       = 0U; /*!< Reserved */

    if ((m_card_type == STD_CAPACITY_SD_CARD_V1_1) || (m_card_type == STD_CAPACITY_SD_CARD_V2_0))
    {
        m_card_info.SD_csd.DeviceSize = (tmp & 0x03U) << 10U;

        /* Byte 7 */
        tmp = (uint8_t)(m_CSD[1U] & 0x000000FFU);
        m_card_info.SD_csd.DeviceSize |= (tmp) << 2U;

        /* Byte 8 */
        tmp = (uint8_t)((m_CSD[2U] & 0xFF000000U) >> 24U);
        m_card_info.SD_csd.DeviceSize |= (tmp & 0xC0U) >> 6U;

        m_card_info.SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38U) >> 3U;
        m_card_info.SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07U);

        /* Byte 9 */
        tmp = (uint8_t)((m_CSD[2U] & 0x00FF0000U) >> 16U);
        m_card_info.SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0U) >> 5U;
        m_card_info.SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1CU) >> 2U;
        m_card_info.SD_csd.DeviceSizeMul      = (tmp & 0x03U) << 1U;
        /* Byte 10 */
        tmp = (uint8_t)((m_CSD[2U] & 0x0000FF00U) >> 8U);
        m_card_info.SD_csd.DeviceSizeMul |= (tmp & 0x80U) >> 7U;

        m_card_info.CardCapacity  = (m_card_info.SD_csd.DeviceSize + 1U) ;
        m_card_info.CardCapacity *= (1U << (m_card_info.SD_csd.DeviceSizeMul + 2U));
        m_card_info.CardBlockSize = 1U << (m_card_info.SD_csd.RdBlockLen);
        m_card_info.CardCapacity *= m_card_info.CardBlockSize;
    }
    else if (m_card_type == HIGH_CAPACITY_SD_CARD)
    {
        /* Byte 7 */
        tmp = (uint8_t)(m_CSD[1U] & 0x000000FFU);
        m_card_info.SD_csd.DeviceSize = (tmp & 0x3FU) << 16U;

        /* Byte 8 */
        tmp = (uint8_t)((m_CSD[2U] & 0xFF000000U) >> 24U);

        m_card_info.SD_csd.DeviceSize |= (tmp << 8U);

        /* Byte 9 */
        tmp = (uint8_t)((m_CSD[2U] & 0x00FF0000U) >> 16U);

        m_card_info.SD_csd.DeviceSize |= (tmp);

        /* Byte 10 */
        tmp = (uint8_t)((m_CSD[2U] & 0x0000FF00U) >> 8U);

        m_card_info.CardCapacity = (uint64_t)((((uint64_t)m_card_info.SD_csd.DeviceSize + 1U)) * 512U * 1024U);
        m_card_info.CardBlockSize = 512U;
    }
    else
    {
        /* Not supported card type */
        errorstate = SD_ERROR;
    }

    m_card_info.SD_csd.EraseGrSize = (tmp & 0x40U) >> 6U;
    m_card_info.SD_csd.EraseGrMul  = (tmp & 0x3FU) << 1U;

    /* Byte 11 */
    tmp = (uint8_t)(m_CSD[2U] & 0x000000FFU);
    m_card_info.SD_csd.EraseGrMul     |= (tmp & 0x80U) >> 7U;
    m_card_info.SD_csd.WrProtectGrSize = (tmp & 0x7FU);

    /* Byte 12 */
    tmp = (uint8_t)((m_CSD[3U] & 0xFF000000U) >> 24U);
    m_card_info.SD_csd.WrProtectGrEnable = (tmp & 0x80U) >> 7U;
    m_card_info.SD_csd.ManDeflECC        = (tmp & 0x60U) >> 5U;
    m_card_info.SD_csd.WrSpeedFact       = (tmp & 0x1CU) >> 2U;
    m_card_info.SD_csd.MaxWrBlockLen     = (tmp & 0x03U) << 2U;

    /* Byte 13 */
    tmp = (uint8_t)((m_CSD[3U] & 0x00FF0000U) >> 16U);
    m_card_info.SD_csd.MaxWrBlockLen      |= (tmp & 0xC0U) >> 6U;
    m_card_info.SD_csd.WriteBlockPaPartial = (tmp & 0x20U) >> 5U;
    m_card_info.SD_csd.Reserved3           = 0U;
    m_card_info.SD_csd.ContentProtectAppli = (tmp & 0x01U);

    /* Byte 14 */
    tmp = (uint8_t)((m_CSD[3U] & 0x0000FF00U) >> 8U);
    m_card_info.SD_csd.FileFormatGrouop = (tmp & 0x80U) >> 7U;
    m_card_info.SD_csd.CopyFlag         = (tmp & 0x40U) >> 6U;
    m_card_info.SD_csd.PermWrProtect    = (tmp & 0x20U) >> 5U;
    m_card_info.SD_csd.TempWrProtect    = (tmp & 0x10U) >> 4U;
    m_card_info.SD_csd.FileFormat       = (tmp & 0x0CU) >> 2U;
    m_card_info.SD_csd.ECC              = (tmp & 0x03U);

    /* Byte 15 */
    tmp = (uint8_t)(m_CSD[3U] & 0x000000FFU);
    m_card_info.SD_csd.CSD_CRC   = (tmp & 0xFEU) >> 1U;
    m_card_info.SD_csd.Reserved4 = 1U;

    /* Byte 0 */
    tmp = (uint8_t)((m_CID[0U] & 0xFF000000U) >> 24U);
    m_card_info.SD_cid.ManufacturerID = tmp;

    /* Byte 1 */
    tmp = (uint8_t)((m_CID[0U] & 0x00FF0000U) >> 16U);
    m_card_info.SD_cid.OEM_AppliID = tmp << 8U;

    /* Byte 2 */
    tmp = (uint8_t)((m_CID[0U] & 0x0000FF00U) >> 8U);
    m_card_info.SD_cid.OEM_AppliID |= tmp;

    /* Byte 3 */
    tmp = (uint8_t)(m_CID[0U] & 0x000000FFU);
    m_card_info.SD_cid.ProdName1 = tmp << 24U;

    /* Byte 4 */
    tmp = (uint8_t)((m_CID[1U] & 0xFF000000U) >> 24U);
    m_card_info.SD_cid.ProdName1 |= tmp << 16U;

    /* Byte 5 */
    tmp = (uint8_t)((m_CID[1U] & 0x00FF0000U) >> 16U);
    m_card_info.SD_cid.ProdName1 |= tmp << 8U;

    /* Byte 6 */
    tmp = (uint8_t)((m_CID[1U] & 0x0000FF00U) >> 8U);
    m_card_info.SD_cid.ProdName1 |= tmp;

    /* Byte 7 */
    tmp = (uint8_t)(m_CID[1U] & 0x000000FFU);
    m_card_info.SD_cid.ProdName2 = tmp;

    /* Byte 8 */
    tmp = (uint8_t)((m_CID[2U] & 0xFF000000U) >> 24U);
    m_card_info.SD_cid.ProdRev = tmp;

    /* Byte 9 */
    tmp = (uint8_t)((m_CID[2U] & 0x00FF0000U) >> 16U);
    m_card_info.SD_cid.ProdSN = tmp << 24U;

    /* Byte 10 */
    tmp = (uint8_t)((m_CID[2U] & 0x0000FF00U) >> 8U);
    m_card_info.SD_cid.ProdSN |= tmp << 16U;

    /* Byte 11 */
    tmp = (uint8_t)(m_CID[2U] & 0x000000FFU);
    m_card_info.SD_cid.ProdSN |= tmp << 8U;

    /* Byte 12 */
    tmp = (uint8_t)((m_CID[3U] & 0xFF000000U) >> 24U);
    m_card_info.SD_cid.ProdSN |= tmp;

    /* Byte 13 */
    tmp = (uint8_t)((m_CID[3U] & 0x00FF0000U) >> 16U);
    m_card_info.SD_cid.Reserved1   |= (tmp & 0xF0U) >> 4U;
    m_card_info.SD_cid.ManufactDate = (tmp & 0x0FU) << 8U;

    /* Byte 14 */
    tmp = (uint8_t)((m_CID[3U] & 0x0000FF00U) >> 8U);
    m_card_info.SD_cid.ManufactDate |= tmp;

    /* Byte 15 */
    tmp = (uint8_t)(m_CID[3U] & 0x000000FFU);
    m_card_info.SD_cid.CID_CRC   = (tmp & 0xFEU) >> 1U;
    m_card_info.SD_cid.Reserved2 = 1U;

    return errorstate;
}

SD_ErrorTypedef STM32_SD::select_deselect(uint32_t addr)
{
    send_command(addr, SD_CMD_SEL_DESEL_CARD, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);
    return cmd_resp1_error(SD_CMD_SEL_DESEL_CARD);
}

SD_ErrorTypedef STM32_SD::is_card_programming(uint8_t *status)
{
    SD_ErrorTypedef errorstate = SD_OK;
    __IO uint32_t responseR1 = 0U;

    send_command(m_RCA << 16U, SD_CMD_SEND_STATUS, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    while (!get_flag(SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) {}

    if (get_flag(SDIO_FLAG_CTIMEOUT))
    {
        clear_flag(SDIO_FLAG_CTIMEOUT);
        return SD_CMD_RSP_TIMEOUT;
    }
    else if (get_flag(SDIO_FLAG_CCRCFAIL))
    {
        clear_flag(SDIO_FLAG_CCRCFAIL);
        return SD_CMD_CRC_FAIL;
    }
    else
    {
        /* No error flag set */
    }

    /* Check response received is of desired command */
    if (get_cmd_response() != SD_CMD_SEND_STATUS)
        return SD_ILLEGAL_CMD;

    /* Clear all the static flags */
    clear_flag(SDIO_STATIC_FLAGS);


    /* We have received response, retrieve it for analysis */
    responseR1 = get_response(SDIO_RESP1);

    /* Find out card status */
    *status = (uint8_t)((responseR1 >> 9U) & 0x0000000FU);

    if ((responseR1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
        return errorstate;

    if ((responseR1 & SD_OCR_ADDR_OUT_OF_RANGE) == SD_OCR_ADDR_OUT_OF_RANGE)
        return(SD_ADDR_OUT_OF_RANGE);

    if ((responseR1 & SD_OCR_ADDR_MISALIGNED) == SD_OCR_ADDR_MISALIGNED)
        return(SD_ADDR_MISALIGNED);

    if ((responseR1 & SD_OCR_BLOCK_LEN_ERR) == SD_OCR_BLOCK_LEN_ERR)
        return(SD_BLOCK_LEN_ERR);

    if ((responseR1 & SD_OCR_ERASE_SEQ_ERR) == SD_OCR_ERASE_SEQ_ERR)
        return(SD_ERASE_SEQ_ERR);

    if ((responseR1 & SD_OCR_BAD_ERASE_PARAM) == SD_OCR_BAD_ERASE_PARAM)
        return(SD_BAD_ERASE_PARAM);

    if ((responseR1 & SD_OCR_WRITE_PROT_VIOLATION) == SD_OCR_WRITE_PROT_VIOLATION)
        return(SD_WRITE_PROT_VIOLATION);

    if ((responseR1 & SD_OCR_LOCK_UNLOCK_FAILED) == SD_OCR_LOCK_UNLOCK_FAILED)
        return(SD_LOCK_UNLOCK_FAILED);

    if ((responseR1 & SD_OCR_COM_CRC_FAILED) == SD_OCR_COM_CRC_FAILED)
        return(SD_COM_CRC_FAILED);

    if ((responseR1 & SD_OCR_ILLEGAL_CMD) == SD_OCR_ILLEGAL_CMD)
        return(SD_ILLEGAL_CMD);

    if ((responseR1 & SD_OCR_CARD_ECC_FAILED) == SD_OCR_CARD_ECC_FAILED)
        return(SD_CARD_ECC_FAILED);

    if ((responseR1 & SD_OCR_CC_ERROR) == SD_OCR_CC_ERROR)
        return(SD_CC_ERROR);

    if ((responseR1 & SD_OCR_GENERAL_UNKNOWN_ERROR) == SD_OCR_GENERAL_UNKNOWN_ERROR)
        return(SD_GENERAL_UNKNOWN_ERROR);

    if ((responseR1 & SD_OCR_STREAM_READ_UNDERRUN) == SD_OCR_STREAM_READ_UNDERRUN)
        return(SD_STREAM_READ_UNDERRUN);

    if ((responseR1 & SD_OCR_STREAM_WRITE_OVERRUN) == SD_OCR_STREAM_WRITE_OVERRUN)
        return(SD_STREAM_WRITE_OVERRUN);

    if ((responseR1 & SD_OCR_CID_CSD_OVERWRITE) == SD_OCR_CID_CSD_OVERWRITE)
        return(SD_CID_CSD_OVERWRITE);

    if ((responseR1 & SD_OCR_WP_ERASE_SKIP) == SD_OCR_WP_ERASE_SKIP)
        return(SD_WP_ERASE_SKIP);

    if ((responseR1 & SD_OCR_CARD_ECC_DISABLED) == SD_OCR_CARD_ECC_DISABLED)
        return(SD_CARD_ECC_DISABLED);

    if ((responseR1 & SD_OCR_ERASE_RESET) == SD_OCR_ERASE_RESET)
        return(SD_ERASE_RESET);

    if ((responseR1 & SD_OCR_AKE_SEQ_ERROR) == SD_OCR_AKE_SEQ_ERROR)
        return(SD_AKE_SEQ_ERROR);

    return errorstate;
}

SD_ErrorTypedef STM32_SD::wide_bus_enable()
{
    SD_ErrorTypedef errorstate = SD_OK;

    uint32_t scr[2U] = {0U, 0U};

    if ((get_response(SDIO_RESP1) & SD_CARD_LOCKED) == SD_CARD_LOCKED)
        return SD_LOCK_UNLOCK_FAILED;

    /* Get SCR Register */
    errorstate = find_SCR(scr);

    if (errorstate != SD_OK)
        return errorstate;

    /* If requested card supports wide bus operation */
    if ((scr[1U] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO)
    {
        /* Send CMD55 APP_CMD with argument as card's RCA.*/
        send_command(m_RCA << 16U, SD_CMD_APP_CMD, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

        /* Check for error conditions */
        errorstate = cmd_resp1_error(SD_CMD_APP_CMD);

        if(errorstate != SD_OK)
            return errorstate;

        /* Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
        send_command(2, SD_CMD_APP_SD_SET_BUSWIDTH, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

        /* Check for error conditions */
        errorstate = cmd_resp1_error(SD_CMD_APP_SD_SET_BUSWIDTH);
    }
    else
        return SD_REQUEST_NOT_APPLICABLE;

    return errorstate;
}

SD_ErrorTypedef STM32_SD::wide_bus_disable()
{
    SD_ErrorTypedef errorstate = SD_OK;

    uint32_t scr[2U] = {0U, 0U};

    if ((get_response(SDIO_RESP1) & SD_CARD_LOCKED) == SD_CARD_LOCKED)
        return SD_LOCK_UNLOCK_FAILED;

    /* Get SCR Register */
    errorstate = find_SCR(scr);

    if (errorstate != SD_OK)
        return errorstate;

    /* If requested card supports 1 bit mode operation */
    if ((scr[1U] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO)
    {
        /* Send CMD55 APP_CMD with argument as card's RCA */
        send_command(m_RCA << 16U, SD_CMD_APP_CMD, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

        /* Check for error conditions */
        errorstate = cmd_resp1_error(SD_CMD_APP_CMD);

        if(errorstate != SD_OK)
            return errorstate;

        /* Send ACMD6 APP_CMD with argument as 0 for single bus mode */
        send_command(0, SD_CMD_APP_SD_SET_BUSWIDTH, SDIO_RESPONSE_SHORT,
                     SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

        /* Check for error conditions */
        errorstate = cmd_resp1_error(SD_CMD_APP_SD_SET_BUSWIDTH);
    }
    else
        return SD_REQUEST_NOT_APPLICABLE;

    return errorstate;
}

SD_ErrorTypedef STM32_SD::find_SCR(uint32_t *scr)
{
    SD_ErrorTypedef errorstate = SD_OK;
    uint32_t index = 0U;
    uint32_t tempscr[2U] = {0U, 0U};

    /* Set Block Size To 8 Bytes */
    /* Send CMD55 APP_CMD with argument as card's RCA */
    send_command(8, SD_CMD_SET_BLOCKLEN, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_SET_BLOCKLEN);

    if (errorstate != SD_OK)
        return errorstate;

    /* Send CMD55 APP_CMD with argument as card's RCA */
    send_command((m_RCA << 16U), SD_CMD_APP_CMD, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_APP_CMD);

    if (errorstate != SD_OK)
        return errorstate;

    data_config(SD_DATATIMEOUT, 8U,
                SDIO_DATABLOCK_SIZE_8B, SDIO_TRANSFER_DIR_TO_SDIO,
                SDIO_TRANSFER_MODE_BLOCK, SDIO_DPSM_ENABLE);

    /* Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
    send_command(0, SD_CMD_SD_APP_SEND_SCR, SDIO_RESPONSE_SHORT,
                 SDIO_WAIT_NO, SDIO_CPSM_ENABLE);

    /* Check for error conditions */
    errorstate = cmd_resp1_error(SD_CMD_SD_APP_SEND_SCR);

    if (errorstate != SD_OK)
        return errorstate;

    #ifdef SDIO_STA_STBITERR
    while(!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))
    #else /* SDIO_STA_STBITERR not defined */
    while(!get_flag(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND))
    #endif /* SDIO_STA_STBITERR */
    {
        if(get_flag(SDIO_FLAG_RXDAVL))
        {
            *(tempscr + index) = read_FIFO();
            index++;
        }
    }

    if (get_flag(SDIO_FLAG_DTIMEOUT))
    {
        clear_flag(SDIO_FLAG_DTIMEOUT);
        return SD_DATA_TIMEOUT;
    }
    else if (get_flag(SDIO_FLAG_DCRCFAIL))
    {
        clear_flag(SDIO_FLAG_DCRCFAIL);
        return SD_DATA_TIMEOUT;
    }
    else if (get_flag(SDIO_FLAG_RXOVERR))
    {
        clear_flag(SDIO_FLAG_RXOVERR);
        return SD_RX_OVERRUN;
    }
    #ifdef SDIO_STA_STBITERR
    else if (get_flag(SDIO_FLAG_STBITERR))
    {
        clear_flag(SDIO_FLAG_STBITERR);
        return SD_START_BIT_ERR;
    }
    #endif /* SDIO_STA_STBITERR */
    else
    {
        /* No error flag set */
    }

    /* Clear all the static flags */
    clear_flag(SDIO_STATIC_FLAGS);

    *(scr + 1U) = ((tempscr[0U] & SD_0TO7BITS) << 24U)  | ((tempscr[0U] & SD_8TO15BITS) << 8U) |
                  ((tempscr[0U] & SD_16TO23BITS) >> 8U) | ((tempscr[0U] & SD_24TO31BITS) >> 24U);

    *(scr) = ((tempscr[1U] & SD_0TO7BITS) << 24U)  | ((tempscr[1U] & SD_8TO15BITS) << 8U) |
             ((tempscr[1U] & SD_16TO23BITS) >> 8U) | ((tempscr[1U] & SD_24TO31BITS) >> 24U);

    return errorstate;
}

SD_CardStateTypedef STM32_SD::get_state()
{
    uint32_t resp1 = 0U;

    if (send_status(&resp1) != SD_OK)
        return SD_CARD_ERROR;
    else
        return (SD_CardStateTypedef)((resp1 >> 9U) & 0x0FU);
}

void ISR::SDIO_IRQ()
{
    ///TODO
}
