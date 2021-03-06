/*
Password Vault V3.0
Distributed under the MIT License
© Copyright Maxim Bortnikov 2022
For more information please visit
https://github.com/Northstrix/Password_Vault_V3.0
Required libraries:
https://github.com/zhouyangchao/AES
https://github.com/peterferrie/serpent
https://github.com/ulwanski/sha512
https://github.com/Chris--A/Keypad
https://github.com/johnrickman/LiquidCrystal_I2C
https://github.com/marvinroger/ESP8266TrueRandom
https://github.com/adafruit/Adafruit-ST7735-Library
https://github.com/adafruit/Adafruit-GFX-Library
https://github.com/GyverLibs/GyverBus
https://github.com/PaulStoffregen/PS2Keyboard
*/
#include <SoftwareSerial.h>
#include "sha512.h"
#include "aes.h"
#include "serpent.h"
#include <ESP8266TrueRandom.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "GBUS.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <FS.h>
#define TFT_CS         D2
#define TFT_RST        D3
#define TFT_DC         D4
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
SoftwareSerial mySerial(5, 16); // RX, TX
GBUS bus(&mySerial, 3, 25);
int cur_pos;
char ch;
bool pr_check;
int pr_key;
struct myStruct {
  char x;
};
char *keys[]=
{"4f18b6b1ffd81f9755b0815db942c415834a9bae3bbc838a2d6b33d2f87598fd"};// Serpent's key
int count;
byte tmp_st[8];
char temp_st_for_pp[16];
int m;
int n;
String dec_st;
String keyb_inp;
uint8_t back_key[32];
uint8_t back_s_key[32];
uint8_t key[32] = {
   0xd1,0xf0,0x68,0x5b,
   0x33,0xa0,0xb1,0x73,
   0xb6,0x25,0x54,0xf9,
   0xdd,0x2c,0xd3,0x1d,
   0xc1,0x93,0xb3,0x14,
   0x16,0x76,0x28,0x59,
   0x04,0x85,0xd4,0x24,
   0x9d,0xe0,0x2a,0x74
};

uint8_t second_key[32] = {
   0xfb,0x87,0x9c,0x11,
   0x16,0x97,0xbb,0x14,
   0x3c,0x1e,0x30,0xdb,
   0x67,0xab,0xb8,0x9b,
   0x23,0x5e,0x15,0x9a,
   0xd2,0xdd,0x7c,0x96,
   0x41,0xc9,0x25,0xd3,
   0xd0,0xe1,0x75,0xe3
};

uint8_t projection_key[32] = {
   0xef,0xe7,0x3e,0x31,
   0x61,0x71,0x6c,0xca,
   0x16,0xe8,0xfb,0x24,
   0xd4,0x57,0x7d,0x9a,
   0x74,0x60,0x76,0xaf,
   0x1c,0x42,0x82,0x6d,
   0xf4,0xc3,0x5b,0x51,
   0x69,0x8e,0x24,0x2d
};

uint8_t broadcastAddress[] = {0xEC, 0x94, 0xCB, 0x67, 0x3A, 0x4C};

typedef struct struct_message {
  char l_srp[16];
  char r_srp[16];
  bool n;
} struct_message;

struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void back_k(){
  for(int i = 0; i<32; i++){
    back_key[i] = key[i];
  }
}

void rest_k(){
  for(int i = 0; i<32; i++){
    key[i] = back_key[i];
  }
}

void back_s_k(){
  for(int i = 0; i<32; i++){
    back_s_key[i] = second_key[i];
  }
}

void rest_s_k(){
  for(int i = 0; i<32; i++){
    second_key[i] = back_s_key[i];
  }
}

void incr_key(){
  if(key[0] == 255){
    key[0] = 0;
    if(key[1] == 255){
      key[1] = 0;
      if(key[2] == 255){
        key[2] = 0;
        if(key[3] == 255){
          key[3] = 0;

  if(key[4] == 255){
    key[4] = 0;
    if(key[5] == 255){
      key[5] = 0;
      if(key[6] == 255){
        key[6] = 0;
        if(key[7] == 255){
          key[7] = 0;
          
  if(key[8] == 255){
    key[8] = 0;
    if(key[9] == 255){
      key[9] = 0;
      if(key[10] == 255){
        key[10] = 0;
        if(key[11] == 255){
          key[11] = 0;

  if(key[12] == 255){
    key[12] = 0;
    if(key[13] == 255){
      key[13] = 0;
      if(key[14] == 255){
        key[14] = 0;
        if(key[15] == 255){
          key[15] = 0;
        }
        else{
          key[15]++;
        }
        }
      else{
        key[14]++;
      }
    }
    else{
      key[13]++;
    }
  }
  else{
    key[12]++;
  }
          
        }
        else{
          key[11]++;
        }
        }
      else{
        key[10]++;
      }
    }
    else{
      key[9]++;
    }
  }
  else{
    key[8]++;
  }
          
        }
        else{
          key[7]++;
        }
        }
      else{
        key[6]++;
      }
    }
    else{
      key[5]++;
    }
  }
  else{
    key[4]++;
  }
          
        }
        else{
          key[3]++;
        }
        }
      else{
        key[2]++;
      }
    }
    else{
      key[1]++;
    }
  }
  else{
    key[0]++;
  }
}

