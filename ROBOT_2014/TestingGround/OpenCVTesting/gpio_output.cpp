/**
 * @file gpio_output
 * @brief Simple GPIO output
 * @author D. Butenhoff
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main(int argc, char** argv)
{
  int GPIOPin50 = 50; // GPIO1_18 or pin 14 on the P9 header
  int GPIOPin60 = 60; // GPIO1_28 or pin 12 on the P9 header
  FILE *myOutputHandle = NULL;
  char setValue[4];
  char GPIO50String[4], GPIO50Value[64], GPIO50Direction[64];
  sprintf(GPIO50String, "%d", GPIOPin50);
  sprintf(GPIO50Value, "/sys/class/gpio/gpio%d/value", GPIOPin50);
  sprintf(GPIO50Direction, "/sys/class/gpio/gpio%d/direction", GPIOPin50);
  char GPIO60String[4], GPIO60Value[64], GPIO60Direction[64];
  sprintf(GPIO60String, "%d", GPIOPin60);
  sprintf(GPIO60Value, "/sys/class/gpio/gpio%d/value", GPIOPin60);
  sprintf(GPIO60Direction, "/sys/class/gpio/gpio%d/direction", GPIOPin60);

  // Export the pins
  if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
      printf("Unable to export GPIO pin\n");
      return 1;
  }
  strcpy(setValue, GPIO50String);
  fwrite(&setValue, sizeof(char), 2, myOutputHandle);
  fclose(myOutputHandle);
  if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
      printf("Unable to export GPIO pin\n");
      return 1;
  }
  strcpy(setValue, GPIO60String);
  fwrite(&setValue, sizeof(char), 2, myOutputHandle);
  fclose(myOutputHandle);

  // Set direction of the pins to output
  if ((myOutputHandle = fopen(GPIO50Direction, "rb+")) == NULL){
      printf("Unable to open direction 50 handle\n");
      return 1;
  }
  strcpy(setValue,"out");
  fwrite(&setValue, sizeof(char), 3, myOutputHandle);
  fclose(myOutputHandle);
  if ((myOutputHandle = fopen(GPIO60Direction, "rb+")) == NULL){
      printf("Unable to open direction 60 handle\n");
      return 1;
  }
  strcpy(setValue,"out");
  fwrite(&setValue, sizeof(char), 3, myOutputHandle);
  fclose(myOutputHandle);

  for( int i = 0; i < 10; i++ )
  {
    // Set output to 50 high, 60 low
    if ((myOutputHandle = fopen(GPIO50Value, "rb+")) == NULL){
      printf("Unable to open value handle\n");
      return 1;
    }
    strcpy(setValue, "1"); // Set value high
    fwrite(&setValue, sizeof(char), 1, myOutputHandle);
    fclose(myOutputHandle);
    if ((myOutputHandle = fopen(GPIO60Value, "rb+")) == NULL){
      printf("Unable to open value handle\n");
      return 1;
    }
    strcpy(setValue, "0"); // Set value low
    fwrite(&setValue, sizeof(char), 1, myOutputHandle);
    fclose(myOutputHandle);
    sleep(5); // wait for 5 sec

    // Set output to 50 low, 60 high
    if ((myOutputHandle = fopen(GPIO50Value, "rb+")) == NULL){
      printf("Unable to open value handle\n");
      return 1;
    }
    strcpy(setValue, "0"); // Set value low
    fwrite(&setValue, sizeof(char), 1, myOutputHandle);
    fclose(myOutputHandle);
    if ((myOutputHandle = fopen(GPIO60Value, "rb+")) == NULL){
      printf("Unable to open value handle\n");
      return 1;
    }
    strcpy(setValue, "1"); // Set value high
    fwrite(&setValue, sizeof(char), 1, myOutputHandle);
    fclose(myOutputHandle);
    sleep(5); // wait for 5 sec
  }

  // Unexport the pins
  if ((myOutputHandle = fopen("/sys/class/gpio/unexport", "ab")) == NULL) {
    printf("Unable to unexport GPIO pin\n");
    return 1;
  }
  strcpy(setValue, GPIO50String);
  fwrite(&setValue, sizeof(char), 2, myOutputHandle);
  fclose(myOutputHandle);
  if ((myOutputHandle = fopen("/sys/class/gpio/unexport", "ab")) == NULL) {
    printf("Unable to unexport GPIO pin\n");
    return 1;
  }
  strcpy(setValue, GPIO60String);
  fwrite(&setValue, sizeof(char), 2, myOutputHandle);
  fclose(myOutputHandle);
  return 0;
}
