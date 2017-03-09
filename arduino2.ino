
#include <TimeLib.h> 

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiUdp.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager


#include "FastLED.h"
#include <Ticker.h>

#define NUM_LEDS 124
#define DATA_PIN 14



//Text Matrix:
//   ESOISTSPUNKT
//   IZWANZIGZEHN
//   WHALBVIERTEL
//   FÜNFENACHVOR
//   HALBGDSIEBEN
//   ELFVIERNEUNG
//   EINSACHTZWEI
//   SECHSCZWÖLFL
//   DREIFÜNFOUND
//   ZEHNHZWANZIG


//Word [0]=Startpos [1]=lengh of LED index
//Word: "ES"
const uint16_t word_0[]  = { 0, 2 };
//Word: "I"
const uint16_t word_1[]  = { 3, 1 };
//Word: "IST"
const uint16_t word_2[]  = { 3, 3 };
//Word: "PUNKT"
const uint16_t word_3[]  = { 7, 5 };
//Word: "NACH"
const uint16_t word_4[]  = { 41, 4 };
//Word: "VIER"
const uint16_t word_5[]  = { 63, 4 };
//Word: "ZEHN"
const uint16_t word_6[]  = { 20, 4 };
//Word: "VIERTEL"
const uint16_t word_7[]  = { 29, 7 };
//Word: "ZWANZIG"
const uint16_t word_8[]  = { 13, 7 };
//Word: "VOR"
const uint16_t word_9[]  = { 45, 3 };
//Word: "HALB"
const uint16_t word_10[]  = { 48, 4 };
//Word: "L"
const uint16_t word_11[]  = { 27, 1 };
//Word: "HALB"
const uint16_t word_12[]  = { 25, 4 };
//Word: "EINS"
const uint16_t word_13[]  = { 72, 4 };
//Word: "EIN"
const uint16_t word_14[]  = { 72, 3 };
//Word: "ZWEI"
const uint16_t word_15[]  = { 80, 4 };
//Word: "DREI"
const uint16_t word_16[]  = { 96, 4 };
//Word: "D"
const uint16_t word_17[]  = { 96, 1 };
//Word: "FÜNF"
const uint16_t word_18[]  = { 36, 4 };
//Word: "FÜNF"
const uint16_t word_19[]  = { 100, 4 };
//Word: "SECH"
const uint16_t word_20[]  = { 84, 4 };
//Word: "SECHS"
const uint16_t word_21[]  = { 84, 5 };
//Word: "SIEB"
const uint16_t word_22[]  = { 54, 4 };
//Word: "SIEBEN"
const uint16_t word_23[]  = { 54, 6 };
//Word: "ACHT"
const uint16_t word_24[]  = { 76, 4 };
//Word: "NEUN"
const uint16_t word_25[]  = { 67, 4 };
//Word: "ZEHN"
const uint16_t word_26[]  = { 108, 4 };
//Word: "ELF"
const uint16_t word_27[]  = { 60, 3 };
//Word: "ZWÖLF"
const uint16_t word_28[]  = { 90, 5 };
//Word: "ZWANZIG"
const uint16_t word_29[]  = { 113, 7 };
//Word: "UND"
const uint16_t word_30[]  = { 105, 3 };
//Word Table: 
const uint16_t* const word_table[]  = { word_0, word_1, word_2, word_3, word_4, word_5, word_6, word_7, word_8, word_9, word_10, word_11, word_12, word_13, word_14, word_15, word_16, word_17, word_18, word_19, word_20, word_21, word_22, word_23, word_24, word_25, word_26, word_27, word_28, word_29, word_30};
//Pattern: "ES IST ZEHN NACH DREI UND ZWANZIG " 23:10 23:11 23:12 23:13 23:14 
const uint8_t pattern_0[]  = { 0, 2, 4, 6, 16, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB ZWEI UND ZWANZIG " 21:35 21:36 21:37 21:38 21:39 
const uint8_t pattern_1[]  = { 0, 2, 4, 10, 15, 18, 29, 30, 0xFF };
//Pattern: "ES IST VIERTEL VOR ACHT ZEHN " 17:45 17:46 17:47 17:48 17:49 
const uint8_t pattern_2[]  = { 0, 2, 7, 9, 24, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH SECH ZEHN " 16:05 16:06 16:07 16:08 16:09 
const uint8_t pattern_3[]  = { 0, 2, 4, 18, 20, 26, 0xFF };
//Pattern: "ES IST ZEHN VOR DREI ZEHN " 12:50 12:51 12:52 12:53 12:54 
const uint8_t pattern_4[]  = { 0, 2, 6, 9, 16, 0xFF };
//Pattern: "ES IST VIERTEL NACH ZWEI UND ZWANZIG " 22:15 22:16 22:17 22:18 22:19 
const uint8_t pattern_5[]  = { 0, 2, 4, 7, 15, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB ZWÖLF " 11:25 11:26 11:27 11:28 11:29 
const uint8_t pattern_6[]  = { 0, 2, 9, 10, 18, 28, 0xFF };
//Pattern: "ES IST PUNKT SIEB ZEHN " 17:00 17:01 17:02 17:03 17:04 
const uint8_t pattern_7[]  = { 0, 2, 3, 22, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB DREI ZEHN " 12:25 12:26 12:27 12:28 12:29 
const uint8_t pattern_8[]  = { 0, 2, 9, 10, 16, 18, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR FÜNF " 04:40 04:41 04:42 04:43 04:44 
const uint8_t pattern_9[]  = { 0, 2, 8, 9, 19, 0xFF };
//Pattern: "ES IST ZWANZIG NACH ACHT " 08:20 08:21 08:22 08:23 08:24 
const uint8_t pattern_10[]  = { 0, 2, 4, 8, 24, 0xFF };
//Pattern: "ES IST VIERTEL NACH DREI UND ZWANZIG " 23:15 23:16 23:17 23:18 23:19 
const uint8_t pattern_11[]  = { 0, 2, 4, 7, 16, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH VIER UND ZWANZIG " 00:05 00:06 00:07 00:08 00:09 
const uint8_t pattern_12[]  = { 0, 2, 4, 5, 18, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB DREI UND ZWANZIG " 22:35 22:36 22:37 22:38 22:39 
const uint8_t pattern_13[]  = { 0, 2, 4, 10, 16, 18, 29, 30, 0xFF };
//Pattern: "ES IST HALB VIER ZEHN " 13:30 13:31 13:32 13:33 13:34 
const uint8_t pattern_14[]  = { 0, 2, 5, 12, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB ACHT ZEHN " 17:25 17:26 17:27 17:28 17:29 
const uint8_t pattern_15[]  = { 0, 2, 9, 10, 18, 24, 26, 0xFF };
//Pattern: "ES IST VIERTEL VOR DREI ZEHN " 12:45 12:46 12:47 12:48 12:49 
const uint8_t pattern_16[]  = { 0, 2, 7, 9, 16, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR VIER ZEHN " 13:55 13:56 13:57 13:58 13:59 
const uint8_t pattern_17[]  = { 0, 2, 5, 9, 18, 26, 0xFF };
//Pattern: "ES IST ZWANZIG NACH ZWEI " 02:20 02:21 02:22 02:23 02:24 
const uint8_t pattern_18[]  = { 0, 2, 4, 8, 15, 0xFF };
//Pattern: "ES IST ZWANZIG NACH DREI " 03:20 03:21 03:22 03:23 03:24 
const uint8_t pattern_19[]  = { 0, 2, 4, 8, 16, 0xFF };
//Pattern: "ES IST VIERTEL NACH ACHT " 08:15 08:16 08:17 08:18 08:19 
const uint8_t pattern_20[]  = { 0, 2, 4, 7, 24, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB ZWEI " 01:35 01:36 01:37 01:38 01:39 
const uint8_t pattern_21[]  = { 0, 2, 4, 10, 15, 18, 0xFF };
//Pattern: "ES IST ZWANZIG NACH ZWANZIG " 20:20 20:21 20:22 20:23 20:24 
const uint8_t pattern_22[]  = { 0, 2, 4, 8, 29, 0xFF };
//Pattern: "ES IST ZEHN NACH ELF " 11:10 11:11 11:12 11:13 11:14 
const uint8_t pattern_23[]  = { 0, 2, 4, 6, 27, 0xFF };
//Pattern: "ES IST ZEHN NACH NEUN ZEHN " 19:10 19:11 19:12 19:13 19:14 
const uint8_t pattern_24[]  = { 0, 2, 4, 6, 25, 26, 0xFF };
//Pattern: "ES IST HALB DREI UND ZWANZIG " 22:30 22:31 22:32 22:33 22:34 
const uint8_t pattern_25[]  = { 0, 2, 12, 16, 29, 30, 0xFF };
//Pattern: "ES IST PUNKT ZWEI " 02:00 02:01 02:02 02:03 02:04 
const uint8_t pattern_26[]  = { 0, 2, 3, 15, 0xFF };
//Pattern: "ES IST ZWANZIG VOR ZEHN " 09:40 09:41 09:42 09:43 09:44 
const uint8_t pattern_27[]  = { 0, 2, 8, 9, 16, 0xFF };
//Pattern: "ES IST FÜNF NACH ZEHN " 10:05 10:06 10:07 10:08 10:09 
const uint8_t pattern_28[]  = { 0, 2, 4, 18, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB ELF " 10:35 10:36 10:37 10:38 10:39 
const uint8_t pattern_29[]  = { 0, 2, 4, 10, 18, 27, 0xFF };
//Pattern: "ES IST VIERTEL NACH DREI " 03:15 03:16 03:17 03:18 03:19 
const uint8_t pattern_30[]  = { 0, 2, 4, 7, 16, 0xFF };
//Pattern: "ES IST FÜNF NACH VIER " 04:05 04:06 04:07 04:08 04:09 
const uint8_t pattern_31[]  = { 0, 2, 4, 5, 18, 0xFF };
//Pattern: "ES IST ZEHN VOR ACHT ZEHN " 17:50 17:51 17:52 17:53 17:54 
const uint8_t pattern_32[]  = { 0, 2, 6, 9, 24, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR SECHS " 05:55 05:56 05:57 05:58 05:59 
const uint8_t pattern_33[]  = { 0, 2, 9, 18, 21, 0xFF };
//Pattern: "ES IST PUNKT ELF " 11:00 11:01 11:02 11:03 11:04 
const uint8_t pattern_34[]  = { 0, 2, 3, 27, 0xFF };
//Pattern: "ES IST ZEHN VOR ZWÖLF " 11:50 11:51 11:52 11:53 11:54 
const uint8_t pattern_35[]  = { 0, 2, 6, 9, 28, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB NEUN ZEHN " 18:35 18:36 18:37 18:38 18:39 
const uint8_t pattern_36[]  = { 0, 2, 4, 10, 18, 25, 26, 0xFF };
//Pattern: "ES IST PUNKT NEUN ZEHN " 19:00 19:01 19:02 19:03 19:04 
const uint8_t pattern_37[]  = { 0, 2, 3, 25, 26, 0xFF };
//Pattern: "ES IST VIERTEL VOR FÜNF " 04:45 04:46 04:47 04:48 04:49 
const uint8_t pattern_38[]  = { 0, 2, 7, 9, 19, 0xFF };
//Pattern: "ES IST ZWANZIG NACH ZWEI UND ZWANZIG " 22:20 22:21 22:22 22:23 22:24 
const uint8_t pattern_39[]  = { 0, 2, 4, 8, 15, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF VOR ZWEI UND ZWANZIG " 21:55 21:56 21:57 21:58 21:59 
const uint8_t pattern_40[]  = { 0, 2, 9, 15, 18, 29, 30, 0xFF };
//Pattern: "ES IST ZWANZIG NACH DREI UND ZWANZIG " 23:20 23:21 23:22 23:23 23:24 
const uint8_t pattern_41[]  = { 0, 2, 4, 8, 16, 29, 30, 0xFF };
//Pattern: "ES IST HALB ZWEI UND ZWANZIG " 21:30 21:31 21:32 21:33 21:34 
const uint8_t pattern_42[]  = { 0, 2, 12, 15, 29, 30, 0xFF };
//Pattern: "ES IST HALB NEUN " 08:30 08:31 08:32 08:33 08:34 
const uint8_t pattern_43[]  = { 0, 2, 12, 25, 0xFF };
//Pattern: "ES IST VIERTEL NACH SIEB ZEHN " 17:15 17:16 17:17 17:18 17:19 
const uint8_t pattern_44[]  = { 0, 2, 4, 7, 22, 26, 0xFF };
//Pattern: "ES IST PUNKT ZWEI UND ZWANZIG " 22:00 22:01 22:02 22:03 22:04 
const uint8_t pattern_45[]  = { 0, 2, 3, 15, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH EINS " 01:05 01:06 01:07 01:08 01:09 
const uint8_t pattern_46[]  = { 0, 2, 4, 13, 18, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB SIEB ZEHN " 16:35 16:36 16:37 16:38 16:39 
const uint8_t pattern_47[]  = { 0, 2, 4, 10, 18, 22, 26, 0xFF };
//Pattern: "ES IST ZEHN NACH SECHS " 06:10 06:11 06:12 06:13 06:14 
const uint8_t pattern_48[]  = { 0, 2, 4, 6, 21, 0xFF };
//Pattern: "ES IST FÜNF VOR SIEBEN " 06:55 06:56 06:57 06:58 06:59 
const uint8_t pattern_49[]  = { 0, 2, 9, 18, 23, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB VIER ZEHN " 13:35 13:36 13:37 13:38 13:39 
const uint8_t pattern_50[]  = { 0, 2, 4, 5, 10, 18, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH FÜNF ZEHN " 15:05 15:06 15:07 15:08 15:09 
const uint8_t pattern_51[]  = { 0, 2, 4, 18, 19, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR VIER UND ZWANZIG " 23:40 23:41 23:42 23:43 23:44 
const uint8_t pattern_52[]  = { 0, 2, 5, 8, 9, 29, 30, 0xFF };
//Pattern: "ES IST VIERTEL VOR EINS " 00:45 00:46 00:47 00:48 00:49 
const uint8_t pattern_53[]  = { 0, 2, 7, 9, 13, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB EINS " 00:25 00:26 00:27 00:28 00:29 
const uint8_t pattern_54[]  = { 0, 2, 9, 10, 13, 18, 0xFF };
//Pattern: "ES IST ZEHN NACH ZWEI " 02:10 02:11 02:12 02:13 02:14 
const uint8_t pattern_55[]  = { 0, 2, 4, 6, 15, 0xFF };
//Pattern: "ES IST PUNKT DREI ZEHN " 13:00 13:01 13:02 13:03 13:04 
const uint8_t pattern_56[]  = { 0, 2, 3, 16, 26, 0xFF };
//Pattern: "ES IST VIERTEL VOR SIEB ZEHN " 16:45 16:46 16:47 16:48 16:49 
const uint8_t pattern_57[]  = { 0, 2, 7, 9, 22, 26, 0xFF };
//Pattern: "I L D" 21:15 
const uint8_t pattern_58[]  = { 1, 11, 17, 0xFF };
//Pattern: "ES IST FÜNF NACH EIN UND ZWANZIG " 21:05 21:06 21:07 21:08 21:09 
const uint8_t pattern_59[]  = { 0, 2, 4, 14, 18, 29, 30, 0xFF };
//Pattern: "ES IST HALB ZWEI " 01:30 01:31 01:32 01:33 01:34 
const uint8_t pattern_60[]  = { 0, 2, 12, 15, 0xFF };
//Pattern: "ES IST PUNKT ACHT ZEHN " 18:00 18:01 18:02 18:03 18:04 
const uint8_t pattern_61[]  = { 0, 2, 3, 24, 26, 0xFF };
//Pattern: "ES IST ZEHN VOR ZEHN " 09:50 09:51 09:52 09:53 09:54 
const uint8_t pattern_62[]  = { 0, 2, 6, 9, 26, 0xFF };
//Pattern: "ES IST ZWANZIG NACH ELF " 11:20 11:21 11:22 11:23 11:24 
const uint8_t pattern_63[]  = { 0, 2, 4, 8, 27, 0xFF };
//Pattern: "ES IST ZEHN VOR ZWEI " 01:50 01:51 01:52 01:53 01:54 
const uint8_t pattern_64[]  = { 0, 2, 6, 9, 15, 0xFF };
//Pattern: "ES IST HALB ELF " 10:30 10:31 10:32 10:33 10:34 
const uint8_t pattern_65[]  = { 0, 2, 12, 27, 0xFF };
//Pattern: "ES IST VIERTEL VOR ZWÖLF " 11:45 11:46 11:47 11:48 11:49 
const uint8_t pattern_66[]  = { 0, 2, 7, 9, 28, 0xFF };
//Pattern: "ES IST FÜNF VOR FÜNF ZEHN " 14:55 14:56 14:57 14:58 14:59 
const uint8_t pattern_67[]  = { 0, 2, 9, 18, 19, 26, 0xFF };
//Pattern: "ES IST ZEHN NACH VIER ZEHN " 14:10 14:11 14:12 14:13 14:14 
const uint8_t pattern_68[]  = { 0, 2, 4, 5, 6, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR SECH ZEHN " 15:40 15:41 15:42 15:43 15:44 
const uint8_t pattern_69[]  = { 0, 2, 8, 9, 20, 23, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH ACHT ZEHN " 18:05 18:06 18:07 18:08 18:09 
const uint8_t pattern_70[]  = { 0, 2, 4, 18, 24, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH ZWÖLF " 12:05 12:06 12:07 12:08 12:09 
const uint8_t pattern_71[]  = { 0, 2, 4, 18, 28, 0xFF };
//Pattern: "ES IST VIERTEL VOR EIN UND ZWANZIG " 20:45 20:46 20:47 20:48 20:49 
const uint8_t pattern_72[]  = { 0, 2, 7, 9, 14, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB ZEHN " 09:25 09:26 09:27 09:28 09:29 
const uint8_t pattern_73[]  = { 0, 2, 9, 10, 18, 26, 0xFF };
//Pattern: "ES IST VIERTEL NACH NEUN ZEHN " 19:15 19:16 19:17 19:18 19:19 
const uint8_t pattern_74[]  = { 0, 2, 4, 7, 25, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH SIEBEN " 07:05 07:06 07:07 07:08 07:09 
const uint8_t pattern_75[]  = { 0, 2, 4, 18, 23, 0xFF };
//Pattern: "ES IST HALB SECHS " 05:30 05:31 05:32 05:33 05:34 
const uint8_t pattern_76[]  = { 0, 2, 12, 21, 0xFF };
//Pattern: "ES IST ZEHN NACH ZWEI UND ZWANZIG " 22:10 22:11 22:12 22:13 22:14 
const uint8_t pattern_77[]  = { 0, 2, 4, 6, 15, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH DREI ZEHN " 13:05 13:06 13:07 13:08 13:09 
const uint8_t pattern_78[]  = { 0, 2, 4, 16, 18, 26, 0xFF };
//Pattern: "ES IST PUNKT VIER ZEHN " 14:00 14:01 14:02 14:03 14:04 
const uint8_t pattern_79[]  = { 0, 2, 3, 5, 26, 0xFF };
//Pattern: "ES IST HALB SIEB ZEHN " 16:30 16:31 16:32 16:33 16:34 
const uint8_t pattern_80[]  = { 0, 2, 12, 22, 26, 0xFF };
//Pattern: "ES IST ZEHN VOR NEUN ZEHN " 18:50 18:51 18:52 18:53 18:54 
const uint8_t pattern_81[]  = { 0, 2, 6, 9, 25, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR ZWANZIG " 19:40 19:41 19:42 19:43 19:44 
const uint8_t pattern_82[]  = { 0, 2, 8, 9, 0xFF };
//Pattern: "ES IST FÜNF NACH SECHS " 06:05 06:06 06:07 06:08 06:09 
const uint8_t pattern_83[]  = { 0, 2, 4, 18, 21, 0xFF };
//Pattern: "ES IST ZEHN VOR ACHT " 07:50 07:51 07:52 07:53 07:54 
const uint8_t pattern_84[]  = { 0, 2, 6, 9, 24, 0xFF };
//Pattern: "ES IST ZEHN NACH SIEBEN " 07:10 07:11 07:12 07:13 07:14 
const uint8_t pattern_85[]  = { 0, 2, 4, 6, 23, 0xFF };
//Pattern: "ES IST VIERTEL VOR ELF " 10:45 10:46 10:47 10:48 10:49 
const uint8_t pattern_86[]  = { 0, 2, 7, 9, 27, 0xFF };
//Pattern: "ES IST ZEHN VOR ZWEI UND ZWANZIG " 21:50 21:51 21:52 21:53 21:54 
const uint8_t pattern_87[]  = { 0, 2, 6, 9, 15, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB SIEBEN " 06:35 06:36 06:37 06:38 06:39 
const uint8_t pattern_88[]  = { 0, 2, 4, 10, 18, 23, 0xFF };
//Pattern: "ES IST VIERTEL VOR NEUN ZEHN " 18:45 18:46 18:47 18:48 18:49 
const uint8_t pattern_89[]  = { 0, 2, 7, 9, 25, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB ACHT " 07:25 07:26 07:27 07:28 07:29 
const uint8_t pattern_90[]  = { 0, 2, 9, 10, 18, 24, 0xFF };
//Pattern: "ES IST VIERTEL VOR ACHT " 07:45 07:46 07:47 07:48 07:49 
const uint8_t pattern_91[]  = { 0, 2, 7, 9, 24, 0xFF };
//Pattern: "ES IST PUNKT ZWÖLF " 12:00 12:01 12:02 12:03 12:04 
const uint8_t pattern_92[]  = { 0, 2, 3, 28, 0xFF };
//Pattern: "ES IST ZEHN VOR DREI " 02:50 02:51 02:52 02:53 02:54 
const uint8_t pattern_93[]  = { 0, 2, 6, 9, 16, 0xFF };
//Pattern: "ES IST ZEHN VOR ELF " 10:50 10:51 10:52 10:53 10:54 
const uint8_t pattern_94[]  = { 0, 2, 6, 9, 27, 0xFF };
//Pattern: "ES IST VIERTEL NACH DREI ZEHN " 13:15 13:16 13:17 13:18 13:19 
const uint8_t pattern_95[]  = { 0, 2, 4, 7, 16, 26, 0xFF };
//Pattern: "ES IST HALB SECH ZEHN " 15:30 15:31 15:32 15:33 15:34 
const uint8_t pattern_96[]  = { 0, 2, 12, 20, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB ELF " 10:25 10:26 10:27 10:28 10:29 
const uint8_t pattern_97[]  = { 0, 2, 9, 10, 18, 27, 0xFF };
//Pattern: "ES IST VIERTEL VOR DREI " 02:45 02:46 02:47 02:48 02:49 
const uint8_t pattern_98[]  = { 0, 2, 7, 9, 16, 0xFF };
//Pattern: "ES IST FÜNF NACH FÜNF " 05:05 05:06 05:07 05:08 05:09 
const uint8_t pattern_99[]  = { 0, 2, 4, 18, 19, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB NEUN ZEHN " 18:25 18:26 18:27 18:28 18:29 
const uint8_t pattern_100[]  = { 0, 2, 9, 10, 18, 25, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR ZWEI " 01:40 01:41 01:42 01:43 01:44 
const uint8_t pattern_101[]  = { 0, 2, 8, 9, 15, 0xFF };
//Pattern: "ES IST FÜNF VOR ELF " 10:55 10:56 10:57 10:58 10:59 
const uint8_t pattern_102[]  = { 0, 2, 9, 18, 27, 0xFF };
//Pattern: "ES IST FÜNF VOR NEUN ZEHN " 18:55 18:56 18:57 18:58 18:59 
const uint8_t pattern_103[]  = { 0, 2, 9, 18, 25, 26, 0xFF };
//Pattern: "ES IST ZWANZIG NACH VIER ZEHN " 14:20 14:21 14:22 14:23 14:24 
const uint8_t pattern_104[]  = { 0, 2, 4, 5, 8, 26, 0xFF };
//Pattern: "ES IST ZEHN VOR DREI UND ZWANZIG " 22:50 22:51 22:52 22:53 22:54 
const uint8_t pattern_105[]  = { 0, 2, 6, 9, 16, 29, 30, 0xFF };
//Pattern: "ES IST ZEHN NACH SIEB ZEHN " 17:10 17:11 17:12 17:13 17:14 
const uint8_t pattern_106[]  = { 0, 2, 4, 6, 22, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB SECHS " 05:35 05:36 05:37 05:38 05:39 
const uint8_t pattern_107[]  = { 0, 2, 4, 10, 18, 21, 0xFF };
//Pattern: "ES IST ZWANZIG NACH FÜNF " 05:20 05:21 05:22 05:23 05:24 
const uint8_t pattern_108[]  = { 0, 2, 4, 8, 19, 0xFF };
//Pattern: "ES IST ZEHN NACH VIER " 04:10 04:11 04:12 04:13 04:14 
const uint8_t pattern_109[]  = { 0, 2, 4, 5, 6, 0xFF };
//Pattern: "ES IST ZWANZIG VOR NEUN ZEHN " 18:40 18:41 18:42 18:43 18:44 
const uint8_t pattern_110[]  = { 0, 2, 8, 9, 25, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB DREI UND ZWANZIG " 22:25 22:26 22:27 22:28 22:29 
const uint8_t pattern_111[]  = { 0, 2, 9, 10, 16, 18, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB FÜNF " 04:35 04:36 04:37 04:38 04:39 
const uint8_t pattern_112[]  = { 0, 2, 4, 10, 18, 19, 0xFF };
//Pattern: "ES IST FÜNF VOR NEUN " 08:55 08:56 08:57 08:58 08:59 
const uint8_t pattern_113[]  = { 0, 2, 9, 18, 25, 0xFF };
//Pattern: "ES IST VIERTEL VOR DREI UND ZWANZIG " 22:45 22:46 22:47 22:48 22:49 
const uint8_t pattern_114[]  = { 0, 2, 7, 9, 16, 29, 30, 0xFF };
//Pattern: "ES IST ZWANZIG VOR DREI ZEHN " 12:40 12:41 12:42 12:43 12:44 
const uint8_t pattern_115[]  = { 0, 2, 8, 9, 16, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR DREI " 02:55 02:56 02:57 02:58 02:59 
const uint8_t pattern_116[]  = { 0, 2, 9, 16, 18, 0xFF };
//Pattern: "ES IST PUNKT NEUN " 09:00 09:01 09:02 09:03 09:04 
const uint8_t pattern_117[]  = { 0, 2, 3, 25, 0xFF };
//Pattern: "ES IST HALB VIER UND ZWANZIG " 23:30 23:31 23:32 23:33 23:34 
const uint8_t pattern_118[]  = { 0, 2, 5, 12, 29, 30, 0xFF };
//Pattern: "ES IST ZWANZIG NACH SIEBEN " 07:20 07:21 07:22 07:23 07:24 
const uint8_t pattern_119[]  = { 0, 2, 4, 8, 23, 0xFF };
//Pattern: "ES IST VIERTEL VOR ZWANZIG " 19:45 19:46 19:47 19:48 19:49 
const uint8_t pattern_120[]  = { 0, 2, 7, 9, 29, 0xFF };
//Pattern: "ES IST HALB FÜNF " 04:30 04:31 04:32 04:33 04:34 
const uint8_t pattern_121[]  = { 0, 2, 12, 18, 0xFF };
//Pattern: "ES IST ZWANZIG VOR ZWEI UND ZWANZIG " 21:40 21:41 21:42 21:43 21:44 
const uint8_t pattern_122[]  = { 0, 2, 8, 9, 15, 29, 30, 0xFF };
//Pattern: "ES IST HALB SIEBEN " 06:30 06:31 06:32 06:33 06:34 
const uint8_t pattern_123[]  = { 0, 2, 12, 23, 0xFF };
//Pattern: "ES IST PUNKT DREI UND ZWANZIG " 23:00 23:01 23:02 23:03 23:04 
const uint8_t pattern_124[]  = { 0, 2, 3, 16, 29, 30, 0xFF };
//Pattern: "ES IST HALB EIN UND ZWANZIG " 20:30 20:31 20:32 20:33 20:34 
const uint8_t pattern_125[]  = { 0, 2, 12, 14, 29, 30, 0xFF };
//Pattern: "ES IST ZEHN NACH SECH ZEHN " 16:10 16:11 16:12 16:13 16:14 
const uint8_t pattern_126[]  = { 0, 2, 4, 6, 20, 26, 0xFF };
//Pattern: "ES IST ZWANZIG NACH EINS " 01:20 01:21 01:22 01:23 01:24 
const uint8_t pattern_127[]  = { 0, 2, 4, 8, 13, 0xFF };
//Pattern: "ES IST ZEHN VOR ZWANZIG " 19:50 19:51 19:52 19:53 19:54 
const uint8_t pattern_128[]  = { 0, 2, 6, 9, 29, 0xFF };
//Pattern: "ES IST ZEHN NACH VIER UND ZWANZIG " 00:10 00:11 00:12 00:13 00:14 
const uint8_t pattern_129[]  = { 0, 2, 4, 5, 6, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB DREI " 02:25 02:26 02:27 02:28 02:29 
const uint8_t pattern_130[]  = { 0, 2, 9, 10, 16, 18, 0xFF };
//Pattern: "ES IST PUNKT ACHT " 08:00 08:01 08:02 08:03 08:04 
const uint8_t pattern_131[]  = { 0, 2, 3, 24, 0xFF };
//Pattern: "ES IST ZWANZIG VOR ZWÖLF " 11:40 11:41 11:42 11:43 11:44 
const uint8_t pattern_132[]  = { 0, 2, 8, 9, 28, 0xFF };
//Pattern: "ES IST VIERTEL NACH SECHS " 06:15 06:16 06:17 06:18 06:19 
const uint8_t pattern_133[]  = { 0, 2, 4, 7, 21, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB ZWANZIG " 19:25 19:26 19:27 19:28 19:29 
const uint8_t pattern_134[]  = { 0, 2, 9, 10, 18, 29, 0xFF };
//Pattern: "ES IST VIERTEL NACH ZWEI " 02:15 02:16 02:17 02:18 02:19 
const uint8_t pattern_135[]  = { 0, 2, 4, 7, 15, 0xFF };
//Pattern: "ES IST HALB VIER " 03:30 03:31 03:32 03:33 03:34 
const uint8_t pattern_136[]  = { 0, 2, 5, 12, 0xFF };
//Pattern: "ES IST FÜNF VOR DREI UND ZWANZIG " 22:55 22:56 22:57 22:58 22:59 
const uint8_t pattern_137[]  = { 0, 2, 9, 16, 18, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB SECH ZEHN " 15:35 15:36 15:37 15:38 15:39 
const uint8_t pattern_138[]  = { 0, 2, 4, 10, 18, 20, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR ACHT ZEHN " 17:40 17:41 17:42 17:43 17:44 
const uint8_t pattern_139[]  = { 0, 2, 8, 9, 24, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR ZWANZIG " 19:55 19:56 19:57 19:58 19:59 
const uint8_t pattern_140[]  = { 0, 2, 9, 18, 29, 0xFF };
//Pattern: "ES IST ZWANZIG NACH EIN UND ZWANZIG " 21:20 21:21 21:22 21:23 21:24 
const uint8_t pattern_141[]  = { 0, 2, 4, 8, 14, 29, 30, 0xFF };
//Pattern: "ES IST PUNKT DREI " 03:00 03:01 03:02 03:03 03:04 
const uint8_t pattern_142[]  = { 0, 2, 3, 16, 0xFF };
//Pattern: "ES IST ZWANZIG NACH SIEB ZEHN " 17:20 17:21 17:22 17:23 17:24 
const uint8_t pattern_143[]  = { 0, 2, 4, 8, 22, 26, 0xFF };
//Pattern: "ES IST VIERTEL VOR NEUN " 08:45 08:46 08:47 08:48 08:49 
const uint8_t pattern_144[]  = { 0, 2, 7, 9, 25, 0xFF };
//Pattern: "ES IST PUNKT ZWANZIG " 20:00 20:01 20:02 20:03 20:04 
const uint8_t pattern_145[]  = { 0, 2, 3, 29, 0xFF };
//Pattern: "ES IST FÜNF VOR ACHT " 07:55 07:56 07:57 07:58 07:59 
const uint8_t pattern_146[]  = { 0, 2, 9, 18, 24, 0xFF };
//Pattern: "ES IST ZEHN NACH FÜNF " 05:10 05:11 05:12 05:13 05:14 
const uint8_t pattern_147[]  = { 0, 2, 4, 6, 19, 0xFF };
//Pattern: "ES IST FÜNF VOR FÜNF " 04:55 04:56 04:57 04:58 04:59 
const uint8_t pattern_148[]  = { 0, 2, 9, 18, 19, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB ZEHN " 09:35 09:36 09:37 09:38 09:39 
const uint8_t pattern_149[]  = { 0, 2, 4, 10, 18, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH NEUN ZEHN " 19:05 19:06 19:07 19:08 19:09 
const uint8_t pattern_150[]  = { 0, 2, 4, 18, 25, 26, 0xFF };
//Pattern: "ES IST ZWANZIG NACH VIER " 04:20 04:21 04:22 04:23 04:24 
const uint8_t pattern_151[]  = { 0, 2, 4, 5, 8, 0xFF };
//Pattern: "ES IST VIERTEL VOR SECHS " 05:45 05:46 05:47 05:48 05:49 
const uint8_t pattern_152[]  = { 0, 2, 7, 9, 21, 0xFF };
//Pattern: "ES IST FÜNF NACH ACHT " 08:05 08:06 08:07 08:08 08:09 
const uint8_t pattern_153[]  = { 0, 2, 4, 18, 24, 0xFF };
//Pattern: "ES IST VIERTEL NACH EIN UND ZWANZIG " 21:16 21:17 21:18 21:19 
const uint8_t pattern_154[]  = { 0, 2, 4, 7, 14, 29, 30, 0xFF };
//Pattern: "ES IST ZEHN VOR SECHS " 05:50 05:51 05:52 05:53 05:54 
const uint8_t pattern_155[]  = { 0, 2, 6, 9, 21, 0xFF };
//Pattern: "ES IST VIERTEL NACH ZEHN " 10:15 10:16 10:17 10:18 10:19 
const uint8_t pattern_156[]  = { 0, 2, 4, 7, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB VIER UND ZWANZIG " 23:35 23:36 23:37 23:38 23:39 
const uint8_t pattern_157[]  = { 0, 2, 4, 5, 10, 18, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH ZWEI " 02:05 02:06 02:07 02:08 02:09 
const uint8_t pattern_158[]  = { 0, 2, 4, 15, 18, 0xFF };
//Pattern: "ES IST ZEHN VOR NEUN " 08:50 08:51 08:52 08:53 08:54 
const uint8_t pattern_159[]  = { 0, 2, 6, 9, 25, 0xFF };
//Pattern: "ES IST FÜNF VOR ZWÖLF " 11:55 11:56 11:57 11:58 11:59 
const uint8_t pattern_160[]  = { 0, 2, 9, 18, 28, 0xFF };
//Pattern: "ES IST PUNKT VIER " 04:00 04:01 04:02 04:03 04:04 
const uint8_t pattern_161[]  = { 0, 2, 3, 5, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB SECHS " 05:25 05:26 05:27 05:28 05:29 
const uint8_t pattern_162[]  = { 0, 2, 9, 10, 18, 21, 0xFF };
//Pattern: "ES IST HALB ZWÖLF " 11:30 11:31 11:32 11:33 11:34 
const uint8_t pattern_163[]  = { 0, 2, 12, 28, 0xFF };
//Pattern: "ES IST ZWANZIG NACH SECH ZEHN " 16:20 16:21 16:22 16:23 16:24 
const uint8_t pattern_164[]  = { 0, 2, 4, 8, 20, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB EIN UND ZWANZIG " 20:35 20:36 20:37 20:38 20:39 
const uint8_t pattern_165[]  = { 0, 2, 4, 10, 14, 18, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF NACH DREI " 03:05 03:06 03:07 03:08 03:09 
const uint8_t pattern_166[]  = { 0, 2, 4, 16, 18, 0xFF };
//Pattern: "ES IST FÜNF NACH ELF " 11:05 11:06 11:07 11:08 11:09 
const uint8_t pattern_167[]  = { 0, 2, 4, 18, 27, 0xFF };
//Pattern: "ES IST ZEHN NACH EINS " 01:10 01:11 01:12 01:13 01:14 
const uint8_t pattern_168[]  = { 0, 2, 4, 6, 13, 0xFF };
//Pattern: "ES IST ZEHN VOR SIEBEN " 06:50 06:51 06:52 06:53 06:54 
const uint8_t pattern_169[]  = { 0, 2, 6, 9, 23, 0xFF };
//Pattern: "ES IST HALB ACHT ZEHN " 17:30 17:31 17:32 17:33 17:34 
const uint8_t pattern_170[]  = { 0, 2, 12, 24, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR VIER ZEHN " 13:40 13:41 13:42 13:43 13:44 
const uint8_t pattern_171[]  = { 0, 2, 5, 8, 9, 26, 0xFF };
//Pattern: "ES IST ZWANZIG NACH VIER UND ZWANZIG " 00:20 00:21 00:22 00:23 00:24 
const uint8_t pattern_172[]  = { 0, 2, 4, 5, 8, 29, 30, 0xFF };
//Pattern: "ES IST HALB EINS " 00:30 00:31 00:32 00:33 00:34 
const uint8_t pattern_173[]  = { 0, 2, 12, 13, 0xFF };
//Pattern: "ES IST VIERTEL NACH SECH ZEHN " 16:15 16:16 16:17 16:18 16:19 
const uint8_t pattern_174[]  = { 0, 2, 4, 7, 20, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB VIER " 03:35 03:36 03:37 03:38 03:39 
const uint8_t pattern_175[]  = { 0, 2, 4, 5, 10, 18, 0xFF };
//Pattern: "ES IST VIERTEL NACH EINS " 01:15 01:16 01:17 01:18 01:19 
const uint8_t pattern_176[]  = { 0, 2, 4, 7, 13, 0xFF };
//Pattern: "ES IST FÜNF NACH ZWEI UND ZWANZIG " 22:05 22:06 22:07 22:08 22:09 
const uint8_t pattern_177[]  = { 0, 2, 4, 15, 18, 29, 30, 0xFF };
//Pattern: "ES IST ZWANZIG NACH FÜNF ZEHN " 15:20 15:21 15:22 15:23 15:24 
const uint8_t pattern_178[]  = { 0, 2, 4, 8, 19, 26, 0xFF };
//Pattern: "ES IST ZEHN NACH EIN UND ZWANZIG " 21:10 21:11 21:12 21:13 21:14 
const uint8_t pattern_179[]  = { 0, 2, 4, 6, 14, 29, 30, 0xFF };
//Pattern: "ES IST VIERTEL NACH VIER UND ZWANZIG " 00:15 00:16 00:17 00:18 00:19 
const uint8_t pattern_180[]  = { 0, 2, 4, 5, 7, 29, 30, 0xFF };
//Pattern: "ES IST VIERTEL VOR VIER ZEHN " 13:45 13:46 13:47 13:48 13:49 
const uint8_t pattern_181[]  = { 0, 2, 5, 7, 9, 26, 0xFF };
//Pattern: "ES IST HALB DREI ZEHN " 12:30 12:31 12:32 12:33 12:34 
const uint8_t pattern_182[]  = { 0, 2, 12, 16, 26, 0xFF };
//Pattern: "ES IST ZWANZIG NACH ZEHN " 10:20 10:21 10:22 10:23 10:24 
const uint8_t pattern_183[]  = { 0, 2, 4, 8, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB NEUN " 08:25 08:26 08:27 08:28 08:29 
const uint8_t pattern_184[]  = { 0, 2, 9, 10, 18, 25, 0xFF };
//Pattern: "ES IST ZWANZIG VOR ELF " 10:40 10:41 10:42 10:43 10:44 
const uint8_t pattern_185[]  = { 0, 2, 8, 9, 27, 0xFF };
//Pattern: "ES IST HALB FÜNF ZEHN " 14:30 14:31 14:32 14:33 14:34 
const uint8_t pattern_186[]  = { 0, 2, 12, 18, 26, 0xFF };
//Pattern: "ES IST PUNKT EIN UND ZWANZIG " 21:00 21:01 21:02 21:03 21:04 
const uint8_t pattern_187[]  = { 0, 2, 3, 14, 29, 30, 0xFF };
//Pattern: "ES IST ZWANZIG VOR ACHT " 07:40 07:41 07:42 07:43 07:44 
const uint8_t pattern_188[]  = { 0, 2, 8, 9, 24, 0xFF };
//Pattern: "ES IST HALB ZEHN " 09:30 09:31 09:32 09:33 09:34 
const uint8_t pattern_189[]  = { 0, 2, 12, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR SECHS " 05:40 05:41 05:42 05:43 05:44 
const uint8_t pattern_190[]  = { 0, 2, 8, 9, 21, 0xFF };
//Pattern: "ES IST PUNKT FÜNF ZEHN " 15:00 15:01 15:02 15:03 15:04 
const uint8_t pattern_191[]  = { 0, 2, 3, 18, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB SIEBEN " 06:25 06:26 06:27 06:28 06:29 
const uint8_t pattern_192[]  = { 0, 2, 9, 10, 18, 23, 0xFF };
//Pattern: "ES IST PUNKT ZEHN " 10:00 10:01 10:02 10:03 10:04 
const uint8_t pattern_193[]  = { 0, 2, 3, 26, 0xFF };
//Pattern: "ES IST VIERTEL NACH FÜNF " 05:15 05:16 05:17 05:18 05:19 
const uint8_t pattern_194[]  = { 0, 2, 4, 7, 19, 0xFF };
//Pattern: "ES IST ZEHN NACH ZWÖLF " 12:10 12:11 12:12 12:13 12:14 
const uint8_t pattern_195[]  = { 0, 2, 4, 6, 28, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB ZWÖLF " 11:35 11:36 11:37 11:38 11:39 
const uint8_t pattern_196[]  = { 0, 2, 4, 10, 18, 28, 0xFF };
//Pattern: "ES IST VIERTEL VOR FÜNF ZEHN " 14:45 14:46 14:47 14:48 14:49 
const uint8_t pattern_197[]  = { 0, 2, 7, 9, 19, 26, 0xFF };
//Pattern: "ES IST VIERTEL NACH VIER " 04:15 04:16 04:17 04:18 04:19 
const uint8_t pattern_198[]  = { 0, 2, 4, 5, 7, 0xFF };
//Pattern: "ES IST ZWANZIG VOR NEUN " 08:40 08:41 08:42 08:43 08:44 
const uint8_t pattern_199[]  = { 0, 2, 8, 9, 25, 0xFF };
//Pattern: "ES IST VIERTEL NACH ACHT ZEHN " 18:15 18:16 18:17 18:18 18:19 
const uint8_t pattern_200[]  = { 0, 2, 4, 7, 24, 26, 0xFF };
//Pattern: "ES IST ZEHN NACH DREI ZEHN " 13:10 13:11 13:12 13:13 13:14 
const uint8_t pattern_201[]  = { 0, 2, 4, 6, 16, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR DREI " 02:40 02:41 02:42 02:43 02:44 
const uint8_t pattern_202[]  = { 0, 2, 8, 9, 16, 0xFF };
//Pattern: "ES IST VIERTEL NACH ZWÖLF " 12:15 12:16 12:17 12:18 12:19 
const uint8_t pattern_203[]  = { 0, 2, 4, 7, 28, 0xFF };
//Pattern: "ES IST ZEHN NACH ACHT ZEHN " 18:10 18:11 18:12 18:13 18:14 
const uint8_t pattern_204[]  = { 0, 2, 4, 6, 24, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR EIN UND ZWANZIG " 20:55 20:56 20:57 20:58 20:59 
const uint8_t pattern_205[]  = { 0, 2, 9, 14, 18, 29, 30, 0xFF };
//Pattern: "ES IST PUNKT SIEBEN " 07:00 07:01 07:02 07:03 07:04 
const uint8_t pattern_206[]  = { 0, 2, 3, 23, 0xFF };
//Pattern: "ES IST FÜNF VOR ZEHN " 09:55 09:56 09:57 09:58 09:59 
const uint8_t pattern_207[]  = { 0, 2, 9, 18, 26, 0xFF };
//Pattern: "ES IST ZEHN VOR FÜNF ZEHN " 14:50 14:51 14:52 14:53 14:54 
const uint8_t pattern_208[]  = { 0, 2, 6, 9, 19, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR EINS " 00:55 00:56 00:57 00:58 00:59 
const uint8_t pattern_209[]  = { 0, 2, 9, 13, 18, 0xFF };
//Pattern: "ES IST FÜNF VOR ACHT ZEHN " 17:55 17:56 17:57 17:58 17:59 
const uint8_t pattern_210[]  = { 0, 2, 9, 18, 24, 26, 0xFF };
//Pattern: "ES IST ZWANZIG NACH NEUN " 09:20 09:21 09:22 09:23 09:24 
const uint8_t pattern_211[]  = { 0, 2, 4, 8, 25, 0xFF };
//Pattern: "ES IST ZEHN VOR VIER ZEHN " 13:50 13:51 13:52 13:53 13:54 
const uint8_t pattern_212[]  = { 0, 2, 5, 6, 9, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB FÜNF ZEHN " 14:25 14:26 14:27 14:28 14:29 
const uint8_t pattern_213[]  = { 0, 2, 9, 10, 18, 19, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH SIEB ZEHN " 17:05 17:06 17:07 17:08 17:09 
const uint8_t pattern_214[]  = { 0, 2, 4, 18, 22, 26, 0xFF };
//Pattern: "ES IST PUNKT EINS " 01:00 01:01 01:02 01:03 01:04 
const uint8_t pattern_215[]  = { 0, 2, 3, 13, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB EINS " 00:35 00:36 00:37 00:38 00:39 
const uint8_t pattern_216[]  = { 0, 2, 4, 10, 13, 18, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB ACHT ZEHN " 17:35 17:36 17:37 17:38 17:39 
const uint8_t pattern_217[]  = { 0, 2, 4, 10, 18, 24, 26, 0xFF };
//Pattern: "ES IST VIERTEL VOR SIEBEN " 06:45 06:46 06:47 06:48 06:49 
const uint8_t pattern_218[]  = { 0, 2, 7, 9, 23, 0xFF };
//Pattern: "ES IST ZEHN NACH FÜNF ZEHN " 15:10 15:11 15:12 15:13 15:14 
const uint8_t pattern_219[]  = { 0, 2, 4, 6, 19, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB FÜNF ZEHN " 14:35 14:36 14:37 14:38 14:39 
const uint8_t pattern_220[]  = { 0, 2, 4, 10, 18, 19, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB VIER ZEHN " 13:25 13:26 13:27 13:28 13:29 
const uint8_t pattern_221[]  = { 0, 2, 5, 9, 10, 18, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB DREI ZEHN " 12:35 12:36 12:37 12:38 12:39 
const uint8_t pattern_222[]  = { 0, 2, 4, 10, 16, 18, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR DREI UND ZWANZIG " 22:40 22:41 22:42 22:43 22:44 
const uint8_t pattern_223[]  = { 0, 2, 8, 9, 16, 29, 30, 0xFF };
//Pattern: "ES IST ZEHN NACH ZEHN " 10:10 10:11 10:12 10:13 10:14 
const uint8_t pattern_224[]  = { 0, 2, 4, 6, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB ZWEI UND ZWANZIG " 21:25 21:26 21:27 21:28 21:29 
const uint8_t pattern_225[]  = { 0, 2, 9, 10, 15, 18, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF VOR DREI ZEHN " 12:55 12:56 12:57 12:58 12:59 
const uint8_t pattern_226[]  = { 0, 2, 9, 16, 18, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR SIEB ZEHN " 16:55 16:56 16:57 16:58 16:59 
const uint8_t pattern_227[]  = { 0, 2, 9, 18, 22, 26, 0xFF };
//Pattern: "ES IST VIERTEL VOR SECH ZEHN " 15:45 15:46 15:47 15:48 15:49 
const uint8_t pattern_228[]  = { 0, 2, 7, 9, 20, 26, 0xFF };
//Pattern: "ES IST ZWANZIG VOR FÜNF ZEHN " 14:40 14:41 14:42 14:43 14:44 
const uint8_t pattern_229[]  = { 0, 2, 8, 9, 19, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB VIER " 03:25 03:26 03:27 03:28 03:29 
const uint8_t pattern_230[]  = { 0, 2, 5, 9, 10, 18, 0xFF };
//Pattern: "ES IST ZWANZIG NACH ACHT ZEHN " 18:20 18:21 18:22 18:23 18:24 
const uint8_t pattern_231[]  = { 0, 2, 4, 8, 24, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB NEUN " 08:35 08:36 08:37 08:38 08:39 
const uint8_t pattern_232[]  = { 0, 2, 4, 10, 18, 25, 0xFF };
//Pattern: "ES IST ZWANZIG NACH ZWÖLF " 12:20 12:21 12:22 12:23 12:24 
const uint8_t pattern_233[]  = { 0, 2, 4, 8, 28, 0xFF };
//Pattern: "ES IST FÜNF NACH NEUN " 09:05 09:06 09:07 09:08 09:09 
const uint8_t pattern_234[]  = { 0, 2, 4, 18, 25, 0xFF };
//Pattern: "ES IST ZWANZIG NACH DREI ZEHN " 13:20 13:21 13:22 13:23 13:24 
const uint8_t pattern_235[]  = { 0, 2, 4, 8, 16, 26, 0xFF };
//Pattern: "ES IST VIERTEL VOR VIER " 03:45 03:46 03:47 03:48 03:49 
const uint8_t pattern_236[]  = { 0, 2, 5, 7, 9, 0xFF };
//Pattern: "ES IST ZWANZIG NACH SECHS " 06:20 06:21 06:22 06:23 06:24 
const uint8_t pattern_237[]  = { 0, 2, 4, 8, 21, 0xFF };
//Pattern: "ES IST ZWANZIG VOR SIEB ZEHN " 16:40 16:41 16:42 16:43 16:44 
const uint8_t pattern_238[]  = { 0, 2, 8, 9, 22, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB SECH ZEHN " 15:25 15:26 15:27 15:28 15:29 
const uint8_t pattern_239[]  = { 0, 2, 9, 10, 18, 20, 26, 0xFF };
//Pattern: "ES IST ZEHN VOR VIER UND ZWANZIG " 23:50 23:51 23:52 23:53 23:54 
const uint8_t pattern_240[]  = { 0, 2, 5, 6, 9, 29, 30, 0xFF };
//Pattern: "ES IST VIERTEL VOR ZWEI " 01:45 01:46 01:47 01:48 01:49 
const uint8_t pattern_241[]  = { 0, 2, 7, 9, 15, 0xFF };
//Pattern: "ES IST VIERTEL NACH SIEBEN " 07:15 07:16 07:17 07:18 07:19 
const uint8_t pattern_242[]  = { 0, 2, 4, 7, 23, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB ZWEI " 01:25 01:26 01:27 01:28 01:29 
const uint8_t pattern_243[]  = { 0, 2, 9, 10, 15, 18, 0xFF };
//Pattern: "ES IST FÜNF NACH VIER ZEHN " 14:05 14:06 14:07 14:08 14:09 
const uint8_t pattern_244[]  = { 0, 2, 4, 5, 18, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR ZWEI " 01:55 01:56 01:57 01:58 01:59 
const uint8_t pattern_245[]  = { 0, 2, 9, 15, 18, 0xFF };
//Pattern: "ES IST PUNKT SECHS " 06:00 06:01 06:02 06:03 06:04 
const uint8_t pattern_246[]  = { 0, 2, 3, 21, 0xFF };
//Pattern: "ES IST FÜNF VOR VIER " 03:55 03:56 03:57 03:58 03:59 
const uint8_t pattern_247[]  = { 0, 2, 5, 9, 18, 0xFF };
//Pattern: "ES IST ZEHN VOR EIN UND ZWANZIG " 20:50 20:51 20:52 20:53 20:54 
const uint8_t pattern_248[]  = { 0, 2, 6, 9, 14, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB EIN UND ZWANZIG " 20:25 20:26 20:27 20:28 20:29 
const uint8_t pattern_249[]  = { 0, 2, 9, 10, 14, 18, 29, 30, 0xFF };
//Pattern: "ES IST ZEHN VOR EINS " 00:50 00:51 00:52 00:53 00:54 
const uint8_t pattern_250[]  = { 0, 2, 6, 9, 13, 0xFF };
//Pattern: "ES IST HALB ZWANZIG " 19:30 19:31 19:32 19:33 19:34 
const uint8_t pattern_251[]  = { 0, 2, 12, 29, 0xFF };
//Pattern: "ES IST ZEHN VOR SECH ZEHN " 15:50 15:51 15:52 15:53 15:54 
const uint8_t pattern_252[]  = { 0, 2, 6, 9, 20, 26, 0xFF };
//Pattern: "ES IST VIERTEL VOR VIER UND ZWANZIG " 23:45 23:46 23:47 23:48 23:49 
const uint8_t pattern_253[]  = { 0, 2, 5, 7, 9, 29, 30, 0xFF };
//Pattern: "ES IST ZWANZIG VOR VIER " 03:40 03:41 03:42 03:43 03:44 
const uint8_t pattern_254[]  = { 0, 2, 5, 8, 9, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB SIEB ZEHN " 16:25 16:26 16:27 16:28 16:29 
const uint8_t pattern_255[]  = { 0, 2, 9, 10, 18, 22, 26, 0xFF };
//Pattern: "ES IST VIERTEL NACH FÜNF ZEHN " 15:15 15:16 15:17 15:18 15:19 
const uint8_t pattern_256[]  = { 0, 2, 4, 7, 19, 26, 0xFF };
//Pattern: "ES IST VIERTEL NACH ZWANZIG " 20:15 20:16 20:17 20:18 20:19 
const uint8_t pattern_257[]  = { 0, 2, 4, 7, 29, 0xFF };
//Pattern: "ES IST ZEHN NACH NEUN " 09:10 09:11 09:12 09:13 09:14 
const uint8_t pattern_258[]  = { 0, 2, 4, 6, 25, 0xFF };
//Pattern: "ES IST HALB ACHT " 07:30 07:31 07:32 07:33 07:34 
const uint8_t pattern_259[]  = { 0, 2, 12, 24, 0xFF };
//Pattern: "ES IST VIERTEL VOR ZWEI UND ZWANZIG " 21:45 21:46 21:47 21:48 21:49 
const uint8_t pattern_260[]  = { 0, 2, 7, 9, 15, 29, 30, 0xFF };
//Pattern: "ES IST VIERTEL VOR ZEHN " 09:45 09:46 09:47 09:48 09:49 
const uint8_t pattern_261[]  = { 0, 2, 7, 9, 26, 0xFF };
//Pattern: "ES IST VIERTEL NACH VIER ZEHN " 14:15 14:16 14:17 14:18 14:19 
const uint8_t pattern_262[]  = { 0, 2, 4, 5, 7, 26, 0xFF };
//Pattern: "ES IST HALB DREI " 02:30 02:31 02:32 02:33 02:34 
const uint8_t pattern_263[]  = { 0, 2, 12, 16, 0xFF };
//Pattern: "ES IST FÜNF VOR VIER UND ZWANZIG " 23:55 23:56 23:57 23:58 23:59 
const uint8_t pattern_264[]  = { 0, 2, 5, 9, 18, 29, 30, 0xFF };
//Pattern: "ES IST ZEHN VOR FÜNF " 04:50 04:51 04:52 04:53 04:54 
const uint8_t pattern_265[]  = { 0, 2, 6, 9, 19, 0xFF };
//Pattern: "ES IST ZEHN NACH ACHT " 08:10 08:11 08:12 08:13 08:14 
const uint8_t pattern_266[]  = { 0, 2, 4, 6, 24, 0xFF };
//Pattern: "ES IST ZEHN VOR SIEB ZEHN " 16:50 16:51 16:52 16:53 16:54 
const uint8_t pattern_267[]  = { 0, 2, 6, 9, 22, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH ZWANZIG " 20:05 20:06 20:07 20:08 20:09 
const uint8_t pattern_268[]  = { 0, 2, 4, 18, 29, 0xFF };
//Pattern: "ES IST PUNKT FÜNF " 05:00 05:01 05:02 05:03 05:04 
const uint8_t pattern_269[]  = { 0, 2, 3, 18, 0xFF };
//Pattern: "ES IST ZEHN NACH DREI " 03:10 03:11 03:12 03:13 03:14 
const uint8_t pattern_270[]  = { 0, 2, 4, 6, 16, 0xFF };
//Pattern: "ES IST ZWANZIG VOR SIEBEN " 06:40 06:41 06:42 06:43 06:44 
const uint8_t pattern_271[]  = { 0, 2, 8, 9, 23, 0xFF };
//Pattern: "ES IST FÜNF VOR SECH ZEHN " 15:55 15:56 15:57 15:58 15:59 
const uint8_t pattern_272[]  = { 0, 2, 9, 18, 20, 26, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB VIER UND ZWANZIG " 23:25 23:26 23:27 23:28 23:29 
const uint8_t pattern_273[]  = { 0, 2, 5, 9, 10, 18, 29, 30, 0xFF };
//Pattern: "ES IST ZEHN NACH ZWANZIG " 20:10 20:11 20:12 20:13 20:14 
const uint8_t pattern_274[]  = { 0, 2, 4, 6, 29, 0xFF };
//Pattern: "ES IST ZWANZIG VOR EIN UND ZWANZIG " 20:40 20:41 20:42 20:43 20:44 
const uint8_t pattern_275[]  = { 0, 2, 8, 9, 14, 29, 30, 0xFF };
//Pattern: "ES IST ZWANZIG NACH NEUN ZEHN " 19:20 19:21 19:22 19:23 19:24 
const uint8_t pattern_276[]  = { 0, 2, 4, 8, 25, 26, 0xFF };
//Pattern: "ES IST PUNKT SECH ZEHN " 16:00 16:01 16:02 16:03 16:04 
const uint8_t pattern_277[]  = { 0, 2, 3, 20, 26, 0xFF };
//Pattern: "ES IST VIERTEL NACH NEUN " 09:15 09:16 09:17 09:18 09:19 
const uint8_t pattern_278[]  = { 0, 2, 4, 7, 25, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB ZWANZIG " 19:35 19:36 19:37 19:38 19:39 
const uint8_t pattern_279[]  = { 0, 2, 4, 10, 18, 29, 0xFF };
//Pattern: "ES IST PUNKT VIER UND ZWANZIG " 00:00 00:01 00:02 00:03 00:04 
const uint8_t pattern_280[]  = { 0, 2, 3, 5, 29, 30, 0xFF };
//Pattern: "ES IST ZWANZIG VOR EINS " 00:40 00:41 00:42 00:43 00:44 
const uint8_t pattern_281[]  = { 0, 2, 8, 9, 13, 0xFF };
//Pattern: "ES IST HALB NEUN ZEHN " 18:30 18:31 18:32 18:33 18:34 
const uint8_t pattern_282[]  = { 0, 2, 12, 25, 26, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB DREI " 02:35 02:36 02:37 02:38 02:39 
const uint8_t pattern_283[]  = { 0, 2, 4, 10, 16, 18, 0xFF };
//Pattern: "ES IST ZEHN VOR VIER " 03:50 03:51 03:52 03:53 03:54 
const uint8_t pattern_284[]  = { 0, 2, 5, 6, 9, 0xFF };
//Pattern: "ES IST FÜNF NACH HALB ACHT " 07:35 07:36 07:37 07:38 07:39 
const uint8_t pattern_285[]  = { 0, 2, 4, 10, 18, 24, 0xFF };
//Pattern: "ES IST VIERTEL NACH ELF " 11:15 11:16 11:17 11:18 11:19 
const uint8_t pattern_286[]  = { 0, 2, 4, 7, 27, 0xFF };
//Pattern: "ES IST FÜNF NACH DREI UND ZWANZIG " 23:05 23:06 23:07 23:08 23:09 
const uint8_t pattern_287[]  = { 0, 2, 4, 16, 18, 29, 30, 0xFF };
//Pattern: "ES IST FÜNF VOR HALB FÜNF " 04:25 04:26 04:27 04:28 04:29 
const uint8_t pattern_288[]  = { 0, 2, 9, 10, 18, 19, 0xFF };
//Main Time Table: 
const uint8_t* const time_table[24][60]  = {
   {pattern_280, pattern_280, pattern_280, pattern_280, pattern_280, pattern_12, pattern_12, pattern_12, pattern_12, pattern_12, pattern_129, pattern_129, pattern_129, pattern_129, pattern_129, pattern_180, pattern_180, pattern_180, pattern_180, pattern_180, pattern_172, pattern_172, pattern_172, pattern_172, pattern_172, pattern_54, pattern_54, pattern_54, pattern_54, pattern_54, pattern_173, pattern_173, pattern_173, pattern_173, pattern_173, pattern_216, pattern_216, pattern_216, pattern_216, pattern_216, pattern_281, pattern_281, pattern_281, pattern_281, pattern_281, pattern_53, pattern_53, pattern_53, pattern_53, pattern_53, pattern_250, pattern_250, pattern_250, pattern_250, pattern_250, pattern_209, pattern_209, pattern_209, pattern_209, pattern_209}, 
   {pattern_215, pattern_215, pattern_215, pattern_215, pattern_215, pattern_46, pattern_46, pattern_46, pattern_46, pattern_46, pattern_168, pattern_168, pattern_168, pattern_168, pattern_168, pattern_176, pattern_176, pattern_176, pattern_176, pattern_176, pattern_127, pattern_127, pattern_127, pattern_127, pattern_127, pattern_243, pattern_243, pattern_243, pattern_243, pattern_243, pattern_60, pattern_60, pattern_60, pattern_60, pattern_60, pattern_21, pattern_21, pattern_21, pattern_21, pattern_21, pattern_101, pattern_101, pattern_101, pattern_101, pattern_101, pattern_241, pattern_241, pattern_241, pattern_241, pattern_241, pattern_64, pattern_64, pattern_64, pattern_64, pattern_64, pattern_245, pattern_245, pattern_245, pattern_245, pattern_245}, 
   {pattern_26, pattern_26, pattern_26, pattern_26, pattern_26, pattern_158, pattern_158, pattern_158, pattern_158, pattern_158, pattern_55, pattern_55, pattern_55, pattern_55, pattern_55, pattern_135, pattern_135, pattern_135, pattern_135, pattern_135, pattern_18, pattern_18, pattern_18, pattern_18, pattern_18, pattern_130, pattern_130, pattern_130, pattern_130, pattern_130, pattern_263, pattern_263, pattern_263, pattern_263, pattern_263, pattern_283, pattern_283, pattern_283, pattern_283, pattern_283, pattern_202, pattern_202, pattern_202, pattern_202, pattern_202, pattern_98, pattern_98, pattern_98, pattern_98, pattern_98, pattern_93, pattern_93, pattern_93, pattern_93, pattern_93, pattern_116, pattern_116, pattern_116, pattern_116, pattern_116}, 
   {pattern_142, pattern_142, pattern_142, pattern_142, pattern_142, pattern_166, pattern_166, pattern_166, pattern_166, pattern_166, pattern_270, pattern_270, pattern_270, pattern_270, pattern_270, pattern_30, pattern_30, pattern_30, pattern_30, pattern_30, pattern_19, pattern_19, pattern_19, pattern_19, pattern_19, pattern_230, pattern_230, pattern_230, pattern_230, pattern_230, pattern_136, pattern_136, pattern_136, pattern_136, pattern_136, pattern_175, pattern_175, pattern_175, pattern_175, pattern_175, pattern_254, pattern_254, pattern_254, pattern_254, pattern_254, pattern_236, pattern_236, pattern_236, pattern_236, pattern_236, pattern_284, pattern_284, pattern_284, pattern_284, pattern_284, pattern_247, pattern_247, pattern_247, pattern_247, pattern_247}, 
   {pattern_161, pattern_161, pattern_161, pattern_161, pattern_161, pattern_31, pattern_31, pattern_31, pattern_31, pattern_31, pattern_109, pattern_109, pattern_109, pattern_109, pattern_109, pattern_198, pattern_198, pattern_198, pattern_198, pattern_198, pattern_151, pattern_151, pattern_151, pattern_151, pattern_151, pattern_288, pattern_288, pattern_288, pattern_288, pattern_288, pattern_121, pattern_121, pattern_121, pattern_121, pattern_121, pattern_112, pattern_112, pattern_112, pattern_112, pattern_112, pattern_9, pattern_9, pattern_9, pattern_9, pattern_9, pattern_38, pattern_38, pattern_38, pattern_38, pattern_38, pattern_265, pattern_265, pattern_265, pattern_265, pattern_265, pattern_148, pattern_148, pattern_148, pattern_148, pattern_148}, 
   {pattern_269, pattern_269, pattern_269, pattern_269, pattern_269, pattern_99, pattern_99, pattern_99, pattern_99, pattern_99, pattern_147, pattern_147, pattern_147, pattern_147, pattern_147, pattern_194, pattern_194, pattern_194, pattern_194, pattern_194, pattern_108, pattern_108, pattern_108, pattern_108, pattern_108, pattern_162, pattern_162, pattern_162, pattern_162, pattern_162, pattern_76, pattern_76, pattern_76, pattern_76, pattern_76, pattern_107, pattern_107, pattern_107, pattern_107, pattern_107, pattern_190, pattern_190, pattern_190, pattern_190, pattern_190, pattern_152, pattern_152, pattern_152, pattern_152, pattern_152, pattern_155, pattern_155, pattern_155, pattern_155, pattern_155, pattern_33, pattern_33, pattern_33, pattern_33, pattern_33}, 
   {pattern_246, pattern_246, pattern_246, pattern_246, pattern_246, pattern_83, pattern_83, pattern_83, pattern_83, pattern_83, pattern_48, pattern_48, pattern_48, pattern_48, pattern_48, pattern_133, pattern_133, pattern_133, pattern_133, pattern_133, pattern_237, pattern_237, pattern_237, pattern_237, pattern_237, pattern_192, pattern_192, pattern_192, pattern_192, pattern_192, pattern_123, pattern_123, pattern_123, pattern_123, pattern_123, pattern_88, pattern_88, pattern_88, pattern_88, pattern_88, pattern_271, pattern_271, pattern_271, pattern_271, pattern_271, pattern_218, pattern_218, pattern_218, pattern_218, pattern_218, pattern_169, pattern_169, pattern_169, pattern_169, pattern_169, pattern_49, pattern_49, pattern_49, pattern_49, pattern_49}, 
   {pattern_206, pattern_206, pattern_206, pattern_206, pattern_206, pattern_75, pattern_75, pattern_75, pattern_75, pattern_75, pattern_85, pattern_85, pattern_85, pattern_85, pattern_85, pattern_242, pattern_242, pattern_242, pattern_242, pattern_242, pattern_119, pattern_119, pattern_119, pattern_119, pattern_119, pattern_90, pattern_90, pattern_90, pattern_90, pattern_90, pattern_259, pattern_259, pattern_259, pattern_259, pattern_259, pattern_285, pattern_285, pattern_285, pattern_285, pattern_285, pattern_188, pattern_188, pattern_188, pattern_188, pattern_188, pattern_91, pattern_91, pattern_91, pattern_91, pattern_91, pattern_84, pattern_84, pattern_84, pattern_84, pattern_84, pattern_146, pattern_146, pattern_146, pattern_146, pattern_146}, 
   {pattern_131, pattern_131, pattern_131, pattern_131, pattern_131, pattern_153, pattern_153, pattern_153, pattern_153, pattern_153, pattern_266, pattern_266, pattern_266, pattern_266, pattern_266, pattern_20, pattern_20, pattern_20, pattern_20, pattern_20, pattern_10, pattern_10, pattern_10, pattern_10, pattern_10, pattern_184, pattern_184, pattern_184, pattern_184, pattern_184, pattern_43, pattern_43, pattern_43, pattern_43, pattern_43, pattern_232, pattern_232, pattern_232, pattern_232, pattern_232, pattern_199, pattern_199, pattern_199, pattern_199, pattern_199, pattern_144, pattern_144, pattern_144, pattern_144, pattern_144, pattern_159, pattern_159, pattern_159, pattern_159, pattern_159, pattern_113, pattern_113, pattern_113, pattern_113, pattern_113}, 
   {pattern_117, pattern_117, pattern_117, pattern_117, pattern_117, pattern_234, pattern_234, pattern_234, pattern_234, pattern_234, pattern_258, pattern_258, pattern_258, pattern_258, pattern_258, pattern_278, pattern_278, pattern_278, pattern_278, pattern_278, pattern_211, pattern_211, pattern_211, pattern_211, pattern_211, pattern_73, pattern_73, pattern_73, pattern_73, pattern_73, pattern_189, pattern_189, pattern_189, pattern_189, pattern_189, pattern_149, pattern_149, pattern_149, pattern_149, pattern_149, pattern_27, pattern_27, pattern_27, pattern_27, pattern_27, pattern_261, pattern_261, pattern_261, pattern_261, pattern_261, pattern_62, pattern_62, pattern_62, pattern_62, pattern_62, pattern_207, pattern_207, pattern_207, pattern_207, pattern_207}, 
   {pattern_193, pattern_193, pattern_193, pattern_193, pattern_193, pattern_28, pattern_28, pattern_28, pattern_28, pattern_28, pattern_224, pattern_224, pattern_224, pattern_224, pattern_224, pattern_156, pattern_156, pattern_156, pattern_156, pattern_156, pattern_183, pattern_183, pattern_183, pattern_183, pattern_183, pattern_97, pattern_97, pattern_97, pattern_97, pattern_97, pattern_65, pattern_65, pattern_65, pattern_65, pattern_65, pattern_29, pattern_29, pattern_29, pattern_29, pattern_29, pattern_185, pattern_185, pattern_185, pattern_185, pattern_185, pattern_86, pattern_86, pattern_86, pattern_86, pattern_86, pattern_94, pattern_94, pattern_94, pattern_94, pattern_94, pattern_102, pattern_102, pattern_102, pattern_102, pattern_102}, 
   {pattern_34, pattern_34, pattern_34, pattern_34, pattern_34, pattern_167, pattern_167, pattern_167, pattern_167, pattern_167, pattern_23, pattern_23, pattern_23, pattern_23, pattern_23, pattern_286, pattern_286, pattern_286, pattern_286, pattern_286, pattern_63, pattern_63, pattern_63, pattern_63, pattern_63, pattern_6, pattern_6, pattern_6, pattern_6, pattern_6, pattern_163, pattern_163, pattern_163, pattern_163, pattern_163, pattern_196, pattern_196, pattern_196, pattern_196, pattern_196, pattern_132, pattern_132, pattern_132, pattern_132, pattern_132, pattern_66, pattern_66, pattern_66, pattern_66, pattern_66, pattern_35, pattern_35, pattern_35, pattern_35, pattern_35, pattern_160, pattern_160, pattern_160, pattern_160, pattern_160}, 
   {pattern_92, pattern_92, pattern_92, pattern_92, pattern_92, pattern_71, pattern_71, pattern_71, pattern_71, pattern_71, pattern_195, pattern_195, pattern_195, pattern_195, pattern_195, pattern_203, pattern_203, pattern_203, pattern_203, pattern_203, pattern_233, pattern_233, pattern_233, pattern_233, pattern_233, pattern_8, pattern_8, pattern_8, pattern_8, pattern_8, pattern_182, pattern_182, pattern_182, pattern_182, pattern_182, pattern_222, pattern_222, pattern_222, pattern_222, pattern_222, pattern_115, pattern_115, pattern_115, pattern_115, pattern_115, pattern_16, pattern_16, pattern_16, pattern_16, pattern_16, pattern_4, pattern_4, pattern_4, pattern_4, pattern_4, pattern_226, pattern_226, pattern_226, pattern_226, pattern_226}, 
   {pattern_56, pattern_56, pattern_56, pattern_56, pattern_56, pattern_78, pattern_78, pattern_78, pattern_78, pattern_78, pattern_201, pattern_201, pattern_201, pattern_201, pattern_201, pattern_95, pattern_95, pattern_95, pattern_95, pattern_95, pattern_235, pattern_235, pattern_235, pattern_235, pattern_235, pattern_221, pattern_221, pattern_221, pattern_221, pattern_221, pattern_14, pattern_14, pattern_14, pattern_14, pattern_14, pattern_50, pattern_50, pattern_50, pattern_50, pattern_50, pattern_171, pattern_171, pattern_171, pattern_171, pattern_171, pattern_181, pattern_181, pattern_181, pattern_181, pattern_181, pattern_212, pattern_212, pattern_212, pattern_212, pattern_212, pattern_17, pattern_17, pattern_17, pattern_17, pattern_17}, 
   {pattern_79, pattern_79, pattern_79, pattern_79, pattern_79, pattern_244, pattern_244, pattern_244, pattern_244, pattern_244, pattern_68, pattern_68, pattern_68, pattern_68, pattern_68, pattern_262, pattern_262, pattern_262, pattern_262, pattern_262, pattern_104, pattern_104, pattern_104, pattern_104, pattern_104, pattern_213, pattern_213, pattern_213, pattern_213, pattern_213, pattern_186, pattern_186, pattern_186, pattern_186, pattern_186, pattern_220, pattern_220, pattern_220, pattern_220, pattern_220, pattern_229, pattern_229, pattern_229, pattern_229, pattern_229, pattern_197, pattern_197, pattern_197, pattern_197, pattern_197, pattern_208, pattern_208, pattern_208, pattern_208, pattern_208, pattern_67, pattern_67, pattern_67, pattern_67, pattern_67}, 
   {pattern_191, pattern_191, pattern_191, pattern_191, pattern_191, pattern_51, pattern_51, pattern_51, pattern_51, pattern_51, pattern_219, pattern_219, pattern_219, pattern_219, pattern_219, pattern_256, pattern_256, pattern_256, pattern_256, pattern_256, pattern_178, pattern_178, pattern_178, pattern_178, pattern_178, pattern_239, pattern_239, pattern_239, pattern_239, pattern_239, pattern_96, pattern_96, pattern_96, pattern_96, pattern_96, pattern_138, pattern_138, pattern_138, pattern_138, pattern_138, pattern_69, pattern_69, pattern_69, pattern_69, pattern_69, pattern_228, pattern_228, pattern_228, pattern_228, pattern_228, pattern_252, pattern_252, pattern_252, pattern_252, pattern_252, pattern_272, pattern_272, pattern_272, pattern_272, pattern_272}, 
   {pattern_277, pattern_277, pattern_277, pattern_277, pattern_277, pattern_3, pattern_3, pattern_3, pattern_3, pattern_3, pattern_126, pattern_126, pattern_126, pattern_126, pattern_126, pattern_174, pattern_174, pattern_174, pattern_174, pattern_174, pattern_164, pattern_164, pattern_164, pattern_164, pattern_164, pattern_255, pattern_255, pattern_255, pattern_255, pattern_255, pattern_80, pattern_80, pattern_80, pattern_80, pattern_80, pattern_47, pattern_47, pattern_47, pattern_47, pattern_47, pattern_238, pattern_238, pattern_238, pattern_238, pattern_238, pattern_57, pattern_57, pattern_57, pattern_57, pattern_57, pattern_267, pattern_267, pattern_267, pattern_267, pattern_267, pattern_227, pattern_227, pattern_227, pattern_227, pattern_227}, 
   {pattern_7, pattern_7, pattern_7, pattern_7, pattern_7, pattern_214, pattern_214, pattern_214, pattern_214, pattern_214, pattern_106, pattern_106, pattern_106, pattern_106, pattern_106, pattern_44, pattern_44, pattern_44, pattern_44, pattern_44, pattern_143, pattern_143, pattern_143, pattern_143, pattern_143, pattern_15, pattern_15, pattern_15, pattern_15, pattern_15, pattern_170, pattern_170, pattern_170, pattern_170, pattern_170, pattern_217, pattern_217, pattern_217, pattern_217, pattern_217, pattern_139, pattern_139, pattern_139, pattern_139, pattern_139, pattern_2, pattern_2, pattern_2, pattern_2, pattern_2, pattern_32, pattern_32, pattern_32, pattern_32, pattern_32, pattern_210, pattern_210, pattern_210, pattern_210, pattern_210}, 
   {pattern_61, pattern_61, pattern_61, pattern_61, pattern_61, pattern_70, pattern_70, pattern_70, pattern_70, pattern_70, pattern_204, pattern_204, pattern_204, pattern_204, pattern_204, pattern_200, pattern_200, pattern_200, pattern_200, pattern_200, pattern_231, pattern_231, pattern_231, pattern_231, pattern_231, pattern_100, pattern_100, pattern_100, pattern_100, pattern_100, pattern_282, pattern_282, pattern_282, pattern_282, pattern_282, pattern_36, pattern_36, pattern_36, pattern_36, pattern_36, pattern_110, pattern_110, pattern_110, pattern_110, pattern_110, pattern_89, pattern_89, pattern_89, pattern_89, pattern_89, pattern_81, pattern_81, pattern_81, pattern_81, pattern_81, pattern_103, pattern_103, pattern_103, pattern_103, pattern_103}, 
   {pattern_37, pattern_37, pattern_37, pattern_37, pattern_37, pattern_150, pattern_150, pattern_150, pattern_150, pattern_150, pattern_24, pattern_24, pattern_24, pattern_24, pattern_24, pattern_74, pattern_74, pattern_74, pattern_74, pattern_74, pattern_276, pattern_276, pattern_276, pattern_276, pattern_276, pattern_134, pattern_134, pattern_134, pattern_134, pattern_134, pattern_251, pattern_251, pattern_251, pattern_251, pattern_251, pattern_279, pattern_279, pattern_279, pattern_279, pattern_279, pattern_82, pattern_82, pattern_82, pattern_82, pattern_82, pattern_120, pattern_120, pattern_120, pattern_120, pattern_120, pattern_128, pattern_128, pattern_128, pattern_128, pattern_128, pattern_140, pattern_140, pattern_140, pattern_140, pattern_140}, 
   {pattern_145, pattern_145, pattern_145, pattern_145, pattern_145, pattern_268, pattern_268, pattern_268, pattern_268, pattern_268, pattern_274, pattern_274, pattern_274, pattern_274, pattern_274, pattern_257, pattern_257, pattern_257, pattern_257, pattern_257, pattern_22, pattern_22, pattern_22, pattern_22, pattern_22, pattern_249, pattern_249, pattern_249, pattern_249, pattern_249, pattern_125, pattern_125, pattern_125, pattern_125, pattern_125, pattern_165, pattern_165, pattern_165, pattern_165, pattern_165, pattern_275, pattern_275, pattern_275, pattern_275, pattern_275, pattern_72, pattern_72, pattern_72, pattern_72, pattern_72, pattern_248, pattern_248, pattern_248, pattern_248, pattern_248, pattern_205, pattern_205, pattern_205, pattern_205, pattern_205}, 
   {pattern_187, pattern_187, pattern_187, pattern_187, pattern_187, pattern_59, pattern_59, pattern_59, pattern_59, pattern_59, pattern_179, pattern_179, pattern_179, pattern_179, pattern_179, pattern_58, pattern_154, pattern_154, pattern_154, pattern_154, pattern_141, pattern_141, pattern_141, pattern_141, pattern_141, pattern_225, pattern_225, pattern_225, pattern_225, pattern_225, pattern_42, pattern_42, pattern_42, pattern_42, pattern_42, pattern_1, pattern_1, pattern_1, pattern_1, pattern_1, pattern_122, pattern_122, pattern_122, pattern_122, pattern_122, pattern_260, pattern_260, pattern_260, pattern_260, pattern_260, pattern_87, pattern_87, pattern_87, pattern_87, pattern_87, pattern_40, pattern_40, pattern_40, pattern_40, pattern_40}, 
   {pattern_45, pattern_45, pattern_45, pattern_45, pattern_45, pattern_177, pattern_177, pattern_177, pattern_177, pattern_177, pattern_77, pattern_77, pattern_77, pattern_77, pattern_77, pattern_5, pattern_5, pattern_5, pattern_5, pattern_5, pattern_39, pattern_39, pattern_39, pattern_39, pattern_39, pattern_111, pattern_111, pattern_111, pattern_111, pattern_111, pattern_25, pattern_25, pattern_25, pattern_25, pattern_25, pattern_13, pattern_13, pattern_13, pattern_13, pattern_13, pattern_223, pattern_223, pattern_223, pattern_223, pattern_223, pattern_114, pattern_114, pattern_114, pattern_114, pattern_114, pattern_105, pattern_105, pattern_105, pattern_105, pattern_105, pattern_137, pattern_137, pattern_137, pattern_137, pattern_137}, 
   {pattern_124, pattern_124, pattern_124, pattern_124, pattern_124, pattern_287, pattern_287, pattern_287, pattern_287, pattern_287, pattern_0, pattern_0, pattern_0, pattern_0, pattern_0, pattern_11, pattern_11, pattern_11, pattern_11, pattern_11, pattern_41, pattern_41, pattern_41, pattern_41, pattern_41, pattern_273, pattern_273, pattern_273, pattern_273, pattern_273, pattern_118, pattern_118, pattern_118, pattern_118, pattern_118, pattern_157, pattern_157, pattern_157, pattern_157, pattern_157, pattern_52, pattern_52, pattern_52, pattern_52, pattern_52, pattern_253, pattern_253, pattern_253, pattern_253, pattern_253, pattern_240, pattern_240, pattern_240, pattern_240, pattern_240, pattern_264, pattern_264, pattern_264, pattern_264, pattern_264}
};
const uint16_t ledindexmap[120]  = {   
 122 /* E */,  121 /* S */,  120 /* O */,  119 /* I */,  118 /* S */,  117 /* T */,  116 /* S */,  115 /* P */,  114 /* U */,  113 /* N */,  112 /* K */,  111 /* T */,    
  98 /* I */,   99 /* Z */,  100 /* W */,  101 /* A */,  102 /* N */,  103 /* Z */,  104 /* I */,  105 /* G */,  106 /* Z */,  107 /* E */,  108 /* H */,  109 /* N */,    
  97 /* W */,   96 /* H */,   95 /* A */,   94 /* L */,   93 /* B */,   92 /* V */,   91 /* I */,   90 /* E */,   89 /* R */,   88 /* T */,   87 /* E */,   86 /* L */,    
  74 /* F */,   75 /* Ü */,   76 /* N */,   77 /* F */,   78 /* E */,   79 /* N */,   80 /* A */,   81 /* C */,   82 /* H */,   83 /* V */,   84 /* O */,   85 /* R */,    
  73 /* H */,   72 /* A */,   71 /* L */,   70 /* B */,   69 /* G */,   68 /* D */,   67 /* S */,   66 /* I */,   65 /* E */,   64 /* B */,   63 /* E */,   62 /* N */,    
  50 /* E */,   51 /* L */,   52 /* F */,   53 /* V */,   54 /* I */,   55 /* E */,   56 /* R */,   57 /* N */,   58 /* E */,   59 /* U */,   60 /* N */,   61 /* G */,    
  49 /* E */,   48 /* I */,   47 /* N */,   46 /* S */,   45 /* A */,   44 /* C */,   43 /* H */,   42 /* T */,   41 /* Z */,   40 /* W */,   39 /* E */,   38 /* I */,    
  26 /* S */,   27 /* E */,   28 /* C */,   29 /* H */,   30 /* S */,   31 /* C */,   32 /* Z */,   33 /* W */,   34 /* Ö */,   35 /* L */,   36 /* F */,   37 /* L */,    
  25 /* D */,   24 /* R */,   23 /* E */,   22 /* I */,   21 /* F */,   20 /* Ü */,   19 /* N */,   18 /* F */,   17 /* O */,   16 /* U */,   15 /* N */,   14 /* D */,    
   1 /* Z */,    2 /* E */,    3 /* H */,    4 /* N */,    5 /* H */,    6 /* Z */,    7 /* W */,    8 /* A */,    9 /* N */,   10 /* Z */,   11 /* I */,   12 /* G */
};


const uint16_t points[4] ={0,123,110,13};
boolean ledstate[NUM_LEDS];

void calcleadstate(uint8_t h, uint8_t m){
  for(uint16_t i=0;i<124;i++){
    ledstate[i]=false;
  }
  h=h%24;
  m=m%60;
  //Read pattern Pointer
  const uint8_t* p_pattern = time_table[h][m];
  for(uint16_t i=0;;i++) {
    uint8_t pvalue=p_pattern[i];
    if(pvalue==0xFF){
      break;    
    }
    //Read word Pointer
    const uint16_t* p_word = word_table[pvalue];
    uint16_t leds_start= p_word[0];
    uint16_t leds_length= p_word[1];
    for(uint16_t j=0;j<leds_length;j++){
      //Map Led Index  
      ledstate[ledindexmap[leds_start+j]]=true;  
    }
  }
}


Ticker tledupdate;
CRGB leds[NUM_LEDS];



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint16_t ledupdate_c=0;
void ledupdate()
{
  if(ledupdate_c>=256*5){
    ledupdate_c=0;
  }
  for(uint16_t i=0; i<NUM_LEDS ; i++) {
    if(ledstate[i]==true)
    { 
      leds[i]= Wheel(((i * 256 / NUM_LEDS) + ledupdate_c) & 255);
    }
    else
    {
      leds[i]=CRGB::Black;        
    }
  }
  for(uint8_t i=0;i<4;i++){
    leds[points[i]]=blend(leds[points[i]],CRGB::Black,200);
  }
  
  ledupdate_c++;
  FastLED.show();
}

Ticker ticker;

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

int sweep=0;
void tick()
{
  sweep++;
  if(sweep==4)
    sweep=0;
  for(int i=0;i<NUM_LEDS;i++){
    ledstate[i]=false;
  }    
  ledstate[points[sweep]]=true;    
}

unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

WiFiUDP udp;

void setup() { 
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(  64 );
  tledupdate.attach(0.04, ledupdate);

  

  ticker.attach(1, tick);
  
  WiFiManager wifiManager;

  wifiManager.setAPCallback(configModeCallback);

  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  Serial.println("connected...yeey :)");

  
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

}


int count=0;
int done=0;
int showtime=0;
void loop() { 
  if(showtime){
    //Update time
    calcleadstate(hour(),minute());
    for(int i=0;i<4;i++){
      if(minute()%5>=(i+1)){
        ledstate[points[i]]=true;
      }
    }
    // print the hour, minute and second:
    Serial.print("Time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print(" ");
    Serial.print(hour());
    Serial.print(":");
    Serial.print(minute());
    Serial.print(":");
    Serial.print(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print(".");
    Serial.print(month());
    Serial.print(".");
    Serial.print(year()); 
    Serial.println(); 
    
  }

  count++;

  if(done==1 && count>60*60){ //update every hour
    done=0;
    count=0;
  }
  if(done==0){

    if(count==60){
      ticker.detach();
      ticker.attach(1, tick);
      showtime=0;
    }
    if(count==60*60){
      ESP.reset();
      delay(1000);      
    }
  
    //get a random server from the pool
    WiFi.hostByName(ntpServerName, timeServerIP); 
  
    sendNTPpacket(timeServerIP); // send an NTP packet to a time server
    // wait to see if a reply is available
    delay(1000);
  
    int cb = udp.parsePacket();
    if (!cb) {
      Serial.println("no packet yet");
    }
    else {
      ticker.detach();
  
      Serial.print("packet received, length=");
      Serial.println(cb);
      // We've received a packet, read the data from it
      udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  
      //the timestamp starts at byte 40 of the received packet and is four bytes,
      // or two words, long. First, esxtract the two words:
  
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      Serial.print("Seconds since Jan 1 1900 = " );
      Serial.println(secsSince1900);
  
      // now convert NTP time into everyday time: 
      Serial.print("Unix time = ");
      // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
      const unsigned long seventyYears = 2208988800UL;
      // subtract seventy years:
      unsigned long epoch = secsSince1900 - seventyYears;
      // print Unix time:
      Serial.println(epoch);
      setTime(epoch);
  
      // print the hour, minute and second:
      Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
      Serial.print(" ");
      Serial.print(hour());
      Serial.print(":");
      Serial.print(minute());
      Serial.print(":");
      Serial.print(second());
      Serial.print(" ");
      Serial.print(day());
      Serial.print(".");
      Serial.print(month());
      Serial.print(".");
      Serial.print(year()); 
      Serial.println(); 
  
      
      adjustTime(SECS_PER_HOUR); //winterzeit
      if((month()>3 && month()<10) || (month()==3 && day()>=26) || (month()==10 && day()<=29) ){
        adjustTime(SECS_PER_HOUR); //Sommerzeit
      }

      showtime=1;
      done=1;
      count=0;
  
    }
  }
  else{
    delay(1000);
  }
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