void incr_second_key(){
  if(second_key[0] == 255){
    second_key[0] = 0;
    if(second_key[1] == 255){
      second_key[1] = 0;
      if(second_key[2] == 255){
        second_key[2] = 0;
        if(second_key[3] == 255){
          second_key[3] = 0;

  if(second_key[4] == 255){
    second_key[4] = 0;
    if(second_key[5] == 255){
      second_key[5] = 0;
      if(second_key[6] == 255){
        second_key[6] = 0;
        if(second_key[7] == 255){
          second_key[7] = 0;
          
  if(second_key[8] == 255){
    second_key[8] = 0;
    if(second_key[9] == 255){
      second_key[9] = 0;
      if(second_key[10] == 255){
        second_key[10] = 0;
        if(second_key[11] == 255){
          second_key[11] = 0;

  if(second_key[12] == 255){
    second_key[12] = 0;
    if(second_key[13] == 255){
      second_key[13] = 0;
      if(second_key[14] == 255){
        second_key[14] = 0;
        if(second_key[15] == 255){
          second_key[15] = 0;
        }
        else{
          second_key[15]++;
        }
        }
      else{
        second_key[14]++;
      }
    }
    else{
      second_key[13]++;
    }
  }
  else{
    second_key[12]++;
  }
          
        }
        else{
          second_key[11]++;
        }
        }
      else{
        second_key[10]++;
      }
    }
    else{
      second_key[9]++;
    }
  }
  else{
    second_key[8]++;
  }
          
        }
        else{
          second_key[7]++;
        }
        }
      else{
        second_key[6]++;
      }
    }
    else{
      second_key[5]++;
    }
  }
  else{
    second_key[4]++;
  }
          
        }
        else{
          second_key[3]++;
        }
        }
      else{
        second_key[2]++;
      }
    }
    else{
      second_key[1]++;
    }
  }
  else{
    second_key[0]++;
  }
}

void incr_projection_key(){
  if(projection_key[0] == 255){
    projection_key[0] = 0;
    if(projection_key[1] == 255){
      projection_key[1] = 0;
      if(projection_key[2] == 255){
        projection_key[2] = 0;
        if(projection_key[3] == 255){
          projection_key[3] = 0;

  if(projection_key[4] == 255){
    projection_key[4] = 0;
    if(projection_key[5] == 255){
      projection_key[5] = 0;
      if(projection_key[6] == 255){
        projection_key[6] = 0;
        if(projection_key[7] == 255){
          projection_key[7] = 0;
          
  if(projection_key[8] == 255){
    projection_key[8] = 0;
    if(projection_key[9] == 255){
      projection_key[9] = 0;
      if(projection_key[10] == 255){
        projection_key[10] = 0;
        if(projection_key[11] == 255){
          projection_key[11] = 0;

  if(projection_key[12] == 255){
    projection_key[12] = 0;
    if(projection_key[13] == 255){
      projection_key[13] = 0;
      if(projection_key[14] == 255){
        projection_key[14] = 0;
        if(projection_key[15] == 255){
          projection_key[15] = 0;
        }
        else{
          projection_key[15]++;
        }
        }
      else{
        projection_key[14]++;
      }
    }
    else{
      projection_key[13]++;
    }
  }
  else{
    projection_key[12]++;
  }
          
        }
        else{
          projection_key[11]++;
        }
        }
      else{
        projection_key[10]++;
      }
    }
    else{
      projection_key[9]++;
    }
  }
  else{
    projection_key[8]++;
  }
          
        }
        else{
          projection_key[7]++;
        }
        }
      else{
        projection_key[6]++;
      }
    }
    else{
      projection_key[5]++;
    }
  }
  else{
    projection_key[4]++;
  }
          
        }
        else{
          projection_key[3]++;
        }
        }
      else{
        projection_key[2]++;
      }
    }
    else{
      projection_key[1]++;
    }
  }
  else{
    projection_key[0]++;
  }
}

int gen_r_num(){
  int rn = ESP8266TrueRandom.random(0,256);
  return rn;
}

int getNum(char ch)
{
    int num=0;
    if(ch>='0' && ch<='9')
    {
        num=ch-0x30;
    }
    else
    {
        switch(ch)
        {
            case 'A': case 'a': num=10; break;
            case 'B': case 'b': num=11; break;
            case 'C': case 'c': num=12; break;
            case 'D': case 'd': num=13; break;
            case 'E': case 'e': num=14; break;
            case 'F': case 'f': num=15; break;
            default: num=0;
        }
    }
    return num;
}

char getChar(int num){
  char ch;
    if(num>=0 && num<=9)
    {
        ch = char(num+48);
    }
    else
    {
        switch(num)
        {
            case 10: ch='a'; break;
            case 11: ch='b'; break;
            case 12: ch='c'; break;
            case 13: ch='d'; break;
            case 14: ch='e'; break;
            case 15: ch='f'; break;
        }
    }
    return ch;
}

size_t hex2bin (void *bin, char hex[]) {
  size_t len, i;
  int x;
  uint8_t *p=(uint8_t*)bin;
  
  len = strlen (hex);
  
  if ((len & 1) != 0) {
    return 0; 
  }
  
  for (i=0; i<len; i++) {
    if (isxdigit((int)hex[i]) == 0) {
      return 0; 
    }
  }
  
  for (i=0; i<len / 2; i++) {
    sscanf (&hex[i * 2], "%2x", &x);
    p[i] = (uint8_t)x;
  } 
  return len / 2;
}

void split_by_eight(char plntxt[], int k, int str_len, bool add_aes){
  char plt_data[] = {0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 8; i++){
      if(i+k > str_len - 1)
      break;
      plt_data[i] = plntxt[i+k];
  }
  char t_encr[16];
  for(int i = 0; i<8; i++){
      t_encr[i] = plt_data[i];
  }
  for(int i = 8; i<16; i++){
      t_encr[i] = gen_r_num();
  }
  encr_AES(t_encr, add_aes);
}

