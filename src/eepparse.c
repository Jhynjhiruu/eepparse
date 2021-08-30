#include "eepparse.h"

#include "shiftjis.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

static char check_dir(const char * path)
{
#ifdef WIN32
    if (mkdir(path))
#else
    if (mkdir(path, S_IRWXU))
#endif
    {
        if (errno == EEXIST)
        {
            struct stat stbuf;
            stat(path, &stbuf);
            if (S_ISDIR(stbuf.st_mode))
            {
                return 0;
            }
        }
        return -1;
    }
    return 0;
}

static char load_file(const char * filename, Eeprom * dest)
{
    off_t fsize, readlen;
    struct stat stbuf;
    int fd;
    FILE * fp;
    int error;
    
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }
    
    if ((fstat(fd, &stbuf) != 0) || (!S_ISREG(stbuf.st_mode)))
    {
        goto err;
    }
    
    fsize = stbuf.st_size;
    
    if (fsize != 0x2000)
    {
        perror("Invalid file size (must be 0x2000 bytes)");
        goto err;
    }
    
    fp = fdopen(fd, "rb");
    if (fp == NULL)
    {
        goto err;
    }
    
    readlen = fread(dest, 1, fsize, fp);
    if (readlen != fsize)
    {
        if (ferror(fp))
        {
            goto err;
        }
    }
    
    if (close(fd))
    {
        return -1;
    }
    return 0;
    
err:
    error = errno;
    close(fd);
    errno = error;
    return -1;
}

static char header_valid(SaveHeader hdr)
{
    static const uint8_t blankcheck[4] = { 0 };
    
    uint16_t sum = 0;
    for (uint8_t i = 0; i <  4; i++) sum += (uint8_t)hdr.code[i];
    for (uint8_t i = 0; i < 12; i++) sum += (uint8_t)hdr.gamename[i];
    
    if (sum == hdr.checksum && memcmp(hdr.code, blankcheck, 4)) return 1;
    else                                                        return 0;
}

static const char usage[] = "Usage: %s infile [outdir]";

int main(int argc, char * argv[])
{
    int rv;
    Eeprom eep;
    static const char * outdir = "out";
    
    if (argc < 2)
    {
        fprintf(stdout, usage, argv[0]);
        return 0;
    }
    else if (argc > 2)
    {
        outdir = argv[2];
    }
    
    if (check_dir(outdir))
    {
        perror("Could not initialise output directory");
        return -1;
    }
    
    if ((rv = load_file(argv[1], &eep)))
    {
        fprintf(stderr, "Error when loading %s, errno = %s\n", argv[1], strerror(errno));
        return -1;
    }
    
    if (strncmp(eep.magic, "GBMN", 4))
    {
        fprintf(stderr, "Invalid EEPROM magic %.4s (should be GBMN)\n", eep.magic);
        return -1;
    }
    
    for (uint8_t i = 0; i < 6; i++)
    {
        if (header_valid(eep.headers[i]))
        {
            FILE * fp;
            size_t writesize;
            
            char * name;
            size_t namelen = read_shiftjis_string(&name, eep.headers[i].gamename, 12);
            
            char filename[strlen(outdir) + 1 + 4 + namelen + 4 + 1];
            
            sprintf(filename, "%s/%.4s%s.bin", outdir, eep.headers[i].code, name);
            free(name);
            
            fp = fopen(filename, "wb");
            if (fp == NULL)
            {
                fprintf(stderr, "Could not open file %s, errno = %s\n", filename, strerror(errno));
                return -1;
            }
            
            writesize = fwrite(eep.saves[i], 1, 0x500, fp);
            fclose(fp);
            if (writesize != 0x500)
            {
                fprintf(stderr, "Could not write to file %s, errno = %s\n", filename, strerror(errno));
                return -1;
            }
        }
    }
    
    return 0;
}
