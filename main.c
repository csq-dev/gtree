#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

float get_file_size(const char *path);
float file_recurser(const char *path);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <path>\n", argv[0]);
        return 1;
    }
    float total_size = file_recurser(argv[1]);
    printf("Total size of %s is %f bytes\n", argv[1], total_size);
    return 0;
}

float get_file_size(const char *path)
{
    struct stat st;
    stat(path, &st);
    return st.st_size;
}

float file_recurser(const char *path)
{
    DIR *dir;
    struct dirent *ent;
    float total_size = 0;
    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_type == DT_REG)
            {
                char *full_path = malloc(strlen(path) + strlen(ent->d_name) + 2);
                strcpy(full_path, path);
                strcat(full_path, "/");
                strcat(full_path, ent->d_name);
                total_size += get_file_size(full_path);
                free(full_path);
            }
            else if (ent->d_type == DT_DIR)
            {
                if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                {
                    char *full_path = malloc(strlen(path) + strlen(ent->d_name) + 2);
                    strcpy(full_path, path);
                    strcat(full_path, "/");
                    strcat(full_path, ent->d_name);
                    total_size += file_recurser(full_path);
                    free(full_path);
                }
            }
        }
        closedir(dir);
    }
    return total_size;
}