/*
   EEPROMCpp.cpp

   Created: 20/10/2017 16:23:50
   Author : Boludo
*/
#include "eeprom.h"
#include <Wire.h>
     void eeprom::gotobyte(uint16_t x) {         //move o ponto em que estão sendo salvos os dados na eeprom
      if ((x < memorysize) && (x > reserved)) {
        mempos.full = x;                //atualiza a posição do ponteiro
      }
      else {
        err = err | 0x01;               //seta o bit de erro d1
      }
    }
    void eeprom::writebyte(uint8_t data, uint16_t pos) { //escreve um unico byte na posiçao especificada, sem restrições
      if (pos < memorysize) {           //verifica se a posição ta dentro da memoria
        delay(5);                       //aguarda o tempo necessario pra dar o define "operationdelay", caso necessario
        temppos.full = pos;             //atualiza a posição temporaria de escrita para o byte
        Wire.beginTransmission(id);
        Wire.write(temppos.half[1]);
        Wire.write(temppos.half[0]);
        Wire.write(data);
        Wire.endTransmission();
      }
      else {
        err = err | 0x02;               //seta o bit de erro d2
      }
    }
    uint8_t eeprom::readbyte(uint16_t pos) {    //le um unico byte na posição especificada, sem restrições
      if (pos < memorysize) {           //verifica se a posição ta dentro da memoria
        delay(5);                       //aguarda o tempo necessario pra dar o define "operationdelay", caso necessario
        temppos.full = pos;             //atualiza a posição temporaria de escrita para o byte
        Wire.beginTransmission(id);     //daqui pra baixo n sei mais o q ta acontecendo
        Wire.write(temppos.half[1]);
        Wire.write(temppos.half[0]);
        Wire.endTransmission();
        Wire.requestFrom(id, 1);
        Wire.endTransmission();
        return Wire.read();
      }
      else {
        err = err | 0x04;
      }
      return 0;
    }
    void eeprom::init(uint8_t addr) {               //inicializa a memoria (favor usar apenas uma vez), e checa se houve um desligamento inesperado
      switchto(addr);
      mempos.half[1] = readbyte(0x00);      //le a ultima posição salva do ponteiro de escrita da eeprom
      mempos.half[0] = readbyte(0x01);      //le a ultima posição salva do ponteiro de escrita da eeprom
      err = readbyte(0x02);                 //carrega o log de erros da eeprom pra ram
      if (mempos.full == 0) {               //se a ultima posição de escrita for 0 o log de erros é limpo,
        mempos.full = reserved;             //joga o ponteiro de escrita para 1 byte dps da quantidade reservada (LEMBRANDO QUE A INDEXAÇÂO COMEÇA DO 0)
        err = 0;                            //limpa o log de erros
        memcount = 0;                       //limpa a quantidade de coisas escritas (var auxiliar)
      }
      else {
        err = err | 0x80;                   //seta o bit de erro d7 (desligamento inesperado)
      }
    }
    void eeprom::writestring(char *data, uint8_t mode) {          //escreve uma string na eeprom. Use mode = 1 para strings comprimidas e 0 para strings normais
      uint16_t offset = 0;                  //variavel pra caso venha mais q uma pagina (64bytes)
      uint8_t temcoisa = 1;                 //tem coisa
      while ( temcoisa && ( mempos.full != memorysize ) ) {                    //escrevendo a string
        memcount = 0;                       //zera o numero de BYTES escritos
        delay(5);                           //garante o delay minimo caso necessario
        Wire.beginTransmission(id);         //chama a biblioteca wire
        Wire.write(mempos.half[1]);
        Wire.write(mempos.half[0]);
        while ( temcoisa ) {                          //escrevendo a pagina
          Wire.write(data[memcount + offset]);      //escreve na eeprom o dado + offset
          if (data[memcount + offset] == '\0') {    //se acabar a string para de escrever
            temcoisa = 0;                           //acabou a coisa
          }
          else if ( ( mode == 1 ) && ( ( 0x0F & data[memcount + offset]) == '\0' ) ){
            temcoisa = 0;
          }
          memcount++;                               //incrementa o numero de coisas escritas
          if ( ( mempos.full + memcount ) == memorysize ){
            temcoisa = 0;
            err = err | 0x10;
          }
          if (((memcount + mempos.full) % 64) == 0 || memcount == 30 ) {
            break;
          }
        }
        Wire.endTransmission();
        offset += memcount;                         //atualiza o offset da leitura da string
        mempos.full += memcount;                    //atualiza o ponteiro de escrita
      }
    }
    uint16_t eeprom::readstring(uint16_t pos, char *bufferr, uint8_t mode) {  //escreve uma string na eeprom. Use mode = 1 para strings comprimidas e 0 para strings normais
      uint16_t offset = 0;
      uint8_t temcoisa = 1;
      temppos.full = pos;
      while (temcoisa && ( temppos.full != memorysize )) {
        memcount = 0;
        delay(5);
        Wire.beginTransmission(id);
        Wire.write(temppos.half[1]);
        Wire.write(temppos.half[0]);
        Wire.endTransmission();
        Wire.requestFrom(id, 64 - (memcount + temppos.full) % 64);
        while (temcoisa) {
          bufferr[memcount + offset] = Wire.read();
          if (bufferr[memcount + offset] == '\0') {
            temcoisa = 0;
          }
          else if ( ( mode == 1 ) && ( ( 0x0F & bufferr[memcount + offset]) == '\0' ) ){
            temcoisa = 0;
          }
          memcount++;
          if ( ( temppos.full + memcount ) == memorysize ){
            temcoisa = 0;
            err = err | 0x08;
          }
          if (((memcount + temppos.full) % 64) == 0 || memcount == 32) {
            break;
          }
        }
        Wire.endTransmission();
        offset += memcount;
        temppos.full += memcount;
      }
      return temppos.full;
    }
    void eeprom::updatelogs(void) {           //atualiza os dados de erro e posição de memoria na eeprom
      writebyte(mempos.half[1], 0x00);
      writebyte(mempos.half[0], 0x01);
      writebyte(err, 0x02);
    }
    void eeprom::resetlogs(void) {            //reseta os dados de erro e posição salva na memoria da eeprom
      writebyte(0x00, 0x00);
      writebyte(0x00, 0x01);
      writebyte(0x00, 0x02);
    }
    void eeprom::switchto(uint8_t val) {
      id = 0b01010000 | (0b00000111 & val);
    }
    uint16_t eeprom::zipstring(char *inp, char *out) {
      uint16_t pos = 0;
      uint16_t outpos = 0;
      uint8_t esquerda;
      uint8_t temcoisa = 1;
      while (temcoisa) {
        if (!(pos & 0x01)) {
          esquerda = 4;
          out[outpos] = 0;
        }
        else {
          esquerda = 0;
        }
        if ((0x30 <= inp[pos]) && (inp[pos] <= 0x39)) {
          out[outpos] += (inp[pos] - 0x2F) << esquerda;
        }
        else if (inp[pos] == to0x0B) {
          out[outpos] += 0x0B << esquerda;
        }
        else if (inp[pos] == to0x0C) {
          out[outpos] += 0x0C << esquerda;
        }
        else if (inp[pos] == to0x0D) {
          out[outpos] += 0x0D << esquerda;
        }
        else if (inp[pos] == to0x0E) {
          out[outpos] += 0x0E << esquerda;
        }
        else if (inp[pos] == to0x0F) {
          out[outpos] += 0x0F << esquerda;
        }
        else if (inp[pos] == '\0') {
          if (esquerda) {
            out[outpos] = '\0';
          }
          temcoisa = 0;
        }
        else {
          err = 0x40;
        }
        pos++;
        if (!esquerda) {
          outpos++;
        }
      }
      return outpos;
    }
    uint16_t eeprom::unzipstring(char *inp, char *out) {
      uint16_t pos = 0;
      uint16_t outpos = 0;
      uint8_t temcoisa = 1;
      while ((inp[pos] != '\0') && (temcoisa)) {
        out[outpos] = ((inp[pos] & 0xF0) >> 4);
        outpos++;
        if ((inp[pos] & 0x0F) != '\0') {
          out[outpos] = inp[pos] & 0x0F;
          outpos++;
        }
        else {
          temcoisa = 0;
        }
        pos++;
      }
      out[outpos] = '\0';
      outpos = 0;
      temcoisa = 1;
      while (temcoisa) {
        if ((0x01 <= out[outpos]) && (out[outpos] <= 0x0A)) {
          out[outpos] = (out[outpos] + 0x2F);
        }
        else if (out[outpos] == 0x0B) {
          out[outpos] = to0x0B;
        }
        else if (out[outpos] == 0x0C) {
          out[outpos] = to0x0C;
        }
        else if (out[outpos] == 0x0D) {
          out[outpos] = to0x0D;
        }
        else if (out[outpos] == 0x0E) {
          out[outpos] = to0x0E;
        }
        else if (out[outpos] == 0x0F) {
          out[outpos] = to0x0F;
        }
        else if (out[outpos] == '\0') {
          temcoisa = 0;
        }
        else {
          err = 0x20;
        }
        outpos++;
      }
      out[outpos] = '\0';
      return outpos;
    }
