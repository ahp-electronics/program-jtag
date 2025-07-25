#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <urjtag.h>

int main(int argc, char **argv)
{
    int opt;
    int params_n = 0;
    int device_n = 0;
    long frequency = 0;
    char *svf, *driver, *bsdl_path;
    char *params_bundle = NULL;
    char **params = NULL;
    svf = driver = NULL;
    if (argc < 1)
        return ENOENT;
    while ((opt = getopt(argc, argv, "i:b:f:d:n:v")) != -1) {
        switch (opt) {
        case 'i':
            svf = optarg;
            break;
        case 'f':
            frequency = atol(optarg);
            break;
        case 'd':
            driver = optarg;
            break;
        case 'b':
            bsdl_path = optarg;
            break;
        case 'n':
            device_n = atoi(optarg);
            break;
        case 'p':
            params_bundle = optarg;
            params[params_n++] = strtok(params_bundle, ",");
            while(params[params_n++] != NULL)
                params[params_n] = strtok(NULL, ",");
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-i svf] [-d driver] [-f frequency] [-n device_number] [-v vendor_id] [-p product_id] [-b bsdl_path]\n",
            argv[0]);
            return EINVAL;
        }
    }
    if(svf != NULL && driver != NULL) {
        int fd = open(svf, O_RDONLY);
        if(fd > -1) {
            return program_jtag(fd, driver, (const char**)params, bsdl_path, frequency, device_n);
        }
    }
    return 0;
}