void encr_AES(char t_enc[], bool add_aes){
  uint8_t text[16];
  for(int i = 0; i<16; i++){
    int c = int(t_enc[i]);
    text[i] = c;
  }
  uint8_t cipher_text[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint32_t key_bit[3] = {128, 192, 256};
  aes_context ctx;
  aes_set_key(&ctx, key, key_bit[m]);
  aes_encrypt_block(&ctx, cipher_text, text);
  /*
  for (int i = 0; i < 16; ++i) {
    Serial.printf("%02x", cipher_text[i]);
  }
  */
  char L_half[16];
  for(int i = 0; i<8; i++){
    L_half[i] = cipher_text[i];
  }
  char R_half[16];
  for(int i = 0; i<8; i++){
    R_half[i] = cipher_text[i+8];
  }
  for(int i = 8; i<16; i++){
    L_half[i] = gen_r_num();
    R_half[i] = gen_r_num();
  }
  serp_enc(L_half, add_aes);
  serp_enc(R_half, add_aes);
}

void serp_enc(char res[], bool add_aes){
  int tmp_s[16];
  for(int i = 0; i < 16; i++){
      tmp_s[i] = res[i];
  }
  /*
   for (int i = 0; i < 16; i++){
     Serial.print(res[i]);
  }
  Serial.println();
  */
  uint8_t ct1[32], pt1[32], key[64];
  int plen, clen, b, j;
  serpent_key skey;
  serpent_blk ct2;
  uint32_t *p;
  
  for (b=0; b<sizeof(keys)/sizeof(char*); b++) {
    hex2bin (key, keys[b]);
  
    // set key
    memset (&skey, 0, sizeof (skey));
    p=(uint32_t*)&skey.x[0][0];
    
    serpent_setkey (&skey, key);
    //Serial.printf ("\nkey=");
    /*
    for (j=0; j<sizeof(skey)/sizeof(serpent_subkey_t)*4; j++) {
      if ((j % 8)==0) putchar('\n');
      Serial.printf ("%08X ", p[j]);
    }
    */
    for(int i = 0; i < 16; i++){
        ct2.b[i] = tmp_s[i];
    }
  serpent_encrypt (ct2.b, &skey, SERPENT_ENCRYPT);
  if(add_aes == false){
    for (int i=0; i<16; i++) {
      if(ct2.b[i]<16)
        Serial.print("0");
      Serial.print(ct2.b[i],HEX);
    }
  }
  if(add_aes == true)
  encr_sec_AES(ct2.b);
  }
}

void encr_sec_AES(byte t_enc[]){
  uint8_t text[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for(int i = 0; i<16; i++){
    int c = int(t_enc[i]);
    text[i] = c;
  }
  uint8_t cipher_text[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint32_t second_key_bit[3] = {128, 192, 256};
  int i = 0;
  aes_context ctx;
  aes_set_key(&ctx, second_key, second_key_bit[m]);
  aes_encrypt_block(&ctx, cipher_text, text);
  for (i = 0; i < 16; ++i) {
    Serial.printf("%02x", cipher_text[i]);
  }
}

void split_dec(char ct[], int ct_len, int p, bool ch, bool add_r){
  int br = false;
  byte res[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 32; i+=2){
    if(i+p > ct_len - 1){
      br = true;
      break;
    }
    if (i == 0){
    if(ct[i+p] != 0 && ct[i+p+1] != 0)
    res[i] = 16*getNum(ct[i+p])+getNum(ct[i+p+1]);
    if(ct[i+p] != 0 && ct[i+p+1] == 0)
    res[i] = 16*getNum(ct[i+p]);
    if(ct[i+p] == 0 && ct[i+p+1] != 0)
    res[i] = getNum(ct[i+p+1]);
    if(ct[i+p] == 0 && ct[i+p+1] == 0)
    res[i] = 0;
    }
    else{
    if(ct[i+p] != 0 && ct[i+p+1] != 0)
    res[i/2] = 16*getNum(ct[i+p])+getNum(ct[i+p+1]);
    if(ct[i+p] != 0 && ct[i+p+1] == 0)
    res[i/2] = 16*getNum(ct[i+p]);
    if(ct[i+p] == 0 && ct[i+p+1] != 0)
    res[i/2] = getNum(ct[i+p+1]);
    if(ct[i+p] == 0 && ct[i+p+1] == 0)
    res[i/2] = 0;
    }
  }
    if(br == false){
      if(add_r == true){
      uint8_t ret_text[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      uint8_t cipher_text[16] = {0};
      for(int i = 0; i<16; i++){
        int c = int(res[i]);
        cipher_text[i] = c;
      }
      uint32_t second_key_bit[3] = {128, 192, 256};
      int i = 0;
      aes_context ctx;
      aes_set_key(&ctx, second_key, second_key_bit[m]);
      aes_decrypt_block(&ctx, ret_text, cipher_text);
      for (i = 0; i < 16; ++i) {
        res[i] = (char)ret_text[i];
      }
      }
      uint8_t ct1[32], pt1[32], key[64];
      int plen, clen, i, j;
      serpent_key skey;
      serpent_blk ct2;
      uint32_t *p;
  
  for (i=0; i<sizeof(keys)/sizeof(char*); i++) {
    hex2bin (key, keys[i]);
  
    // set key
    memset (&skey, 0, sizeof (skey));
    p=(uint32_t*)&skey.x[0][0];
    
    serpent_setkey (&skey, key);
    //Serial.printf ("\nkey=");

    for (j=0; j<sizeof(skey)/sizeof(serpent_subkey_t)*4; j++) {
      if ((j % 8)==0) putchar('\n');
      //Serial.printf ("%08X ", p[j]);
    }

    for(int i = 0; i <16; i++)
      ct2.b[i] = res[i];
    /*
    Serial.printf ("\n\n");
    for(int i = 0; i<16; i++){
    Serial.printf("%x", ct2.b[i]);
    Serial.printf(" ");
    */
    }
    //Serial.printf("\n");
    serpent_encrypt (ct2.b, &skey, SERPENT_DECRYPT);
    if (ch == false){
    for (int i=0; i<8; i++) {
      tmp_st[i] = char(ct2.b[i]);
    }
    }
    if (ch == true){
      decr_AES(ct2.b);
    }
  }
}

void decr_AES(byte sh[]){
  uint8_t ret_text[16];
  for(int i = 0; i<8; i++){
    ret_text[i] = tmp_st[i];
  }
  for(int i = 0; i<8; i++){
    ret_text[i+8] = sh[i];
  }
      uint8_t cipher_text[16] = {0};
      for(int i = 0; i<16; i++){
        int c = int(ret_text[i]);
        cipher_text[i] = c;
      }
      uint32_t key_bit[3] = {128, 192, 256};
      int i = 0;
      aes_context ctx;
      aes_set_key(&ctx, key, key_bit[m]);
      aes_decrypt_block(&ctx, ret_text, cipher_text);
      for (i = 0; i < 8; ++i) {
        dec_st += (char(ret_text[i]));
      }
}

void project(){
  if(pr_check == false)
    set_key_for_pp(false);
  else
    proj_pass();
}

void set_key_for_pp(bool cs){
      tft.fillScreen(0x65d9);
      tft.setTextColor(0xffff, 0x65d9);
      if (cs == true){
        tft.fillScreen(0x2145);
        tft.setTextColor(0xdefb, 0x2145);
      }
      tft.setCursor(0,0);
      tft.println("Type this key on the      receiver board's keypad");
      String pass_f_p = "";
      for(int i = 0; i<8; i++){
        int rnd_n = ESP8266TrueRandom.random(16,256);
        pass_f_p += String(rnd_n, HEX);
        pass_f_p += " ";
      }
      Serial.println(pass_f_p);
      tft.setTextColor(0xffff, 0x65d9);
      if (cs == true){
        tft.setTextColor(0xf7be, 0x2145);
      }
      tft.setCursor(10,30);
      tft.println(pass_f_p);
      int str_len = pass_f_p.length() + 1;
      char input_arr[str_len];
      pass_f_p.toCharArray(input_arr, str_len);
      std::string str = "";
      if(str_len > 1){
        for(int i = 0; i<str_len-1; i++){
          str += input_arr[i];
        }
      }
      String h = sha512( str ).c_str();
      int h_len = h.length() + 1;
      char h_array[h_len];
      h.toCharArray(h_array, h_len);
      byte res[16] = {0};
      for (int i = 0; i < 32; i+=2){
      if (i == 0){
      if(h_array[i] != 0 && h_array[i+1] != 0)
      res[i] = 16*getNum(h_array[i])+getNum(h_array[i+1]);
      if(h_array[i] != 0 && h_array[i+1] == 0)
      res[i] = 16*getNum(h_array[i]);
      if(h_array[i] == 0 && h_array[i+1] != 0)
      res[i] = getNum(h_array[i+1]);
      if(h_array[i] == 0 && h_array[i+1] == 0)
      res[i] = 0;
      }
      else{
      if(h_array[i] != 0 && h_array[i+1] != 0)
      res[i/2] = 16*getNum(h_array[i])+getNum(h_array[i+1]);
      if(h_array[i] != 0 && h_array[i+1] == 0)
      res[i/2] = 16*getNum(h_array[i]);
      if(h_array[i] == 0 && h_array[i+1] != 0)
      res[i/2] = getNum(h_array[i+1]);
      if(h_array[i] == 0 && h_array[i+1] == 0)
      res[i/2] = 0;
      }
     }
     uint8_t ct1[32], pt1[32], key[64];
     int plen, clen, i, j;
     serpent_key skey;
     serpent_blk ct2;
     uint32_t *p;
     for (i=0; i<sizeof(keys)/sizeof(char*); i++) {
      hex2bin (key, keys[i]);
      memset (&skey, 0, sizeof (skey));
      p=(uint32_t*)&skey.x[0][0];
      serpent_setkey (&skey, key);
      for (j=0; j<sizeof(skey)/sizeof(serpent_subkey_t)*4; j++) {
        if ((j % 8)==0) putchar('\n');
      }
      for(int i = 0; i <16; i++)
        ct2.b[i] = res[i];
      }
    for(int i = 0; i<931; i++)
      serpent_encrypt (ct2.b, &skey, SERPENT_ENCRYPT);
    for(int i = 0; i < 11; i++){
      projection_key[i] = ct2.b[i];
    }
    tft.setTextColor(0xffff, 0x65d9);
    if (cs == true){
      tft.setTextColor(0xdefb, 0x2145);
    }
    tft.setCursor(0,55);
    tft.print("You should obtain these   verification numbers");
    tft.setTextColor(0xffff, 0x65d9);
    if (cs == true){
      tft.setTextColor(0xf7be, 0x2145);
    }
    tft.setCursor(0,80);
    tft.print(ct2.b[15]);
    tft.setCursor(70,80);
    tft.print(ct2.b[14]);
    tft.setCursor(140,80);
    tft.print(ct2.b[13]);
    if (cs == true){
      tft.setTextColor(0xdefb, 0x2145);
    }
    tft.setCursor(0,110);
    tft.println("Press any key to continue.");
    keyb_inp = "";
    while (!bus.gotData()){
      bus.tick();
    }
    pr_check = true;
    if (cs == false){
      proj_pass();
    }
    return;
}

void proj_pass(){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the password:");
  while (pr_key != 27){
    bus.tick();
    if (bus.gotData()) {
      myStruct data;
      bus.readData(data);
      ch = data.x;
      pr_key = int(ch);
      if(pr_key != 127 && pr_key != 13 && pr_key != 9 && pr_key != 10 && pr_key != 11){
        keyb_inp += ch;
      }
      else if (ch == 127) {
        tft.fillScreen(0x2145);
        if(keyb_inp.length() > 0)
          keyb_inp.remove(keyb_inp.length() -1, 1);
      }
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the password:");
  tft.setTextColor(0xf7be, 0x2145);
  tft.setCursor(0,10);
  tft.println(keyb_inp);
  if (pr_key == 13){
      int str_len = keyb_inp.length() + 1;
      char char_array[str_len];
      keyb_inp.toCharArray(char_array, str_len);
      int p = 0;
      while( str_len > p+1){
        split_by_eight_for_pass_proj(char_array, p, str_len);
        p+=8;
      }
    keyb_inp = "";
    disp_m_menu();
    return;
  }
  if (pr_key == 27){
     keyb_inp = "";
     disp_m_menu();
     return;
  }
  }
 }
}

void split_by_eight_for_pass_proj(char plntxt[], int k, int str_len){
  char res[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 8; i++){
      if(i+k > str_len - 1)
      break;
      res[i] = plntxt[i+k];
  }
  for (int i = 8; i < 16; i++){
      res[i] = gen_r_num();
  }
  /*
   for (int i = 0; i < 8; i++){
     Serial.print(res[i]);
  }
  Serial.println();
  */
  encr_AES_for_pp(res);
}

void encr_AES_for_pp(char t_enc[]){
  uint8_t text[16];
  for(int i = 0; i<16; i++){
    int c = int(t_enc[i]);
    text[i] = c;
  }
  uint8_t cipher_text[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint32_t key_bit[3] = {128, 192, 256};
  aes_context ctx;
  aes_set_key(&ctx, projection_key, key_bit[m]);
  aes_encrypt_block(&ctx, cipher_text, text);
  /*
  for (int i = 0; i < 16; ++i) {
    Serial.printf("%02x", cipher_text[i]);
  }
  */
  char L_half[16];
  for(int i = 0; i<8; i++){
    L_half[i] = cipher_text[i];
  }
  char R_half[16];
  for(int i = 0; i<8; i++){
    R_half[i] = cipher_text[i+8];
  }
  for(int i = 8; i<16; i++){
    L_half[i] = gen_r_num();
    R_half[i] = gen_r_num();
  }
  serp_for_pp(L_half, false);
  serp_for_pp(R_half, true);
}

void serp_for_pp(char res[], bool snd){
  int tmp_s[16];
  for(int i = 0; i < 16; i++){
      tmp_s[i] = res[i];
  }
  /*
   for (int i = 0; i < 16; i++){
     Serial.print(res[i]);
  }
  Serial.println();
  */
  uint8_t ct1[32], pt1[32], key[64];
  int plen, clen, b, j;
  serpent_key skey;
  serpent_blk ct2;
  uint32_t *p;
  
  for (b=0; b<sizeof(keys)/sizeof(char*); b++) {
    hex2bin (key, keys[b]);
  
    // set key
    memset (&skey, 0, sizeof (skey));
    p=(uint32_t*)&skey.x[0][0];
    
    serpent_setkey (&skey, key);
    //Serial.printf ("\nkey=");
    /*
    for (j=0; j<sizeof(skey)/sizeof(serpent_subkey_t)*4; j++) {
      if ((j % 8)==0) putchar('\n');
      Serial.printf ("%08X ", p[j]);
    }
    */
    for(int i = 0; i < 16; i++){
        ct2.b[i] = tmp_s[i];
    }
  serpent_encrypt (ct2.b, &skey, SERPENT_ENCRYPT);
    for (int i=0; i<16; i++) {
      if(ct2.b[i]<16)
        Serial.print("0");
      Serial.print(ct2.b[i],HEX);
    }
    if (snd == false){
     for(int i = 0; i <16; i++){
      temp_st_for_pp[i] = ct2.b[i];
     }
    }
    if (snd == true){
     for(int i = 0; i <16; i++){
      myData.l_srp[i] = temp_st_for_pp[i];
      myData.r_srp[i] = ct2.b[i];
     }
     myData.n = n;
     esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
     incr_projection_key();
     n = true;;
     delayMicroseconds(240);
    }
  }
}

void disp_m_menu(){
  tft.fillScreen(0x3aca);
  tft.setTextColor(0xf75b, 0x3aca);
  tft.setTextSize(1);
  tft.setCursor(15,7);
  tft.print("Encrypt password");
  tft.setCursor(15,17);
  tft.print("Decrypt password");
  tft.setCursor(15,27);
  tft.print("Set master password");
  tft.setCursor(15,37);
  tft.print("Set IV");
  tft.setCursor(15,47);
  tft.print("Save file into memory");
  tft.setCursor(15,57);
  tft.print("Load file from memory");
  tft.setCursor(15,67);
  tft.print("Remove file from memory");
  tft.setCursor(15,77);
  tft.print("List all stored files");
  tft.setCursor(15,87);
  tft.print("Project password");
  tft.setCursor(15,97);
  tft.print("Hash string with SHA512");
  tft.setCursor(15,107);
  tft.print("Additional features");
}

void disp_cur_pos(){
  tft.setTextColor(0xf75b, 0x3aca);
  tft.setTextSize(1);
  if (cur_pos == 0){
    tft.setCursor(5,7);
    tft.print("*");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 1){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print("*");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 2){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print("*");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 3){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print("*");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 4){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print("*");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 5){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print("*");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 6){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print("*");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 7){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print("*");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 8){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print("*");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 9){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print("*");
    tft.setCursor(5,107);
    tft.print(" ");
  }
  if (cur_pos == 10){
    tft.setCursor(5,7);
    tft.print(" ");
    tft.setCursor(5,17);
    tft.print(" ");
    tft.setCursor(5,27);
    tft.print(" ");
    tft.setCursor(5,37);
    tft.print(" ");
    tft.setCursor(5,47);
    tft.print(" ");
    tft.setCursor(5,57);
    tft.print(" ");
    tft.setCursor(5,67);
    tft.print(" ");
    tft.setCursor(5,77);
    tft.print(" ");
    tft.setCursor(5,87);
    tft.print(" ");
    tft.setCursor(5,97);
    tft.print(" ");
    tft.setCursor(5,107);
    tft.print("*");
  }
}

void enc_rec(){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the password:");
  while (pr_key != 27){
    bus.tick();
    if (bus.gotData()) {
      myStruct data;
      bus.readData(data);
      // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
      ch = data.x;
      pr_key = int(ch);
      if(pr_key != 127 && pr_key != 13 && pr_key != 9 && pr_key != 10 && pr_key != 11){
        keyb_inp += ch;
      }
      else if (ch == 127) {
        tft.fillScreen(0x2145);
        if(keyb_inp.length() > 0)
          keyb_inp.remove(keyb_inp.length() -1, 1);
      }
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the password:");
  tft.setTextColor(0xf7be, 0x2145);
  tft.setCursor(0,10);
  tft.println(keyb_inp);
  if (pr_key == 13){
    int str_len = keyb_inp.length() + 1;
    char keyb_inp_arr[str_len];
    keyb_inp.toCharArray(keyb_inp_arr, str_len);
    Serial.println("Ciphertext:");
    int p = 0;
    while(str_len > p+1){
      incr_key();
      incr_second_key();
      split_by_eight(keyb_inp_arr, p, str_len, true);
      p+=8;
    }
    rest_k();
    rest_s_k();
    Serial.println("");
    keyb_inp = "";
    disp_m_menu();
    return;
  }
  if (pr_key == 27){
     keyb_inp = "";
     disp_m_menu();
     return;
  }
  }
 }
}

void dec_rec(){
 while (pr_key != 27){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Paste the ciphertext:");
  dec_st = "";
  String ct;
  Serial.println("Paste the ciphertext:");
  while (!Serial.available()) {
    bus.tick();
   if (bus.gotData()) {
    myStruct data;
    bus.readData(data);
    // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
    ch = data.x;
    pr_key = int(ch);
    if (pr_key == 27){
      keyb_inp = "";
      dec_st = "";
      disp_m_menu();
      return;
    }
   }  
  }
  ct = Serial.readString();
  int ct_len = ct.length() + 1;
  char ct_array[ct_len];
  ct.toCharArray(ct_array, ct_len);
  int ext = 0;
  count = 0;
  bool ch = false;
  while(ct_len > ext){
  if(count%2 == 1 && count !=0)
    ch = true;
  else{
    ch = false;
      incr_key();
      incr_second_key();
  }
  split_dec(ct_array, ct_len, 0+ext, ch, true);
  ext+=32;
  count++;
  }
  rest_k();
  rest_s_k();
  Serial.println("Decrypted password:");
  Serial.println(dec_st);
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Decrypted password:");
  tft.setTextColor(0xf7be, 0x2145);
  tft.setCursor(0,10);
  tft.println(dec_st);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,100);
  tft.println("Press any key to return tothe main menu.");
  keyb_inp = "";
  dec_st = "";
  while (!bus.gotData()){
    bus.tick();
  }
  disp_m_menu();
  return;
 }
}

void set_mp(){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the master password:");
  while (pr_key != 27){
    bus.tick();
    if (bus.gotData()) {
      myStruct data;
      bus.readData(data);
      // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
      ch = data.x;
      pr_key = int(ch);
      if(pr_key != 127 && pr_key != 13 && pr_key != 9 && pr_key != 10 && pr_key != 11){
        keyb_inp += ch;
      }
      else if (ch == 127) {
        tft.fillScreen(0x2145);
        if(keyb_inp.length() > 0)
          keyb_inp.remove(keyb_inp.length() -1, 1);
      }
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the master password:");
  tft.setTextColor(0xf7be, 0x2145);
  tft.setCursor(0,10);
  tft.println(keyb_inp);
  if (pr_key == 13){
      int str_len = keyb_inp.length() + 1;
      char input_arr[str_len];
      keyb_inp.toCharArray(input_arr, str_len);
      std::string str = "";
      if(str_len > 1){
        for(int i = 0; i<str_len-1; i++){
          str += input_arr[i];
        }
      }
      String h = sha512( str ).c_str();
      int h_len = h.length() + 1;
      char h_array[h_len];
      h.toCharArray(h_array, h_len);
      byte res[16] = {0};
      for (int i = 0; i < 32; i+=2){
      if (i == 0){
      if(h_array[i] != 0 && h_array[i+1] != 0)
      res[i] = 16*getNum(h_array[i])+getNum(h_array[i+1]);
      if(h_array[i] != 0 && h_array[i+1] == 0)
      res[i] = 16*getNum(h_array[i]);
      if(h_array[i] == 0 && h_array[i+1] != 0)
      res[i] = getNum(h_array[i+1]);
      if(h_array[i] == 0 && h_array[i+1] == 0)
      res[i] = 0;
      }
      else{
      if(h_array[i] != 0 && h_array[i+1] != 0)
      res[i/2] = 16*getNum(h_array[i])+getNum(h_array[i+1]);
      if(h_array[i] != 0 && h_array[i+1] == 0)
      res[i/2] = 16*getNum(h_array[i]);
      if(h_array[i] == 0 && h_array[i+1] != 0)
      res[i/2] = getNum(h_array[i+1]);
      if(h_array[i] == 0 && h_array[i+1] == 0)
      res[i/2] = 0;
      }
     }
     uint8_t ct1[32], pt1[32], key[64];
     int plen, clen, i, j;
     serpent_key skey;
     serpent_blk ct2;
     uint32_t *p;
     for (i=0; i<sizeof(keys)/sizeof(char*); i++) {
      hex2bin (key, keys[i]);
      memset (&skey, 0, sizeof (skey));
      p=(uint32_t*)&skey.x[0][0];
      serpent_setkey (&skey, key);
      for (j=0; j<sizeof(skey)/sizeof(serpent_subkey_t)*4; j++) {
        if ((j % 8)==0) putchar('\n');
      }
      for(int i = 0; i <16; i++)
        ct2.b[i] = res[i];
      }
      for(int i = 0; i<576; i++)
        serpent_encrypt (ct2.b, &skey, SERPENT_DECRYPT);
      key[0] = ct2.b[0];
      key[1] = ct2.b[1];
      key[3] = ct2.b[2];
      key[4] = ct2.b[3];
      key[6] = ct2.b[4];
      key[7] = ct2.b[5];
      key[8] = ct2.b[12];
      second_key[0] = ct2.b[6];
      second_key[1] = ct2.b[7];
      second_key[3] = ct2.b[8];
      second_key[4] = ct2.b[9];
      second_key[6] = ct2.b[10];
      second_key[7] = ct2.b[11];
      second_key[8] = ct2.b[13];
    tft.fillScreen(0x2145);
    tft.setTextColor(0xdefb, 0x2145);
    tft.setCursor(0,0);
    tft.println("Key derived successfully.");
    tft.setCursor(0,10);
    tft.print("Verification number is ");
    tft.setTextColor(0xf7be, 0x2145);
    tft.print(ct2.b[14]);
    tft.setTextColor(0xdefb, 0x2145);
    tft.setCursor(0,100);
    tft.println("Press any key to return tothe main menu.");
    keyb_inp = "";
    while (!bus.gotData()){
      bus.tick();
    }
    disp_m_menu();
    return;
  }
  if (pr_key == 27){
     keyb_inp = "";
     disp_m_menu();
     return;
  }
  }
 }
}

void set_iv(){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the IV:");
  while (pr_key != 27){
    bus.tick();
    if (bus.gotData()) {
      myStruct data;
      bus.readData(data);
      // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
      ch = data.x;
      pr_key = int(ch);
      if(pr_key != 127 && pr_key != 13 && pr_key != 9 && pr_key != 10 && pr_key != 11){
        keyb_inp += ch;
      }
      else if (ch == 127) {
        tft.fillScreen(0x2145);
        if(keyb_inp.length() > 0)
          keyb_inp.remove(keyb_inp.length() -1, 1);
      }
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the IV:");
  tft.setTextColor(0xf7be, 0x2145);
  tft.setCursor(0,10);
  tft.println(keyb_inp);
  if (pr_key == 13){
    int itr = keyb_inp.toInt();
    for(int i = 0; i < itr; i++){
      incr_key();
    }
    keyb_inp = "";
    disp_m_menu();
    return;
  }
  if (pr_key == 27){
     keyb_inp = "";
     disp_m_menu();
     return;
  }
  }
 }
}

void hash_str(){
  tft.fillScreen(0x49a9);
  tft.setTextColor(0xee76, 0x49a9);
  tft.setCursor(0,0);
  tft.println("Enter the string to hash:");
  while (pr_key != 27){
    bus.tick();
    if (bus.gotData()) {
      myStruct data;
      bus.readData(data);
      // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
      ch = data.x;
      pr_key = int(ch);
      if(pr_key != 127 && pr_key != 13 && pr_key != 9 && pr_key != 10 && pr_key != 11){
        keyb_inp += ch;
      }
      else if (ch == 127) {
        tft.fillScreen(0x49a9);
        if(keyb_inp.length() > 0)
          keyb_inp.remove(keyb_inp.length() -1, 1);
      }
  tft.setTextColor(0xee76, 0x49a9);
  tft.setCursor(0,0);
  tft.println("Enter the string to hash:");
  tft.setTextColor(0xf75b, 0x49a9);
  tft.setCursor(0,10);
  tft.println(keyb_inp);
  if (pr_key == 13){
    int str_len = keyb_inp.length() + 1;
    char keyb_inp_arr[str_len];
    keyb_inp.toCharArray(keyb_inp_arr, str_len);
    std::string str = "";
    if(str_len > 1){
      for(int i = 0; i<str_len-1; i++){
        str += keyb_inp_arr[i];
      }
    }
    String h = sha512( str ).c_str();
    //Serial.println(h);
    tft.fillScreen(0x49a9);
    tft.setTextColor(0xee76, 0x49a9);
    tft.setCursor(0,0);
    tft.println("Resulted hash:");
    tft.setTextColor(0xf75b, 0x49a9);
    tft.setCursor(0,10);
    tft.println(h);
    tft.setTextColor(0xee76, 0x49a9);
    tft.setCursor(0,100);
    tft.println("Press any key to return tothe main menu.");
    keyb_inp = "";
    while (!bus.gotData()){
      bus.tick();
    }
    disp_m_menu();
    return;
  }
  if (pr_key == 27){
     keyb_inp = "";
     disp_m_menu();
     return;
  }
  }
 }
}

String readFile(fs::FS &fs, String path){
  Serial.println("Content of the file - " + path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, String path, String message){
  Serial.println("Writing file - " + path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}

void removeFile(fs::FS &fs, String path){
  fs.remove(path);
}

void save_f(){
 while (pr_key != 27){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the name of the new file");
  Serial.println("Enter the name of the new file");
  while (!Serial.available()) {
    bus.tick();
   if (bus.gotData()) {
    myStruct data;
    bus.readData(data);
    // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
    ch = data.x;
    pr_key = int(ch);
    if (pr_key == 27){
      keyb_inp = "";
      dec_st = "";
      disp_m_menu();
      return;
    }
   }  
  }
  String fn;
  fn = Serial.readString();
  save_f_cont(fn);
  return;
 }
}

void save_f_cont(String fn){
 while (pr_key != 27){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the content of the new file");
  Serial.println("Enter the content of the new file");
  while (!Serial.available()) {
    bus.tick();
   if (bus.gotData()) {
    myStruct data;
    bus.readData(data);
    // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
    ch = data.x;
    pr_key = int(ch);
    if (pr_key == 27){
      keyb_inp = "";
      dec_st = "";
      disp_m_menu();
      return;
    }
   }  
  }
  String cnt;
  cnt = Serial.readString();
  writeFile(SPIFFS, fn, cnt);
  disp_m_menu();
  return;
 }
}

void load_f(){
 while (pr_key != 27){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the name of the fileto load:");
  Serial.println("Enter the name of the file to load:");
  while (!Serial.available()) {
    bus.tick();
   if (bus.gotData()) {
    myStruct data;
    bus.readData(data);
    // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
    ch = data.x;
    pr_key = int(ch);
    if (pr_key == 27){
      keyb_inp = "";
      dec_st = "";
      disp_m_menu();
      return;
    }
   }  
  }
  String ld;
  ld = Serial.readString();
  readFile(SPIFFS, ld);
  disp_m_menu();
  return;
 }
}

void remove_f(){
 while (pr_key != 27){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the name of the fileto remove:");
  Serial.println("Enter the name of the file to remove:");
  while (!Serial.available()) {
    bus.tick();
   if (bus.gotData()) {
    myStruct data;
    bus.readData(data);
    // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
    ch = data.x;
    pr_key = int(ch);
    if (pr_key == 27){
      keyb_inp = "";
      dec_st = "";
      disp_m_menu();
      return;
    }
   }  
  }
  String rmv;
  rmv = Serial.readString();
  removeFile(SPIFFS, rmv);
  disp_m_menu();
  return;
 }
}

void list_f(){
  Dir dir = SPIFFS.openDir ("");
  while (dir.next ()) {
    Serial.println (dir.fileName ());
  }
}

void add_ft(){
  tft.fillScreen(0xe77c);
  tft.setTextColor(0x23d9, 0xe77c);
  tft.setCursor(0,7);
  tft.println("1.Take password from the  Serial Monitor and then   encrypt it.");
  tft.println("2.Generate new key for thewireless password projection.");
  tft.println("3.Take password from the  Serial Monitor and then   send it using wireless    password projection.");
  tft.println("4.Return to the main menu.");
  while (pr_key != 27){
    bus.tick();
    if (bus.gotData()) {
      myStruct data;
      bus.readData(data);
      // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
      ch = data.x;
      pr_key = int(ch);
    if (pr_key == 27 || pr_key == 52){
      disp_m_menu();
      return;
    }
    if (pr_key == 49){
      enc_from_serial();
      disp_m_menu();
      return;
    }
    if (pr_key == 50){
      set_key_for_pp(true);
      disp_m_menu();
      return;
    }
    if (pr_key == 51){
      proj_pass_from_Serial();
      disp_m_menu();
      return;
    }
   }
 }
}

void enc_from_serial(){
 while (pr_key != 27){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Paste the password to encrypt into the Serial Monitor:");
  Serial.println("Paste the password to encrypt:");
  String inp_str;
  while (!Serial.available()) {}
  inp_str = Serial.readString();
  int str_len = inp_str.length() + 1;
  char char_array[str_len];
  inp_str.toCharArray(char_array, str_len);
  Serial.println("Ciphertext:");
  int p = 0;
  while(str_len > p+1){
    incr_key();
    incr_second_key();
    split_by_eight(char_array, p, str_len, true);
    p+=8;
  }
  rest_k();
  rest_s_k();
  Serial.println("");
  disp_m_menu();
  return;
 }
}

void proj_pass_from_Serial(){
 while (pr_key != 27){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Paste the password to send into the Serial Monitor:");
  Serial.println("Paste the password to send:");
  String inp_str;
  while (!Serial.available()) {}
  inp_str = Serial.readString();
  int str_len = inp_str.length() + 1;
  char char_array[str_len];
  inp_str.toCharArray(char_array, str_len);
  Serial.println("Ciphertext:");
  int p = 0;
  while(str_len > p+1){
    incr_key();
    incr_second_key();
    split_by_eight_for_pass_proj(char_array, p, str_len);
    p+=8;
  }
  rest_k();
  rest_s_k();
  disp_m_menu();
  return;
 }
}

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  disp_m_menu();
  cur_pos = 0;
  pr_check = false;
  disp_cur_pos();
  m = 2;
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  back_k();
  back_s_k();
  n = false;
  bus.tick();
  if (bus.gotData()) {
    myStruct data;
    bus.readData(data);
    // 11 - Up arrow; 10 - Down arrow; 13 - Enter; 27 - Escape; 9 - Tab.
    ch = data.x;
    //Serial.println(ch);
    //Serial.println(int(ch));
    pr_key = int(ch);
    if (pr_key == 10)
      cur_pos++;
    if (pr_key == 11)
      cur_pos--;
    if (cur_pos < 0)
      cur_pos = 10;
    if (cur_pos > 10)
      cur_pos = 0;
    if (cur_pos == 0 && pr_key == 13)
      enc_rec();
    if (cur_pos == 1 && pr_key == 13)
      dec_rec();
    if (cur_pos == 2 && pr_key == 13)
      set_mp();
    if (cur_pos == 3 && pr_key == 13)
      set_iv();
    if (cur_pos == 4 && pr_key == 13)
      save_f();
    if (cur_pos == 5 && pr_key == 13)
      load_f();
    if (cur_pos == 6 && pr_key == 13)
      remove_f();
    if (cur_pos == 7 && pr_key == 13){
      Serial.println("");
      Serial.println("Stored files:");
      list_f();
      Serial.println("/////THE END OF THE LIST/////");
      Serial.println("");
    }
    if (cur_pos == 8 && pr_key == 13)
      project();
    if (cur_pos == 9 && pr_key == 13)
      hash_str();
    if (cur_pos == 10 && pr_key == 13)
      add_ft();
    disp_cur_pos();
  }
}
