//////////////////////////////////////////////////////////////
//                       TELEGRAPH RECEIVER CODE
//////////////////////////////////////////////////////////////
#include <stdio.h>
#include <Arduino.h>

//////////////////////////////////////////////////////////////
//                         Telegraph Constants
//////////////////////////////////////////////////////////////

// Time units (in seconds)
#define AVERAGE_DOT    0.1  // Base time unit for a dot

// Morse Code timing in time units
#define DOT            1     // Length of a dot in time units
#define DASH           3     // Length of a dash in time units

// Delays in time units
#define ITEM_DELAY     1     // Delay between dot/dash elements within a letter
#define LETTER_DELAY   3     // Delay between letters
#define WORD_DELAY     7     // Delay between words

// Defining Errors
#define invalid_string 1

// Define the Morse code mappings using '0' for dot and '1' for dash
const char* morse_code[46] = {
    "01",    // A: .-
    "1000",  // B: -...
    "1010",  // C: -.-.
    "100",   // D: -..
    "0",     // E: .
    "0010",  // F: ..-.
    "110",   // G: --.
    "0000",  // H: ....
    "00",    // I: ..
    "0111",  // J: .---
    "101",   // K: -.-
    "0100",  // L: .-..
    "11",    // M: --
    "10",    // N: -.
    "111",   // O: ---
    "0110",  // P: .--.
    "1101",  // Q: --.-
    "010",   // R: .-.
    "000",   // S: ...
    "1",     // T: -
    "001",   // U: ..-
    "0001",  // V: ...-
    "011",   // W: .--
    "1001",  // X: -..-
    "1011",  // Y: -.--
    "1100",  // Z: --..
    "11111", // 0: -----
    "01111", // 1: .----
    "00111", // 2: ..---
    "00011", // 3: ...--
    "00001", // 4: ....-
    "00000", // 5: .....
    "10000", // 6: -....
    "11000", // 7: --...
    "11100", // 8: ---..
    "11110", // 9: ----.
    "010101",// .: .-.-.-
    "110011",// ,: --..--
    "001100",// ?: ..--..
    "10010", // /: -..-.
    "011010",// @: .--.-.
    "10101"  // -: -....-
};

//////////////////////////////////////////////////////////////
//                         GPIO Constants
//////////////////////////////////////////////////////////////

// Pin to use
#define telegraph_pin 5

// Baud Rate
#define baud_rate 115200

// Buffer size for incoming string
#define BUFFER_SIZE 1000

// Buffer to hold incoming data
char receivedString[BUFFER_SIZE];
int bufferIndex = 0;

//////////////////////////////////////////////////////////////
//                           Setup
//////////////////////////////////////////////////////////////

void setup() {
  //setting pin modes
  pinMode(telegraph_pin, OUTPUT);

  //setting buad rate
  Serial.begin(baud_rate);

  // Clear the buffer
  memset(receivedString, 0, BUFFER_SIZE);
}

//////////////////////////////////////////////////////////////
//                           Main Code
//////////////////////////////////////////////////////////////

// Function to get the Morse code for a given character
const char* get_morse_code(char c) {
    if (c >= 'A' && c <= 'Z') {
        return morse_code[c - 'A'];
    } else if (c >= '0' && c <= '9') {
        return morse_code[c - '0' + 26];
    } else if (c == '.') {
        return morse_code[36];
    } else if (c == ',') {
        return morse_code[37];
    } else if (c == '?') {
        return morse_code[38];
    } else if (c == '/') {
        return morse_code[39];
    } else if (c == '@') {
        return morse_code[40];
    } else if (c == '-') {
        return morse_code[41];
    }
    return ""; // Return empty string for unsupported characters
}

// Function to print a single char
const int print_digit(char c) {
  if (c == '0') { // dot
    digitalWrite(telegraph_pin, HIGH);
    delay(AVERAGE_DOT * 1000 * DOT);
  } else { // dash
    digitalWrite(telegraph_pin, HIGH);
    delay(AVERAGE_DOT * 1000 * DASH);
  }
  // turning off the telegraph pin
  digitalWrite(telegraph_pin, LOW);
  delay(AVERAGE_DOT * 1000 * ITEM_DELAY);

  return 1;
}

// Function to print whole strings
void print_string(char* str) {
  // going char by char
  const char* morse_code_str;
  int idx = 0;
  
  while(str[idx] != '\0') {
    // checking if space
    if (str[idx] == ' ') {
      digitalWrite(telegraph_pin, LOW);
      delay(AVERAGE_DOT * 1000 * WORD_DELAY);
      idx++;
      continue;
    }

    // For non-space characters
    morse_code_str = get_morse_code(str[idx]);  // Get Morse code for current character
    int morse_idx = 0;
    
    while(morse_code_str[morse_idx] != '\0') {
      print_digit(morse_code_str[morse_idx]);
      morse_idx++;
    }
    // adding space between letters
    delay(AVERAGE_DOT * 1000 * LETTER_DELAY);

    idx++;  // Move to the next character
  }
}

//Function to handle errors
void error_handler(int error) {

}


//////////////////////////////////////////////////////////////
//                           Loop
//////////////////////////////////////////////////////////////

void loop() {
  // Main code here, runs repeatedly
  char* testing = "SOS";
  print_string(testing);
  delay(1000);
}
// void loop() {
//   // Check if data is available to read
//   if (Serial.available() > 0) {
//     // Read the incoming byte
//     char incomingByte = Serial.read();

//     // Check for end of line character (e.g., newline '\n')
//     if (incomingByte == '\n' || bufferIndex >= BUFFER_SIZE - 1) {
//       // Null-terminate the string
//       receivedString[bufferIndex] = '\0';

//       // Echo the received string back to the sender
//       Serial.print("Received: ");
//       Serial.println(receivedString);

//       //printing message:
//       print_string(receivedString);
//       // Clear the buffer for the next message
//       memset(receivedString, 0, BUFFER_SIZE);
//       bufferIndex = 0; // Reset the index
//     } else {
//       // Add the incoming byte to the buffer
//       receivedString[bufferIndex++] = incomingByte;
//     }
//   }
// }
