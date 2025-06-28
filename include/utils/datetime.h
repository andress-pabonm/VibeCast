#ifndef VIBECAST_DATETIME_H
#define VIBECAST_DATETIME_H 1

// Tamaño del búfer para getTime()
#define TIME_BUFSIZ 9

// Tamaño del búfer para getDate()
#define DATE_BUFSIZ 11

// Tamaño del búfer para getDateTime()
#define DATETIME_BUFSIZ 21

// Tipo para la fecha y hora
typedef void *datetime_t;

// Tipo de búfer para getTime()
typedef char time_buf_t[TIME_BUFSIZ];

// Tipo de búfer para getDate()
typedef char date_buf_t[DATE_BUFSIZ];

// Tipo de búfer para getDateTime()
typedef char datetime_buf_t[DATETIME_BUFSIZ];

// Para obtener la fecha y hora actual
datetime_t now();

// Para obtener un string con la hora (hh:mm:ss)
void getTime(time_buf_t buf, datetime_t dt);

// Para obtener un string con la fecha (yyyy-mm-dd)
void getDate(date_buf_t buf, datetime_t dt);

// Para obtener un string combinado entre fecha y hora (yyyy-mm-dd hh:mm:ss)
void getDateTime(datetime_buf_t buf, datetime_t dt);

#endif // VIBECAST_DATETIME_H