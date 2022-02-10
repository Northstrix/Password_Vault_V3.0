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
#include "GBUS.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#define TFT_CS         D2
#define TFT_RST        D3
#define TFT_DC         D4
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
SoftwareSerial mySerial(5, 16); // RX, TX
GBUS bus(&mySerial, 3, 25);
int cur_pos;
char ch;
int pr_key;
struct myStruct {
  char x;
};
char *keys[]=
{"9a93d3e3e81ef9a9195fffffd41ae7a7a5b8262f33fe44b199559591a0e195c6"};// Serpent's key
int count;
byte tmp_st[8];
int m;
String dec_st;
String keyb_inp;
uint8_t back_key[32];
uint8_t back_s_key[32];
uint8_t key[32] = {
   0xb7,0x64,0x71,0x2b,
   0x81,0x4b,0xf1,0x7c,
   0xaf,0x3a,0x1f,0x63,
   0xe2,0x87,0x32,0x0e,
   0x56,0x0d,0xcf,0xdc,
   0xe9,0x88,0x4a,0x55,
   0xe1,0xec,0x11,0xb2,
   0x97,0xc3,0x78,0x94
};

uint8_t second_key[32] = {
   0xbf,0x91,0x36,0x23,
   0x53,0x6a,0xb5,0xdb,
   0x92,0x72,0xe8,0xad,
   0x3b,0xba,0x57,0x38,
   0x17,0x5d,0x20,0x7c,
   0x70,0x26,0xe7,0x65,
   0x41,0x10,0xc5,0xe5,
   0x82,0x69,0xce,0x76
};

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

