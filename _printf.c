#include "custom_printer.h"
#include <limits.h>
#include <stdio.h>

/**
 * unique_printf - Generates formatted output based on a format string.
 * @format: The format string containing characters and conversion specifiers.
 *
 * Description: This function utilizes the resolve_printer() function to identify
 * the appropriate printing function to apply, taking into account conversion
 * specifiers within the format string.
 *
 * Returns: The length of the formatted output string.
 */
int unique_printf(const char *format, ...)
{
    int (*printer)(va_list, flags_t *);
    const char *iterator;
    va_list arguments;
    flags_t flags = {0, 0, 0};

    int output_length = 0;

    va_start(arguments, format);

    if (!format || (format[0] == '%' && !format[1]))
        return (-1);

    if (format[0] == '%' && format[1] == ' ' && !format[2])
        return (-1);

    for (iterator = format; *iterator; iterator++)
    {
        if (*iterator == '%')
        {
            iterator++;
            if (*iterator == '%')
            {
                output_length += unique_putchar('%');
                continue;
            }
            while (inspect_flags(*iterator, &flags))
                iterator++;
            printer = resolve_printer(*iterator);
            output_length += (printer)
                ? printer(arguments, &flags)
                : unique_printf("%%%c", *iterator);
        }
        else
            output_length += unique_putchar(*iterator);
    }

    unique_putchar(-1);
    va_end(arguments);
    return output_length;
}
