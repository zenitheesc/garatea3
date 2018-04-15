/*
   EEPROMCpp.cpp
   Created: 20/10/2017 16:23:50
   Author : Boludo
*/
#ifndef eeprom_h
#define eeprom_h
#include <inttypes.h>
class eeprom {
private:
    /* error id
      d7 - desligamento inesperado do sistema
      d6 - letra nao premitida durante compressão
      d5 - letra nao premitida durante descompressão
      d4 - memoria cheia
      d3 - leu ate o fim
      d2 - tentativa de ler em uma posição inacessivel (sem permissao ou maior que a memoria)
      d1 - tentativa de escrever em uma posição inacessivel (sem permissao ou maior que a memoria)
      d0 - tentativa de acessar uma posição inacessivel (sem permissao ou maior que a memoria)
    */
    /* bytes reservados no inicio da eeprom, protegidos de overwriting.
      0x00 mempos[1]
      0x01 mempos[0]
      0x02 err
    */
    //ziptable                  //tabela de compactação (pode colocar char)
    #define to0x0B 'n'
    #define to0x0C 'e'
    #define to0x0D 's'
    #define to0x0E 'w'
    #define to0x0F 'x'
    //agr vem o resto
    #define operationdelay 50   //tempo minimo de espera entre operações
    #define reserved 3          //numero de bytes reservados no começo da memoria
    #define pagesize 64         //tamanho maximo de uma page
    #define memorysize 32768    //tamanho da memoria em bytes
    uint16_t memcount;          //variavel auxiliar para esctira
    int id;
    typedef union {             //union para auxiliar na quebra do endereço principal da memoria
      uint16_t full;            //usa o full pra salvar o dado inteiro
      uint8_t half[2];          //dps le as metades :)
    } memdiv;
    memdiv mempos;              //aponta para a posição principal na eeprom
    memdiv temppos;             //aponta para uma posição temporaria na eeprom
public:
    uint8_t err;                                                        //log de erros
    void gotobyte(uint16_t x);                                          //move o ponto em que estão sendo salvos os dados na eeprom
    void writebyte(uint8_t data, uint16_t pos);                         //escreve um unico byte na posiçao especificada, sem restrições
    uint8_t readbyte(uint16_t pos);                                     //le um unico byte na posição especificada, sem restrições
    void init(uint8_t addr);                                            //inicializa a memoria (favor usar apenas uma vez), e checa se houve um desligamento inesperado
    void writestring(char *data, uint8_t mode);                         //escreve uma string na eeprom. Use mode = 1 para strings comprimidas e 0 para strings normais
    uint16_t readstring(uint16_t pos, char *bufferr, uint8_t mode);     //escreve uma string na eeprom. Use mode = 1 para strings comprimidas e 0 para strings normais
    void updatelogs(void);                                              //atualiza os dados de erro e posição de memoria na eeprom
    void resetlogs(void);                                               //reseta os dados de erro e posição salva na memoria da eeprom
    void switchto(uint8_t val);
    uint16_t zipstring(char *inp, char *out); 
    uint16_t unzipstring(char *inp, char *out);
} eeprom;
#endif