void split_by_eight_for_AES(char plntxt[], int k, int str_len){
  char res[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for(int i = 0; i < 8; i++){
      if(i+k > str_len - 1)
      break;
      res[i] = plntxt[i+k];
  }
  for(int i = 8; i<16; i++){
    res[i] = gen_r_num();
  }
  encr_AES_only(res);
}

void encr_AES_only(char t_enc[]){
  uint8_t text[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for(int i = 0; i<16; i++){
    int c = int(t_enc[i]);
    text[i] = c;
  }
  uint8_t cipher_text[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint32_t key_bit[3] = {128, 192, 256};
  int i = 0;
  aes_context ctx;
  aes_set_key(&ctx, key, key_bit[m]);
  aes_encrypt_block(&ctx, cipher_text, text);
  for (i = 0; i < 16; ++i) {
    Serial.printf("%02x", cipher_text[i]);
  }
}

void split_dec_for_AES(char ct[], int ct_len, int p){
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
      uint8_t ret_text[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      uint8_t cipher_text[16] = {0};
      for(int i = 0; i<16; i++){
        int c = int(res[i]);
        cipher_text[i] = c;
      }
      uint32_t key_bit[3] = {128, 192, 256};
      int i = 0;
      aes_context ctx;
      aes_set_key(&ctx, key, key_bit[m]);
      aes_decrypt_block(&ctx, ret_text, cipher_text);
      for (i = 0; i < 8; ++i) {
        Serial.print(char(ret_text[i]));
      }
   }
}

void split_by_eight_for_Serpent_only(char plntxt[], int k, int str_len){
  char res[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 8; i++){
      if(i+k > str_len - 1)
      break;
      res[i] = plntxt[i+k];
  }
  for (int i = 8; i < 16; i++){
      res[i] = gen_r_num();
  }
  int tmp_s[16];
  for(int i = 0; i < 16; i++){
      tmp_s[i] = res[i];
  }
  /*
   for (int i = 0; i < 8; i++){
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
  }
}

void split_dec_for_Serpent_only(char ct[], int ct_len, int p){
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
    for (int i=0; i<8; i++) {
      dec_st += char(ct2.b[i]);
    }
  }
}
void disp_m_menu(){
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
  tft.setTextSize(1);
  tft.setCursor(15,4);
  tft.print("Encrypt password");
  tft.setCursor(15,14);
  tft.print("Decrypt password");
  tft.setCursor(15,24);
  tft.print("Set master password");
  tft.setCursor(15,34);
  tft.print("Set IV");
  tft.setCursor(15,44);
  tft.print("Save file into memory");
  tft.setCursor(15,54);
  tft.print("Load file from memory");
  tft.setCursor(15,64);
  tft.print("Remove file from memory");
  tft.setCursor(15,74);
  tft.print("List all stored files");
  tft.setCursor(15,84);
  tft.print("Project password");
  tft.setCursor(15,94);
  tft.print("Hash string with SHA512");
}

void disp_cur_pos(){
  tft.setTextColor(0xdefb, 0x2145);
  tft.setTextSize(1);
  if (cur_pos == 0){
    tft.setCursor(5,4);
    tft.print("*");
    tft.setCursor(5,14);
    tft.print(" ");
    tft.setCursor(5,24);
    tft.print(" ");
    tft.setCursor(5,34);
    tft.print(" ");
    tft.setCursor(5,44);
    tft.print(" ");
    tft.setCursor(5,54);
    tft.print(" ");
    tft.setCursor(5,64);
    tft.print(" ");
    tft.setCursor(5,74);
    tft.print(" ");
    tft.setCursor(5,84);
    tft.print(" ");
    tft.setCursor(5,94);
    tft.print(" ");
  }
  if (cur_pos == 1){
    tft.setCursor(5,4);
    tft.print(" ");
    tft.setCursor(5,14);
    tft.print("*");
    tft.setCursor(5,24);
    tft.print(" ");
    tft.setCursor(5,34);
    tft.print(" ");
    tft.setCursor(5,44);
    tft.print(" ");
    tft.setCursor(5,54);
    tft.print(" ");
    tft.setCursor(5,64);
    tft.print(" ");
    tft.setCursor(5,74);
    tft.print(" ");
    tft.setCursor(5,84);
    tft.print(" ");
    tft.setCursor(5,94);
    tft.print(" ");
  }
  if (cur_pos == 2){
    tft.setCursor(5,4);
    tft.print(" ");
    tft.setCursor(5,14);
    tft.print(" ");
    tft.setCursor(5,24);
    tft.print("*");
    tft.setCursor(5,34);
    tft.print(" ");
    tft.setCursor(5,44);
    tft.print(" ");
    tft.setCursor(5,54);
    tft.print(" ");
    tft.setCursor(5,64);
    tft.print(" ");
    tft.setCursor(5,74);
    tft.print(" ");
    tft.setCursor(5,84);
    tft.print(" ");
    tft.setCursor(5,94);
    tft.print(" ");
  }
  if (cur_pos == 3){
    tft.setCursor(5,4);
    tft.print(" ");
    tft.setCursor(5,14);
    tft.print(" ");
    tft.setCursor(5,24);
    tft.print(" ");
    tft.setCursor(5,34);
    tft.print("*");
    tft.setCursor(5,44);
    tft.print(" ");
    tft.setCursor(5,54);
    tft.print(" ");
    tft.setCursor(5,64);
    tft.print(" ");
    tft.setCursor(5,74);
    tft.print(" ");
    tft.setCursor(5,84);
    tft.print(" ");
    tft.setCursor(5,94);
    tft.print(" ");
  }
  if (cur_pos == 4){
    tft.setCursor(5,4);
    tft.print(" ");
    tft.setCursor(5,14);
    tft.print(" ");
    tft.setCursor(5,24);
    tft.print(" ");
    tft.setCursor(5,34);
    tft.print(" ");
    tft.setCursor(5,44);
    tft.print("*");
    tft.setCursor(5,54);
    tft.print(" ");
    tft.setCursor(5,64);
    tft.print(" ");
    tft.setCursor(5,74);
    tft.print(" ");
    tft.setCursor(5,84);
    tft.print(" ");
    tft.setCursor(5,94);
    tft.print(" ");
  }
  if (cur_pos == 5){
    tft.setCursor(5,4);
    tft.print(" ");
    tft.setCursor(5,14);
    tft.print(" ");
    tft.setCursor(5,24);
    tft.print(" ");
    tft.setCursor(5,34);
    tft.print(" ");
    tft.setCursor(5,44);
    tft.print(" ");
    tft.setCursor(5,54);
    tft.print("*");
    tft.setCursor(5,64);
    tft.print(" ");
    tft.setCursor(5,74);
    tft.print(" ");
    tft.setCursor(5,84);
    tft.print(" ");
    tft.setCursor(5,94);
    tft.print(" ");
  }
  if (cur_pos == 6){
    tft.setCursor(5,4);
    tft.print(" ");
    tft.setCursor(5,14);
    tft.print(" ");
    tft.setCursor(5,24);
    tft.print(" ");
    tft.setCursor(5,34);
    tft.print(" ");
    tft.setCursor(5,44);
    tft.print(" ");
    tft.setCursor(5,54);
    tft.print(" ");
    tft.setCursor(5,64);
    tft.print("*");
    tft.setCursor(5,74);
    tft.print(" ");
    tft.setCursor(5,84);
    tft.print(" ");
    tft.setCursor(5,94);
    tft.print(" ");
  }
  if (cur_pos == 7){
    tft.setCursor(5,4);
    tft.print(" ");
    tft.setCursor(5,14);
    tft.print(" ");
    tft.setCursor(5,24);
    tft.print(" ");
    tft.setCursor(5,34);
    tft.print(" ");
    tft.setCursor(5,44);
    tft.print(" ");
    tft.setCursor(5,54);
    tft.print(" ");
    tft.setCursor(5,64);
    tft.print(" ");
    tft.setCursor(5,74);
    tft.print("*");
    tft.setCursor(5,84);
    tft.print(" ");
    tft.setCursor(5,94);
    tft.print(" ");
  }
  if (cur_pos == 8){
    tft.setCursor(5,4);
    tft.print(" ");
    tft.setCursor(5,14);
    tft.print(" ");
    tft.setCursor(5,24);
    tft.print(" ");
    tft.setCursor(5,34);
    tft.print(" ");
    tft.setCursor(5,44);
    tft.print(" ");
    tft.setCursor(5,54);
    tft.print(" ");
    tft.setCursor(5,64);
    tft.print(" ");
    tft.setCursor(5,74);
    tft.print(" ");
    tft.setCursor(5,84);
    tft.print("*");
    tft.setCursor(5,94);
    tft.print(" ");
  }
  if (cur_pos == 9){
    tft.setCursor(5,4);
    tft.print(" ");
    tft.setCursor(5,14);
    tft.print(" ");
    tft.setCursor(5,24);
    tft.print(" ");
    tft.setCursor(5,34);
    tft.print(" ");
    tft.setCursor(5,44);
    tft.print(" ");
    tft.setCursor(5,54);
    tft.print(" ");
    tft.setCursor(5,64);
    tft.print(" ");
    tft.setCursor(5,74);
    tft.print(" ");
    tft.setCursor(5,84);
    tft.print(" ");
    tft.setCursor(5,94);
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
  Serial.println("Enter ciphertext");
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
  Serial.println("Plaintext");
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
  //Serial.println(dec_st);
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
        tft.fillScreen(0x2145);
        if(keyb_inp.length() > 0)
          keyb_inp.remove(keyb_inp.length() -1, 1);
      }
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the string to hash:");
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
  tft.fillScreen(0x2145);
  tft.setTextColor(0xdefb, 0x2145);
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
        tft.fillScreen(0x2145);
        if(keyb_inp.length() > 0)
          keyb_inp.remove(keyb_inp.length() -1, 1);
      }
  tft.setTextColor(0xdefb, 0x2145);
  tft.setCursor(0,0);
  tft.println("Enter the string to hash:");
  tft.setTextColor(0xf7be, 0x2145);
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
    tft.fillScreen(0x2145);
    tft.setTextColor(0xdefb, 0x2145);
    tft.setCursor(0,0);
    tft.println("Resulted hash:");
    tft.setTextColor(0xf7be, 0x2145);
    tft.setCursor(0,10);
    tft.println(h);
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

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  disp_m_menu();
  cur_pos = 0;
  disp_cur_pos();
}

void loop() {
  back_k();
  back_s_k();
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
      cur_pos = 9;
    if (cur_pos > 9)
      cur_pos = 0;
    if (cur_pos == 0 && pr_key == 13)
      enc_rec();
    if (cur_pos == 1 && pr_key == 13)
      dec_rec();
    if (cur_pos == 2 && pr_key == 13)
      set_mp();
    if (cur_pos == 3 && pr_key == 13)
      set_iv();
    if (cur_pos == 9 && pr_key == 13)
      hash_str();
    disp_cur_pos();
  }
}
