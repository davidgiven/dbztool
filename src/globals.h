#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
typedef std::basic_string<uint8_t> Bytes;

#define VERSION "0.1"
#define SlowBaudRate 19200

/* Global options. */

extern bool Verbose;
extern const char* SerialPort;

/* Utilities. */

extern void error(const char* message, ...);
extern void verbose(const char* message, ...);
extern void warning(const char* message, ...);
extern std::string aprintf(const char* fmt, ...);
extern void resettimer();
extern double gettime();
extern void pad_with_nops(Bytes& stub);
extern void writebe32(uint8_t* dest, uint32_t value);
extern uint16_t readbe16(const uint8_t* ptr);

/* Serial port management. */

extern void logon();
extern void dodgyterm();
extern void sendbyte(uint8_t c);
extern void send(const std::string& s);
extern uint8_t recvbyte();

/* B-record protocol. */

extern void brecord_execute(uint32_t addr);
extern void brecord_write(uint32_t addr, uint8_t count, const uint8_t* data);

/* Operations */

extern Bytes buffer_read(uint32_t start, uint32_t length);

extern void cmd_write(char** argv);
extern void cmd_read(char** argv);
extern void cmd_execute(char** argv);
extern void cmd_cs(char** argv);

#endif

