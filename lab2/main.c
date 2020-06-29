#include <stdio.h>
#include <time.h>
 
extern char *tzname[];
 
int main() {
    struct tm *value_ptr;
    time_t value;
    int error;
 
    value = time(NULL);
    if (value == (time_t) -1) {
        perror("Get time failure");
        return 1;
    }
 
    error = setenv("TZ", "America/Los_Angeles",1);
    if (error == -1) {
        perror("Set TZ failure");
        return 1;
    }
 
    value_ptr = localtime(&value);
    if (value_ptr == NULL) {
        perror("Localtime failure");
        return 1;
    }
 
     printf("%d/%d/%02d %d:%02d %s\n",
            value_ptr->tm_mon + 1, value_ptr->tm_mday,
            value_ptr->tm_year + 1900, value_ptr->tm_hour,
            value_ptr->tm_min, tzname[value_ptr->tm_isdst]);
 
    return 0;
}