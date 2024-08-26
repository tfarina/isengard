#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_PASSWORD_LENGTH 64

/* https://stackoverflow.com/a/78462414
 */
static
int
disable_raw_mode(
  void)
{
  struct termios term;

  if (tcgetattr(STDIN_FILENO, &term) != 0)
    return -1;

  term.c_lflag &= ~(ECHO | ICANON);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) != 0)
    return -1;

  return 0;
}

static
int
enable_raw_mode(
  void)
{
  struct termios term;

  if (tcgetattr(STDIN_FILENO, &term) != 0)
    return -1;

  term.c_lflag |= ECHO | ICANON;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) != 0)
    return -1;

  return 0;
}

int
main(
  void)
{
  int rc = 0;
  char password[MAX_PASSWORD_LENGTH];

  rc = disable_raw_mode();
  if (rc < 0)
    return 1;

  fputs("Enter password: ", stdout);
  fgets(password, sizeof(password), stdin);

  rc = enable_raw_mode();
  if (rc < 0)
    return 1;

  fputc('\n', stdout);

  printf("You typed: %s", password);

  return 0;
}
