#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXLEN 80
#define EXTRA 5
/* 4 for field name "data", 1 for "=" */
#define MAXINPUT MAXLEN+EXTRA+2
/* 1 for added line break, 1 for trailing NUL */
#define DATAFILE "../../../var/www/html/data.json"


void unencode(char *src, char *last, char *dest)
{
  *dest = '\n';
  dest++;
  *dest = '\"';
  dest++;
 for(; src != last; src++, dest++)
   if(*src == '+')
     *dest = ' ';
   else if(*src == '%') {
     int code;
     if(sscanf(src+1, "%2x", &code) != 1) code = '?';
     *dest = code;
     src +=2; }     
   else if(*src == '&'){
     *dest='\"';
     dest++;
     *dest=',';
     dest++;
     *dest='\n';
     dest++;
     *dest = '\"';
   }
   else if (*src == '='){
    *dest = '\"';
    dest++;
    *dest = ':';
    dest++;
    *dest = '\"';
   }
   else
     *dest = *src;
 *dest = '\"';
 dest++;
  *dest = ',';
 dest++;
 *dest='\n';
 *++dest = '\0';
}

int main(void)
{
char *lenstr;
char input[MAXINPUT], data[MAXINPUT];
long len;
printf("%s%c%c\n",
"Content-Type:text/html;charset=iso-8859-1",13,10);
printf("<TITLE>Response</TITLE>\n");

lenstr = getenv("CONTENT_LENGTH");
printf("<P>len: %d", lenstr);
if(lenstr == NULL || sscanf(lenstr,"%ld",&len)!=1 || len > MAXLEN)
  printf("<P>Error in invocation - wrong FORM probably.");
else {
printf("<P>Results output: %s", stdin);
  FILE *f;
  fgets(input, len+1, stdin);
  unencode(input, input+len, data);
    time_t current_time;
    char* c_time_string;

    /* Obtain current time. */
    current_time = time(NULL);

    if (current_time == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

    if (c_time_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }
  char output[200];
  c_time_string[strlen(c_time_string)-1] = '\"';
  sprintf(output, "\n{%s\"timestamp\":\"%s}]}", data, c_time_string);
  char buffer[200];
  printf("<P>Results output: %s", output);

  f = fopen(DATAFILE, "r+");
  if(f == NULL)
    printf("<P>Sorry, cannot store your data.");
  else
  fseek( f, -3, SEEK_END );
  char ch = fgetc(f);
    printf("<P>Results char: %c", ch);


  if(ch !='['){
    fclose(f);
    f = fopen(DATAFILE, "r+");

    fseek( f, -3, SEEK_END);
    ch = fgetc(f);

    printf("<P>Results char: %c", ch);

    sprintf(buffer, ",%s", output);
    printf("<P>Results char: %s", buffer);
    fputs(buffer, f);
  }
  else{
    fseek( f, 0, SEEK_CUR );
    fputs(output, f);
  }
  fclose(f);
  printf("<P>Thank you! Your contribution has been stored.");
  }
return 0;
}
