int
main(int argc, char *argv[])
{
  char *s;
  char *progname;

  progname = argv[0];

  while (--argc > 0 && (*++argv)[0]=='-') {
    for (s = argv[0]+1; *s; s++)
      switch (*s) {
        default:
          return (-1);
          break;
      }
  }
  return 0;
}