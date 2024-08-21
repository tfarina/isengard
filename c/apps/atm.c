#include <stdio.h>

#define SECRETCODE 4242

int
main(
  void
  )
{
  int pin = 0;
  int choice = 0;
  /*BOOL */ int quit = 0;
  unsigned long balance = 100;
  unsigned long amount = 0;

  while (pin != SECRETCODE)
  {
    fputs("ENTER YOUR PIN: ", stdout);
    scanf("%d", &pin);
    if (pin != SECRETCODE)
    {
      fputs("INVALID PIN CODE\n", stdout);
    }
  }

  do
  {
    fputs("\nWelcome to XYZ Bank ATM\n\n", stdout);
    fputs("    1. Check Balance\n", stdout);
    fputs("    2. Withdraw\n", stdout);
    fputs("    3. Deposit\n", stdout);
    fputs("    4. Quit\n", stdout);
    fputs("\nEnter your choice: ", stdout);
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      printf("\nYOUR BALANCE IS: %lu\n", balance);
      break;

    case 2:
      fputs("\nOPERATION NOT AVAILABLE!\n", stdout);
      break;

    case 3:
      fputs("\nENTER THE AMOUNT TO DEPOSIT: ", stdout);
      scanf("%lu", &amount);
      balance = balance + amount;
      break;

    case 4:
      quit = 1;
      break;

    default:
      fputs("\nINVALID CHOICE\n", stdout);
    }
  } while (!quit);

  return 0;
}
